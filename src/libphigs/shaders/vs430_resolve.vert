#version 430 compatibility
/*
 * Order independent rendering, pass 2 of 2: vertex stage.
 *
 * Pass 2 draws a single quad covering the whole viewport, so that the
 * fragment stage gets exactly one invocation per pixel. The quad is handed
 * over in clip coordinates already, which is why nothing is transformed here.
 */
void main()
{
  gl_Position = gl_Vertex;
}
