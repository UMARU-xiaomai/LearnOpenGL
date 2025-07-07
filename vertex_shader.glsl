#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord; // 输出纹理坐标到片段着色器

uniform mat4 transform; // 变换矩阵
void main()
{
   gl_Position = transform * vec4(aPos.x, aPos.y, aPos.z, 1.0);// 将顶点位置传递给片段着色器
   TexCoord = aTexCoord; // 将纹理坐标传递给片段着色器
}