#version 120
uniform mat4 ModelViewMatrix;
uniform mat4 ProjectionMatrix;
attribute vec4 vColor;
varying vec4 Color;
varying vec4 Normal;
varying vec4 VertexPosEye;

void main()
{
  VertexPosEye = gl_ModelViewMatrix * gl_Vertex;
  Color = vColor;
  Normal = normalize(ModelViewMatrix * vec4(gl_Normal, 1));
  gl_Position = ProjectionMatrix * ModelViewMatrix * gl_Vertex;
};
