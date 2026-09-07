#version 430 compatibility
/*
 * Order independent rendering, pass 1 of 2: geometry.
 *
 * This stage is unchanged with respect to vs120: it only transforms the
 * geometry and hands the interpolants to the fragment stage. The per pixel
 * fragment list is built in fs420.frag, because the list is keyed by
 * gl_FragCoord, which does not exist here.
 */
layout(location=8)  uniform mat4 ModelViewMatrix;
layout(location=9)  uniform mat4 ProjectionMatrix;
layout(location=10) uniform vec4 sPlane;
layout(location=11) uniform vec4 tPlane;
in vec4 vColor;

out vec4 Color;
out vec4 Normal;
out vec4 VertexPosEye;
out vec2 TexCoord;

void main()
{
  VertexPosEye = gl_ModelViewMatrix * gl_Vertex;
  gl_ClipVertex = VertexPosEye;
  Color = vColor;
  // should not use w component here
  // Normal = normalize(ModelViewMatrix * vec4(gl_Normal, 1));
  Normal = vec4(normalize(mat3(ModelViewMatrix) * gl_Normal), 0.0);
  gl_Position = ProjectionMatrix * ModelViewMatrix * gl_Vertex;
  TexCoord = vec2(dot(gl_Vertex, sPlane), dot(gl_Vertex, tPlane));
}
