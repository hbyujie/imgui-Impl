project(applications)

include_directories(
		${CMAKE_CURRENT_SOURCE_DIR}
		${IMGUI_INCLUDE_DIRS}
		)
		
MESSAGE("----------${PROJECT_NAME}------------------------------------------------")
FILE(GLOB children *)
FOREACH (child ${children})
    IF (IS_DIRECTORY ${child})
        if (EXISTS "${child}/CMakeLists.txt")
            get_filename_component(f_base ${child} NAME_WE)
            MESSAGE(STATUS ${f_base})
            add_subdirectory(${child})
            SET_PROPERTY(TARGET ${f_base} PROPERTY FOLDER ${PROJECT_NAME})
        endif ()
    ENDIF ()
ENDFOREACH ()
MESSAGE("")