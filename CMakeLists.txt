cmake_minimum_required(VERSION 3.10)

project("glfw-imgui" VERSION 0.9)
message("Project name: " ${CMAKE_PROJECT_NAME})

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)

if (UNIX)
    if (NOT APPLE)
        find_package(Threads REQUIRED)
        find_package(X11 REQUIRED)
    endif()
endif()

if (WIN32)
    SET (PROGRAMS-PREFIX "c:/programs")
else()
    SET (PROGRAMS-PREFIX "~/programs")
endif()
add_library("glad" "${PROGRAMS-PREFIX}/glad/src/glad.c")
find_library(GLFW glfw3 PATHS "${PROGRAMS-PREFIX}/glfw/lib")

include_directories(
    "${PROGRAMS-PREFIX}/glad/include"
    "${PROGRAMS-PREFIX}/glfw/include"
    )

set(sources
    main.cpp
    functions.cpp
    imgui/imconfig.h
    imgui/imgui_demo.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_impl_glfw.cpp
    imgui/imgui_impl_glfw.h
    imgui/imgui_impl_opengl2.cpp
    imgui/imgui_impl_opengl2.h
    imgui/imgui_internal.h
    imgui/imgui_widgets.cpp
    imgui/imgui.cpp
    imgui/imgui.h
    imgui/imstb_rectpack.h
    imgui/imstb_textedit.h
    imgui/imstb_truetype.h
    )

add_executable(${CMAKE_PROJECT_NAME} ${sources})

target_link_libraries(${CMAKE_PROJECT_NAME}
    ${OPENGL_gl_LIBRARY}
    "glad"
    ${GLFW}
    )
if (UNIX)
    if (APPLE)
        target_link_libraries(${CMAKE_PROJECT_NAME}
            "-framework Cocoa"
            "-framework IOKit"
            "-framework CoreVideo"
            )
    else()
        target_link_libraries(${CMAKE_PROJECT_NAME}
            ${CMAKE_THREAD_LIBS_INIT}
            ${X11_LIBRARIES}
            ${CMAKE_DL_LIBS}
            )
    endif()
endif()

# copy font file to the build directory
add_custom_command(
        TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/verdana.ttf
                ${CMAKE_CURRENT_BINARY_DIR}/verdana.ttf
                )
