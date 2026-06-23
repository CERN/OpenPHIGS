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

varying float v_clipDist0;
varying float v_clipDist1;
varying vec4 VertexPosEye;

void main()
{
  VertexPosEye = gl_ModelViewMatrix * gl_Vertex;
  Color = vColor;
  Normal = normalize(ModelViewMatrix * vec4(gl_Normal, 1));
  gl_Position = ProjectionMatrix * ModelViewMatrix * gl_Vertex;

  if (clipping_ind > 0) {
    if (num_clip_planes == 1) {
      v_clipDist0 = dot(gl_Vertex - point0, plane0);
      v_clipDist1 = 1.0;
    } else if (num_clip_planes == 2) {
      v_clipDist0 = dot(gl_Vertex - point0, plane0);
      v_clipDist1 = dot(gl_Vertex - point1, plane1);
    } else {
      v_clipDist0 = 1.0;
      v_clipDist1 = 1.0;
    }
  } else {
    v_clipDist0 = 1.0;
    v_clipDist1 = 1.0;
  }
};
