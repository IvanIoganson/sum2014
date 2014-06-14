/* FILE NAME   : a.frag
 * PURPOSE     : Simple fragment shader.
 * PROGRAMMER  : VG4.
 * LAST UPDATE : 10.06.2013
 */

#version 420

out vec4 OutColor;

uniform float Time;

uniform vec4 UseColor;
in vec4 DrawColor;
in vec4 DrawPos;

/* Main function */
void main( void )
{
  /*if (DrawPos.x > 3.75 + 5.1 * sin(Time * 3.0))
    discard;*/
  OutColor = DrawColor; //+ vec4(1, 1, 1, 1) * sin(Time);
} /* End of 'main' function */

/* End of 'a.frag' file */
