#version 330 core
in vec2 TexCoord; // �������Զ�����ɫ������������
out vec4 FragColor;

uniform sampler2D boxTexture; // ���������
uniform sampler2D smileTexture; // �ڶ������������
void main()
{
	
	FragColor = mix(texture(boxTexture, TexCoord),texture(smileTexture,TexCoord),0.2); // ʹ�������������ȡ��ɫ
}