cmake_minimum_required(VERSION 3.12)

project(imgui-openglwidget)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

############### IMGUI ###############
set(IMGUI_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/submodule/imgui)
set(IMGUI_SOURCE
		${IMGUI_INCLUDE_DIRS}/imconfig.h
		${IMGUI_INCLUDE_DIRS}/imgui.cpp
		${IMGUI_INCLUDE_DIRS}/imgui.h
		${IMGUI_INCLUDE_DIRS}/imgui_demo.cpp
		${IMGUI_INCLUDE_DIRS}/imgui_draw.cpp
		${IMGUI_INCLUDE_DIRS}/imgui_internal.h
		${IMGUI_INCLUDE_DIRS}/imgui_widgets.cpp
		${IMGUI_INCLUDE_DIRS}/imgui_tables.cpp
		${IMGUI_INCLUDE_DIRS}/imstb_rectpack.h
		${IMGUI_INCLUDE_DIRS}/imstb_textedit.h
		${IMGUI_INCLUDE_DIRS}/imstb_truetype.h
		)
add_library(imgui STATIC ${IMGUI_SOURCE})
SET_PROPERTY(TARGET imgui PROPERTY FOLDER submodule)

add_subdirectory(applications)