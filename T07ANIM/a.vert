/* FILE NAME   : a.vert
 * PURPOSE     : Simple vertex shader.
 * PROGRAMMER  : VG4.
 * LAST UPDATE : 13.06.2014
 */

#version 420

layout(location = 0) in vec3 InPosition;
layout(location = 1) in vec2 InTexCoord;
layout(location = 2) in vec3 InNormal;
layout(location = 3) in vec4 InColor;

uniform vec4 UseColor;
uniform mat4 MatrWVP;
uniform mat4 MatrWorldInverseTranspose;
uniform mat4 MatrWorld;
uniform mat4 MatrView;

uniform float Time;

out vec4 DrawColor;
out vec3 DrawPos;
out vec2 DrawTexCoord;
out vec3 DrawNormal;
out vec4 CameraPos;

/* Main function */
void main( void )
{
  CameraPos = (MatrWorld * MatrView) * vec4(InPosition.xyz, 1);
  gl_Position = MatrWVP * vec4(InPosition.xyz, 1);
  DrawNormal = mat3(MatrWorldInverseTranspose) * InNormal;
  DrawColor = InColor;
  DrawPos = mat4x3(MatrWorld) * vec4(InPosition, 1);
  DrawTexCoord = InTexCoord.st;
} /* End of 'main' function */

/* End of 'a.vert' file */
