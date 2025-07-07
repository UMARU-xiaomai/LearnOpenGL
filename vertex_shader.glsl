#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord; // ����������굽Ƭ����ɫ��

uniform mat4 transform; // �任����
void main()
{
   gl_Position = transform * vec4(aPos.x, aPos.y, aPos.z, 1.0);// ������λ�ô��ݸ�Ƭ����ɫ��
   TexCoord = aTexCoord; // ���������괫�ݸ�Ƭ����ɫ��
}