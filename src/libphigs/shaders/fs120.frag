#version 120
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
varying vec4 Normal;
varying vec4 Color;
varying float v_clipDist0;
varying float v_clipDist1;
varying vec4 VertexPosEye;

/*
 * getLight: returns the RGB contribution of a single light source.
 *
 * Fix 1 (darkness): removed the global "/tot" normalization that divided
 *   every light's contribution by the combined magnitude of vAmbient +
 *   vDiffuse + vSpecular. That division could easily exceed 4.0 for
 *   ordinary material colors, crushing brightness well below intended
 *   levels, especially with few lights active. Standard ambient+diffuse+
 *   specular summation does not require this step; the final min(...,1.0)
 *   clamp in main() already guards against over-brightening when many
 *   lights are summed.
 *
 * Fix 2 (alpha): alpha is no longer computed here at all. Previously each
 *   light contributed its own alpha value (vAmbient.w, vDiffuse.w, or a
 *   hardcoded 1.0 for specular), and main() summed these across every
 *   active light, so final alpha depended on how many lights were on
 *   rather than on the material's actual transparency. Alpha is now
 *   handled once in main(), from Color.a.
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
    refl = coef.x * pow(angle, coef.y);
    light = vSpecular.rgb * refl;
  };
  return vec4(light, 0.0);
}

void main()
{
  int i;
  /* clipping */
  if (v_clipDist0 < 0.0 || v_clipDist1 < 0.0) discard;
  if (ShadingMode > 0) {
    int n = 0;
    gl_FragColor = vec4(0., 0., 0., 0.);
    for (i=0; i<7; i++){
      if (i==0) {
        if (lightSource0 > 0){ gl_FragColor += getLight(lightSourceTyp0, lightSourceCol0, lightSourcePos0, lightSourceCoef0);n += 1;};
      }
      if (i==1) {
        if (lightSource1 > 0){ gl_FragColor += getLight(lightSourceTyp1, lightSourceCol1, lightSourcePos1, lightSourceCoef1);n += 1;};
      }
      if (i==2) {
        if (lightSource2 > 0){ gl_FragColor += getLight(lightSourceTyp2, lightSourceCol2, lightSourcePos2, lightSourceCoef2);n += 1;};
      }
      if (i==3) {
        if (lightSource3 > 0){ gl_FragColor += getLight(lightSourceTyp3, lightSourceCol3, lightSourcePos3, lightSourceCoef3);n += 1;};
      }
      if (i==4) {
        if (lightSource4 > 0){ gl_FragColor += getLight(lightSourceTyp4, lightSourceCol4, lightSourcePos4, lightSourceCoef4);n += 1;};
      }
      if (i==5) {
        if (lightSource5 > 0){ gl_FragColor += getLight(lightSourceTyp5, lightSourceCol5, lightSourcePos5, lightSourceCoef5);n += 1;};
      }
      if (i==6) {
        if (lightSource6 > 0){ gl_FragColor += getLight(lightSourceTyp6, lightSourceCol6, lightSourcePos6, lightSourceCoef6);n += 1;};
      }
    };
    if (n > 0){
      gl_FragColor.rgb = min(gl_FragColor.rgb, vec3(1., 1., 1.));
      gl_FragColor.a = Color.a;
    } else { gl_FragColor = Color;};
  } else {
    gl_FragColor = Color;
  };
};
