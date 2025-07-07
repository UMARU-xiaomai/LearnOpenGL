#version 330 core
in vec2 TexCoord; // 接收来自顶点着色器的纹理坐标
out vec4 FragColor;

uniform sampler2D boxTexture; // 纹理采样器
uniform sampler2D smileTexture; // 第二个纹理采样器
void main()
{
	
	FragColor = mix(texture(boxTexture, TexCoord),texture(smileTexture,TexCoord),0.2); // 使用纹理采样器获取颜色
}