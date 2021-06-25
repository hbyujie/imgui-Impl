# imgui-openglwidget

cmakelist usage:
  1. vs 文件成组：
    a) source_group("folder_name" FILES ${files})
       多个分组并列
       
    b) 单独使用
        source_group(
        TREE ${CMAKE_CURRENT_SOURCE_DIR}
        PREFIX ""
        FILES ${CURRENT_SOURCE})
  2. vs 工程成组：
    a) 启用：SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)
    
    b) 设置：SET_PROPERTY(TARGET 工程名 PROPERTY FOLDER 成组名)
