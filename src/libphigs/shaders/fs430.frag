#version 430 compatibility
/*
 * Order independent rendering.
 *
 * The fragment is shaded as in fs120, then appended to the linked list of
 * fragments belonging to its pixel. The list is walked, sorted by depth and
 * blended at the bottom of main().
 *
 * This is pass 1 of 2. It shades the fragment and then, whenever OIR is
 * enabled, appends it to the linked list of its pixel and discards --
 * opaque and transparent fragments alike (see appendFragment() below; commit
 * 71768f4 "Treat all fragments in the same way" removed the earlier
 * col.a>=1.0 fast path that let opaque fragments write straight to the
 * framebuffer and skip the list).
 *
 * fs430_resolve.frag is pass 2: it walks each pixel's list, sorts it by depth
 * and composites the result over the opaque image already written by
 * ordinary (non-OIR) geometry, or left over from before this frame.
 *
 * TRADE-OFF: routing opaque fragments through the list too is what makes an
 * opaque object sandwiched between two transparent surfaces at different
 * depths composite correctly (e.g. a detector track behind the near face of
 * a transparent shell but in front of its far face) -- a single depth test
 * against the real depth buffer cannot show the near face, hide the track
 * behind it, and still let the far face shine through from behind, all at
 * once; having opaque fragments in the list lets sortFragments()/
 * finalColor*() in the resolve pass get it right. The cost is that opaque
 * geometry now also competes for the same per-pixel MAX_FRAGMENTS(16) window
 * (see fs430_resolve.frag) and the shared, global list_capacity budget
 * (wsgl_oir_reset()/appendFragment() below), instead of just the transparent
 * surfaces. A busy/dense scene -- especially at a viewing angle that packs
 * many opaque primitives onto the same pixels, e.g. a detector wireframe
 * viewed nearly along an axis of symmetry -- can now exhaust either budget
 * where it could not before: appendFragment() returning false makes that one
 * fragment fall back to an unsorted, immediate framebuffer write instead of
 * going through the resolve pass, and a per-pixel list longer than
 * MAX_FRAGMENTS makes createFragmentList() evict its farthest entries. If
 * fills or overlays flicker or show holes in busy/dense scenes, especially
 * ones that change with the viewing angle, this capacity pressure is the
 * first place to look (check whether wsgl_oir_reset()'s overflow warning
 * fires on stderr for that frame). If detector views with tracks behind
 * transparent surfaces start looking wrong instead, the opaque/transparent
 * split this trade-off replaced is the second place to look.
 */
uniform int ShadingMode;
uniform vec4 vAmbient;
uniform vec4 vDiffuse;
uniform vec4 vSpecular;
uniform int lightSource0;
uniform int lightSourceTyp0;
uniform vec4 lightSourceCol0;
uniform vec4 lightSourcePos0;
uniform vec4 lightSourceCoef0;
uniform int lightSource1;
uniform int lightSourceTyp1;
uniform vec4 lightSourceCol1;
uniform vec4 lightSourcePos1;
uniform vec4 lightSourceCoef1;
uniform int lightSource2;
uniform int lightSourceTyp2;
uniform vec4 lightSourceCol2;
uniform vec4 lightSourcePos2;
uniform vec4 lightSourceCoef2;
uniform int lightSource3;
uniform int lightSourceTyp3;
uniform vec4 lightSourceCol3;
uniform vec4 lightSourcePos3;
uniform vec4 lightSourceCoef3;
uniform int lightSource4;
uniform int lightSourceTyp4;
uniform vec4 lightSourceCol4;
uniform vec4 lightSourcePos4;
uniform vec4 lightSourceCoef4;
uniform int lightSource5;
uniform int lightSourceTyp5;
uniform vec4 lightSourceCol5;
uniform vec4 lightSourcePos5;
uniform vec4 lightSourceCoef5;
uniform int lightSource6;
uniform int lightSourceTyp6;
uniform vec4 lightSourceCol6;
uniform vec4 lightSourcePos6;
uniform vec4 lightSourceCoef6;
uniform sampler2D currentTexture;
uniform int applyTexture;
/* number of entries the fragment list can hold, set by wsgl_oir_reset() */
uniform uint list_capacity;
uniform int oirEnable;
/* width of the canvas, so gl_FragCoord can be turned into a head pointer index */
uniform uint oirWidth;

in vec4 Normal;
in vec4 Color;
in vec4 VertexPosEye;
in vec2 TexCoord;

/*
 * Order independent rendering state.
 *
 * The head pointer is a shader storage buffer of one uint per pixel, indexed
 * as y * oirWidth + x, rather than a uimage2D: at least one NVIDIA driver
 * (580.178.04) does not reliably make a uimage2D's contents visible to
 * imageLoad() in a separately linked program (confirmed with
 * tools/oir_repro.c in the OpenPHIGS repository), even though the equivalent
 * SSBO does not show the problem. The fragment list is unaffected by that and
 * stays a uimageBuffer, read back with imageLoad through the one binding
 * below.
 *
 * head_pointers is bound to binding point 0 (GL_SHADER_STORAGE_BUFFER) by
 * wsgl_oir_reset(), list_buffer to image unit 1.
 */
/*
 * NOTE: early_fragment_tests must NOT be used here. It moves the depth test
 * and, crucially, the depth WRITE in front of the shader, so the discard
 * below can no longer suppress the write. A transparent surface would then
 * leave its depth behind, and any opaque primitive behind it would be
 * rejected by the depth test before this shader ever ran, which is to say
 * before it could be drawn or appended. Leaving the tests late costs a
 * little work on hidden fragments and keeps transparency correct.
 */
