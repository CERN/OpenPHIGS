#version 430 compatibility
/*
 * Order independent rendering, pass 2 of 2: resolve.
 *
 * One invocation per pixel, run after all geometry has been rasterised by
 * pass 1. It walks the linked list of transparent fragments belonging to the
 * pixel, sorts them back to front and composites them into a single colour.
 *
 * Only transparent fragments are in the list. Opaque geometry was written
 * straight to the framebuffer by pass 1, so the result of this pass is
 * blended over it, which is what lets an opaque track stay visible behind a
 * transparent surface.
 *
 * The bindings have to match the ones wsgl_oir_reset() sets up, and the ones
 * fs420.frag appends through.
 */
layout (binding = 0, r32ui)    coherent uniform uimage2D     head_pointer_image;
layout (binding = 1, rgba32ui) coherent uniform uimageBuffer list_buffer;
uniform uint list_capacity;

#define MAX_FRAGMENTS 16
#define LIST_END 0xFFFFFFFFu
/*
  Hard cap on how many links of a pixel's chain we are willing to follow.
  The chain is built by concurrent appends, so a torn entry can leave a bogus
  next index and, in the worst case, a cycle. The walk below no longer stops
  at MAX_FRAGMENTS, so without this bound a corrupt list would hang the
  shader, and with it the display.
*/
#define MAX_WALK 256

/* Define the mode in which the final color is calculated */
uniform int oirMode;

uvec4 fragments[MAX_FRAGMENTS];

/*
 * createFragmentList: collect the fragments of this pixel, head first.
 *
 * The whole chain is walked, but only MAX_FRAGMENTS entries are kept, and the
 * ones kept are the NEAREST rather than the most recently appended. Keeping
 * the newest would make the choice depend on draw order, so a thin primitive
 * such as a track could be dropped purely because many layers happened to be
 * appended after it. Sorting by depth instead means the fragments discarded
 * are the far ones, whose contribution is the most attenuated anyway.
 */
int createFragmentList(){
  int n = 0;
  //ivec2 coord = ivec2(gl_FragCoord.xy);
  //ivec2 sz = imageSize(head_pointer_image);
  //  if (coord.x >= sz.x || coord.y >= sz.y) return 0;

  int steps = 0;
  uint current = imageLoad(head_pointer_image, ivec2(gl_FragCoord.xy)).x;
  while (current != LIST_END && steps < MAX_WALK){
    steps++;
    if (current >= list_capacity) break;
    uvec4 item = imageLoad(list_buffer, int(current));
    current = item.x;
    if (n < MAX_FRAGMENTS){
      fragments[n] = item;
      n++;
    } else {
      /* full: let this fragment displace the farthest one held, if nearer */
      int far = 0;
      float fardepth = uintBitsToFloat(fragments[0].z);
      int i;
      for (i = 1; i < MAX_FRAGMENTS; i++){
        float d = uintBitsToFloat(fragments[i].z);
        if (d > fardepth){ fardepth = d; far = i; }
      }
      if (uintBitsToFloat(item.z) < fardepth) fragments[far] = item;
    }
  }
  return(n);
}

/*
 * sortFragments: farthest fragment first, so that the loop in finalColor()
 * can composite each nearer fragment over what is already accumulated
 */
void sortFragments(int n){
  int i, j;
  for (i=0; i<n-1; i++){
    for (j=0; j<n-1-i; j++){
      float depth_j  = uintBitsToFloat(fragments[j].z);
      float depth_j1 = uintBitsToFloat(fragments[j+1].z);
      if (depth_j < depth_j1){
        uvec4 tmp = fragments[j];
        fragments[j] = fragments[j+1];
        fragments[j+1] = tmp;
      }
    }
  }
}

/*
 * finalColor: composite the sorted fragments with the over operator.
 *
 * The accumulation is premultiplied, but the result is handed back with the
 * colour divided out again, so that the ordinary
 * GL_SRC_ALPHA / GL_ONE_MINUS_SRC_ALPHA blend puts it over the opaque image
 * correctly. This is the default mode.
 */
vec4 finalColor1(int nfrag){
  vec3 acc = vec3(0.0, 0.0, 0.0);
  float alpha = 0.0;
  int i;
  for (i=0; i<nfrag; i++){
    vec4 inCol = unpackUnorm4x8(fragments[i].y);
    acc   = acc   * (1.0 - inCol.a) + inCol.rgb * inCol.a;
    alpha = alpha * (1.0 - inCol.a) + inCol.a;
  }
  if (alpha <= 0.0) return vec4(0.0, 0.0, 0.0, 0.0);
  return vec4(acc / alpha, alpha);
}

/* Alternative approach: start from the front and blend in stuff which is behind
   scaling by a factor (e.g. 0.6) to enforce fragments which are further away
   contribute less and being darker */
vec4 finalColor2(int nfrag){
  vec3 acc = vec3(0.0, 0.0, 0.0);
  float alpha = 0.0;
  int i;
  vec4 inCol = unpackUnorm4x8(fragments[nfrag-1].y);
  acc = inCol.rgb;
  alpha = inCol.a;
  for (i=nfrag-1; i>=0; i--){
    vec4 inCol = unpackUnorm4x8(fragments[i].y);
    acc = acc * (1.0 - inCol.a) + inCol.rgb*inCol.a * 0.6;
    alpha = alpha * (1.0 - inCol.a) + inCol.a;
  }
  if (alpha <= 0.0) return vec4(0.0, 0.0, 0.0, 0.0);
  return vec4(acc / alpha, alpha);
}

/*
 * nearestDepth: depth of the transparent fragment closest to the viewer
 */
float nearestDepth(int nfrag){
  float d = 1.0;
  int i;
  for (i=0; i<nfrag; i++){
    d = min(d, uintBitsToFloat(fragments[i].z));
  }
  return d;
}

void main()
{
  int nFragments = createFragmentList();
  /* nothing transparent here, leave the opaque image alone */
  if (nFragments == 0) discard;
  sortFragments(nFragments);
  /*
    Hand the depth of the nearest transparent fragment to the depth test, so
    that the opaque image still occludes this pixel's transparent surfaces
    when it is in front of them. Without this the resolve would paint over
    opaque geometry that was drawn after the transparent surfaces had already
    been appended, for instance a banner drawn on top of the scene.
  */
  gl_FragDepth = nearestDepth(nFragments);
  switch (oirMode){
  case 1:
    gl_FragColor = finalColor1(nFragments);
    break;    
  case 2:
    gl_FragColor = finalColor2(nFragments);
    break;    
  default:
    gl_FragColor = finalColor1(nFragments);
    break;
  }
}
