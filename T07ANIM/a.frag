/* FILE NAME   : a.frag
 * PURPOSE     : Simple fragment shader.
 * PROGRAMMER  : VG4.
 * LAST UPDATE : 13.06.2014
 */

#version 420

/* �������� �������� ����� ��������� */
out vec4 OutColor;

/* ���������� ������ */
uniform float Time;
uniform vec4 LightPosition;
uniform vec4 LightColor;
uniform vec3 ViewDir;

/* �������� */
uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float Phong;
uniform float Trans;

uniform vec4 UseColor;
uniform mat4 MatrWVP;
uniform mat4 MatrWorldInverseTranspose;
uniform mat4 MatrWorld;

/* �������� */
uniform sampler2D DrawTexture;

/* ������� ������ */
in vec4 DrawColor;
in vec3 DrawPos;
in vec2 DrawTexCoord;
in vec3 DrawNormal;
in vec4 CameraPos;

vec3 Illum( vec3 N )
{
  vec4 texc = texture2D(DrawTexture, DrawTexCoord.xy);
  vec3 color = Ka;
  vec3 Dir = mat3(MatrWorld) * ViewDir; 

  vec3 lPos = vec3(100, 130, 10);
  vec3 l = normalize(lPos - DrawPos);

  N = abs(faceforward(N, ViewDir, N));
  float nl = dot(N, l);
  if (nl > 0)
    color += (texc.xyz * 1 * Kd + vec3(0.7, 0.7, 0.7)) * abs(nl);

  vec3 R = reflect(Dir, N);
  R = Dir - N * (2 * dot(Dir, N));
  float rl = dot(R, l);
  if (rl > 0)
    color += Ks * pow(dot(R, l), 14);

  return abs(color);
}


/* Main function */
void main( void )
{
  float start = 3, end = -3;
  float dist = CameraPos.z;
  /*if (dist < end)
    discard;*/
  float t = 1;
  /*if (dist > start)
    t = 1;
  else
    if (dist < end)
      t = 0;
    else
      t = 1 - (dist - start) / (end - start);*/
  OutColor = vec4(0.3, 0.5, 0.7, 1) * (1 - t) + vec4(Illum(DrawNormal.xyz), Trans) * t;
} /* End of 'main' function */

/* End of 'a.frag' file */
