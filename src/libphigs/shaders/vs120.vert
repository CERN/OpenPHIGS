#version 120
uniform mat4 ModelViewMatrix;
uniform mat4 ProjectionMatrix;
attribute vec4 vColor;
varying vec4 Color;
varying vec4 Normal;
uniform int num_clip_planes;
uniform int clipping_ind;
uniform vec4 plane0;
uniform vec4 point0;
uniform vec4 plane1;
uniform vec4 point1;

void main()
{
  Color = vColor;
  Normal = normalize(ModelViewMatrix * vec4(gl_Normal, 1));
  gl_Position = ProjectionMatrix * ModelViewMatrix * gl_Vertex;

  if ((num_clip_planes >0 ) && (clipping_ind > 0)) {
    gl_ClipVertex = transpose(ModelViewMatrix) * gl_Vertex;
  };
};
