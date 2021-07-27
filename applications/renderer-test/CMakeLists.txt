project(renderer-test)

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
		${CMAKE_CURRENT_SOURCE_DIR}
		${CMAKE_SOURCE_DIR}/applications/renderer/include
		${CMAKE_SOURCE_DIR}/3rdparty/assimp/include
		${CMAKE_SOURCE_DIR}/3rdparty/glew/include		
		${CMAKE_SOURCE_DIR}/3rdparty/glm
		${Qt5_DIR}
)

link_directories(
		${LIBRARY_OUTPUT_PATH}
		${CMAKE_SOURCE_DIR}/3rdparty/assimp/lib
		${CMAKE_SOURCE_DIR}/3rdparty/glew/lib
)

file(GLOB_RECURSE CURRENT_SOURCE
		"${CMAKE_CURRENT_SOURCE_DIR}/*.c*"
		"${CMAKE_CURRENT_SOURCE_DIR}/*.h*"
		)
source_group("src" FILES ${CURRENT_SOURCE})

set(qrc "${CMAKE_CURRENT_SOURCE_DIR}/res/res.qrc")

add_executable(${PROJECT_NAME} ${CURRENT_SOURCE} ${qrc})
target_link_libraries(${PROJECT_NAME}
		assimp-vc140-mt
		glew32
		${OPENGL_gl_LIBRARY}
		Qt5::Core Qt5::Gui Qt5::Widgets
		imgui
		renderer
		)