layout (binding = 0, offset = 0) uniform atomic_uint index_counter;
layout (std430, binding = 0)     coherent buffer HeadPointers { uint head_pointers[]; };
layout (binding = 1, rgba32ui)   coherent uniform uimageBuffer list_buffer;

/*
 * getLight: returns the RGB contribution of a single light source.
 *
 */
vec4 getLight(int type, vec4 color, vec4 pos, vec4 coef){
  vec3 light = vec3(0.5, 0.5, 0.5);
  float refl = 0.0;
  float angle = Normal.x*pos.x+Normal.y*pos.y+Normal.z*pos.z;
  float lennorm = sqrt(Normal.x*Normal.x+Normal.y*Normal.y+Normal.z*Normal.z);
  float lenpos = sqrt(pos.x*pos.x+pos.y*pos.y+pos.z*pos.z);
  if (lennorm == 0.0) lennorm = 1.0;
  if (lenpos == 0.0) lenpos = 1.0;
  angle = max(angle/lennorm/lenpos, 0.0);

  if (type == 1) {
    /* ambient: flat contribution, independent of angle */
    light = color.rgb * vAmbient.rgb;
  };
  if (type == 2) {
    /* diffuse: falls off with angle between normal and light direction */
    light = color.rgb * vDiffuse.rgb * angle;
  };
  if (type == 3) {
    vec3 V = normalize(-VertexPosEye.xyz);     // view direction, eye space
    vec3 L = normalize(pos.xyz);               // light direction (already what you compute angle from)
    vec3 N = normalize(Normal.xyz);
    vec3 R = reflect(-L, N);
    float specAngle = max(dot(R, V), 0.0);
    refl = coef.x * pow(specAngle, coef.y);
    light = vSpecular.rgb * refl;
  };
  return vec4(light, 0.0);
}

vec4 applyLight(vec4 inColor){
  int i;
  vec4 outColor = inColor;
  if (ShadingMode > 0) {
    int n = 0;
    outColor = vec4(0., 0., 0., 0.);
    for (i=0; i<7; i++){
      if (i==0) {
        if (lightSource0 > 0){ outColor += getLight(lightSourceTyp0, lightSourceCol0, lightSourcePos0, lightSourceCoef0);n += 1;};
      }
      if (i==1) {
        if (lightSource1 > 0){ outColor += getLight(lightSourceTyp1, lightSourceCol1, lightSourcePos1, lightSourceCoef1);n += 1;};
      }
      if (i==2) {
        if (lightSource2 > 0){ outColor += getLight(lightSourceTyp2, lightSourceCol2, lightSourcePos2, lightSourceCoef2);n += 1;};
      }
      if (i==3) {
        if (lightSource3 > 0){ outColor += getLight(lightSourceTyp3, lightSourceCol3, lightSourcePos3, lightSourceCoef3);n += 1;};
      }
      if (i==4) {
        if (lightSource4 > 0){ outColor += getLight(lightSourceTyp4, lightSourceCol4, lightSourcePos4, lightSourceCoef4);n += 1;};
      }
      if (i==5) {
        if (lightSource5 > 0){ outColor += getLight(lightSourceTyp5, lightSourceCol5, lightSourcePos5, lightSourceCoef5);n += 1;};
      }
      if (i==6) {
        if (lightSource6 > 0){ outColor += getLight(lightSourceTyp6, lightSourceCol6, lightSourcePos6, lightSourceCoef6);n += 1;};
      }
    };
    if (n > 0){
      outColor.rgb = min(outColor.rgb, vec3(1., 1., 1.));
      outColor.a = inColor.a;
    } else { outColor = inColor;};
  }
  return(outColor);
}

/* named applyTexturing, because applyTexture is already a uniform above */
vec4 applyTexturing(vec4 inColor){
  vec4 outColor = inColor;
  if (applyTexture != 0){
    vec4 texColor = texture(currentTexture, TexCoord);
    outColor = texColor*inColor;
  }
  return(outColor);
}

vec4 fragColor(vec4 inColor){
  // Apply lighting and textures to incoming color
  return(applyTexturing(applyLight(inColor)));
}

/*
 * appendFragment: push one shaded fragment onto the list of its pixel.
 * Returns false when the list is full, in which case nothing is stored and
 * the head pointer is left alone. Without that check the store would go out
 * of range and the head would be made to point at an entry that does not
 * exist, which corrupts the lists of unrelated pixels.
 * We store the index in the 4th component, which is used later on for proper sorting
 * of the list.
 */
bool appendFragment(vec4 fragCol){
  uint index = atomicCounterIncrement(index_counter);
  if (index >= list_capacity) return false;
  uint headIndex = uint(gl_FragCoord.y) * oirWidth + uint(gl_FragCoord.x);
  uint old_head = atomicExchange(head_pointers[headIndex], index);
  uvec4 item;
  item.x = old_head;
  item.y = packUnorm4x8(fragCol);
  item.z = floatBitsToUint(gl_FragCoord.z);
  item.w = index;
  imageStore(list_buffer, int(index), item);
  return true;
}

void main()
{
  vec4 col = fragColor(Color);
  /*
   * All fragments go through the list when OIR is enabled, opaque and
   * transparent alike -- see the TRADE-OFF note at the top of this file for
   * why, and for the capacity cost that comes with it.
   */
  if (oirEnable == 0 ){
    gl_FragColor = col;    /* OIR disabled, or opaque: straight out */
  } else {
    if (!appendFragment(col)) {
      gl_FragColor = col;    /* no room in the list: draw it, unsorted */
      return;
    }
    discard;
  }
}
