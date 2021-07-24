project(renderer)

add_definitions(-DIMGUI_IMPL_OPENGL_LOADER_GLEW)

############### OpenGL ###############
find_package(OpenGL REQUIRED)
#message(STATUS "--Found OpenGL--")
#message(STATUS OPENGL_gl_LIBRARY: ${OPENGL_gl_LIBRARY})

############### Qt5    ###############
find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)
#message(STATUS "--Found Qt5--")
#message(STATUS Qt5_DIR: ${Qt5_DIR})

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

include_directories(
		${CMAKE_CURRENT_SOURCE_DIR}/include
		${CMAKE_CURRENT_SOURCE_DIR}/include/imgui
		${CMAKE_SOURCE_DIR}/3rdparty/glew/include
		${CMAKE_SOURCE_DIR}/3rdparty/glm
		${Qt5_DIR}
)

link_directories(
		${LIBRARY_OUTPUT_PATH}
		${CMAKE_SOURCE_DIR}/3rdparty/glew/lib
)

file(GLOB_RECURSE includes
		"${CMAKE_CURRENT_SOURCE_DIR}/include/*.h*"
		)
source_group("include" FILES ${includes})

file(GLOB_RECURSE srcs
		"${CMAKE_CURRENT_SOURCE_DIR}/src/*.c*"
		)
source_group("src" FILES ${srcs})

add_library(${PROJECT_NAME} STATIC ${includes} ${srcs})
target_link_libraries(${PROJECT_NAME}
		glew32
		${OPENGL_gl_LIBRARY}
		Qt5::Core Qt5::Gui Qt5::Widgets
		imgui
		)