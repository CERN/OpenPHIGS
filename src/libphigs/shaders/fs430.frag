#version 430 compatibility
/*
 * Order independent rendering.
 *
 * The fragment is shaded as in fs120, then appended to the linked list of
 * fragments belonging to its pixel. The list is walked, sorted by depth and
 * blended at the bottom of main().
 *
 * This is pass 1 of 2. It shades the fragment and then either
 *
 *   - writes it straight out, if it is opaque. Opaque geometry cannot have
 *     anything show through it, so it does not belong in the fragment list,
 *     and keeping it out is what leaves room in the list for the surfaces
 *     that do need it. It writes depth as usual, so opaque primitives such
 *     as tracks stay visible behind transparent surfaces.
 *
 *   - or appends it to the linked list of its pixel and discards, if it is
 *     transparent. Nothing is written to the framebuffer in that case.
 *
 * fs430_resolve.frag is pass 2: it walks each pixel's list, sorts it by depth
 * and composites the result over the opaque image left behind by this pass.
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

in vec4 Normal;
in vec4 Color;
in vec4 VertexPosEye;
in vec2 TexCoord;

/*
 * Order independent rendering state. The head pointer image and the fragment
 * list are read back with imageLoad rather than through a second pair of
 * sampler uniforms, so each object needs only the one binding below.
 *
 * head_pointer_image is bound to image unit 0 by wsgl_oir_reset(), the
 * fragment list still needs a texture and a binding to unit 1 on the C side.
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
/* number of entries the fragment list can hold, set by wsgl_oir_reset() */
layout (binding = 0, r32ui)      coherent uniform uimage2D     head_pointer_image;
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
 */
bool appendFragment(vec4 fragCol){
  uint index = atomicCounterIncrement(index_counter);
  if (index >= list_capacity) return false;
  uint old_head = imageAtomicExchange(head_pointer_image,
                                      ivec2(gl_FragCoord.xy),
                                      index);
  uvec4 item;
  item.x = old_head;
  item.y = packUnorm4x8(fragCol);
  item.z = floatBitsToUint(gl_FragCoord.z);
  item.w = 0u;
  imageStore(list_buffer, int(index), item);
  return true;
}

void main()
{
  vec4 col = fragColor(Color);
  if (oirEnable == 0){
    gl_FragColor = col;    /* OIR disabled, straight out */
  } else {
    /* Everything goes here */
    if (!appendFragment(col)) {
      gl_FragColor = col;    /* no room in the list: draw it, unsorted */
      return;
    }
    discard;
  }
}
