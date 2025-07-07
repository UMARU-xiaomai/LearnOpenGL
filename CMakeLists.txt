# CMakeList.txt: OpenGL_test1 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

# 如果支持，请为 MSVC 编译器启用热重载。
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("OpenGL_test1")

include_directories (
	SYSTEM "${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/include"
)

link_directories (
	SYSTEM "${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/lib"
)
# 将源代码添加到此项目的可执行文件。
add_executable (OpenGL_test1 "OpenGL_test1.cpp" "OpenGL_test1.h" "glad.c" "MyShader.h" "MyShader.cpp" "stbi.cpp")

target_link_libraries(OpenGL_test1 glfw3.lib)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET OpenGL_test1 PROPERTY CXX_STANDARD 20)
endif()

# TODO: 如有需要，请添加测试并安装目标。
