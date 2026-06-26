#version 130
in vec4 vColor;
out vec4 Color;
out vec4 Normal;
out float gl_ClipDistance[6];
uniform mat4 ModelViewMatrix;
uniform mat4 ProjectionMatrix;
uniform int num_clip_planes;
uniform int clipping_ind;
uniform vec4 plane0;
uniform vec4 point0;
uniform vec4 plane1;
uniform vec4 point1;
float distance;

void main()
{
  Color = vColor;
  Normal = normalize(ModelViewMatrix * vec4(gl_Normal, 1));
  gl_Position = ProjectionMatrix * ModelViewMatrix * gl_Vertex;

  if (clipping_ind > 0) {
    if (num_clip_planes == 1) {
      gl_ClipDistance[0] = dot(gl_Vertex-point0, plane0);
    } else if (num_clip_planes == 2) {
      gl_ClipDistance[0] = dot(gl_Vertex-point0, plane0);
      gl_ClipDistance[1] = -dot(gl_Vertex-point1, plane1);
    } else {
      gl_ClipDistance[0] = 1.0;
      gl_ClipDistance[1] = 1.0;
    }
  } else {
    gl_ClipDistance[0] = 1.0;
    gl_ClipDistance[1] = 1.0;
  }
}
