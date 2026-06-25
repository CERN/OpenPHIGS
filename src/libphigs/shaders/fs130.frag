#version 130
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

in vec4 Color;
in vec4 Normal;
out vec4 FragColor;

vec4 getLight(int type, vec4 color, vec4 pos, vec4 coef){
  vec4 light;
  float refl = 0.0;
  float angle = Normal.x*pos.x+Normal.y*pos.y+Normal.z*pos.z;
  float lennorm = sqrt(Normal.x*Normal.x+Normal.y*Normal.y+Normal.z*Normal.z);
  float lenpos = sqrt(pos.x*pos.x+pos.y*pos.y+pos.z*pos.z);
  float atot = sqrt(vAmbient.x*vAmbient.x+vAmbient.y*vAmbient.y+vAmbient.z*vAmbient.z+vAmbient.w*vAmbient.w);
  float dtot = sqrt(vDiffuse.x*vDiffuse.x+vDiffuse.y*vDiffuse.y+vDiffuse.z*vDiffuse.z+vDiffuse.w*vDiffuse.w);
  float stot = sqrt(vSpecular.x*vSpecular.x+vSpecular.y*vSpecular.y+vSpecular.z*vSpecular.z+vSpecular.w*vSpecular.w);
  float tot = atot+dtot+stot;
  if (tot <= 0) tot = 0.2;
  if (lennorm == 0.0) lennorm = 1.0;
  if (lenpos == 0.0) lenpos = 1.0;
  angle = max(angle/lennorm/lenpos, 0.0);
  switch (type) {
  case 1:
    light = color * vAmbient * vec4(1.0/tot, 1.0/tot, 1.0/tot, 1.0);
    break;
  case 2:
    light = color * vDiffuse * angle * vec4(1.0/tot, 1.0/tot, 1.0/tot, 1.0);;
    break;
  case 3:
    refl = coef.x * pow(angle, coef.y);
    light = vSpecular * vec4(refl/tot, refl/tot, refl/tot, 1.0);
    break;
  default:
    light = vec4(0.5, 0.5, 0.5, 1.0);
    break;
  };
  return light;
}
void main()
{
  int i;
  if (ShadingMode > 0) {
    int n = 0;
    FragColor = vec4(0., 0., 0, 1.);
    for (i=0; i<7; i++){
      switch (i) {
      case 0:
        if (lightSource0 > 0){ FragColor += getLight(lightSourceTyp0, lightSourceCol0, lightSourcePos0, lightSourceCoef0);n += 1;};
        break;
      case 1:
        if (lightSource1 > 0){ FragColor += getLight(lightSourceTyp1, lightSourceCol1, lightSourcePos1, lightSourceCoef1);n += 1;};
        break;
      case 2:
        if (lightSource2 > 0){ FragColor += getLight(lightSourceTyp2, lightSourceCol2, lightSourcePos2, lightSourceCoef2);n += 1;};
        break;
      case 3:
        if (lightSource3 > 0){ FragColor += getLight(lightSourceTyp3, lightSourceCol3, lightSourcePos3, lightSourceCoef3);n += 1;};
        break;
      case 4:
        if (lightSource4 > 0){ FragColor += getLight(lightSourceTyp4, lightSourceCol4, lightSourcePos4, lightSourceCoef4);n += 1;};
        break;
      case 5:
        if (lightSource5 > 0){ FragColor += getLight(lightSourceTyp5, lightSourceCol5, lightSourcePos5, lightSourceCoef5);n += 1;};
        break;
      case 6:
        if (lightSource6 > 0){ FragColor += getLight(lightSourceTyp6, lightSourceCol6, lightSourcePos6, lightSourceCoef6);n += 1;};
        break;
      default:
        break;
      };
    };
    if (n > 0){
      FragColor = min(FragColor, vec4(1., 1., 1., 1.));
    } else { FragColor = Color;};
  } else {
    FragColor = Color;
  };
};
