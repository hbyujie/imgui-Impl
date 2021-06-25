project(imgui-openglwidget3)

message("Project - imgui-openglwidget3")

add_definitions(-DIMGUI_IMPL_OPENGL_LOADER_GLEW)

############### GLEW ###############
set(GLEW_DIR ${CMAKE_SOURCE_DIR}/3rdparty/glew)

############### OpenGL ###############
find_package(OpenGL REQUIRED)
message(STATUS "--Found OpenGL--")
message(STATUS OPENGL_gl_LIBRARY: ${OPENGL_gl_LIBRARY})

############### Qt5    ###############
find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)
message(STATUS "--Found Qt5--")
message(STATUS Qt5_DIR: ${Qt5_DIR})

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

include_directories(
		${CMAKE_CURRENT_SOURCE_DIR}
		${GLEW_DIR}/include
		${Qt5_DIR}
)

link_directories(
		${LIBRARY_OUTPUT_PATH}
		${GLEW_DIR}/lib
)

file(GLOB_RECURSE CURRENT_SOURCE
		"${CMAKE_CURRENT_SOURCE_DIR}/*.c*"
		"${CMAKE_CURRENT_SOURCE_DIR}/*.h*"
		)

source_group("src" FILES ${CURRENT_SOURCE})

set(imgui_impl_opengl
	${IMGUI_INCLUDE_DIRS}/backends/imgui_impl_opengl3.h
	${IMGUI_INCLUDE_DIRS}/backends/imgui_impl_opengl3.cpp
	${CMAKE_SOURCE_DIR}/applications/common/imgui_impl_openglwidget.h
	${CMAKE_SOURCE_DIR}/applications/common/imgui_impl_openglwidget.cpp
	)

source_group("backends" FILES ${imgui_impl_opengl})

add_executable(${PROJECT_NAME} ${CURRENT_SOURCE} ${imgui_impl_opengl})
target_link_libraries(${PROJECT_NAME}
		glew32
		${OPENGL_gl_LIBRARY}
		Qt5::Core Qt5::Gui Qt5::Widgets
		imgui
		)