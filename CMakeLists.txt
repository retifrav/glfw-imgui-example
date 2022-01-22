cmake_minimum_required(VERSION 3.21)

project("glfw-imgui"
    DESCRIPTION "Example application based on GLFW and Dear ImGui"
    VERSION 0.9.1
    LANGUAGES C CXX
)
#message("Project name: " ${CMAKE_PROJECT_NAME})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(${CMAKE_PROJECT_NAME})

# --- cache variables and options

set(GLAD_PREFIX "~/programs" CACHE STRING "Path to Glad")
set(GLFW_PREFIX "d:/programs/glfw" CACHE STRING "Path to GLFW")

option(USING_CONAN "Dependencies are handled by Conan" OFF)

# ---

if(USING_CONAN)
    include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup()
else()
    include_directories(
        "${CMAKE_CURRENT_SOURCE_DIR}/_dependencies"
    )
endif()

set(sources
    main.cpp
    functions.cpp
)

set(resource_files
    ${CMAKE_SOURCE_DIR}/JetBrainsMono-ExtraLight.ttf
)

find_package(OpenGL REQUIRED)

if (UNIX)
    if (NOT APPLE)
        find_package(Threads REQUIRED)
        find_package(X11 REQUIRED)
    endif()
endif()

# glad
if (USING_CONAN)
    find_package(glad CONFIG REQUIRED)
else()
    add_library(glad "${GLAD_PREFIX}/src/glad.c")
    include_directories(
        "${GLAD_PREFIX}/include"
    )
endif()

# GLFW
if(USING_CONAN OR (NOT WIN32))
    find_package(glfw3 CONFIG REQUIRED)
    target_link_libraries(${CMAKE_PROJECT_NAME}
        PRIVATE
            glfw
    )
else()
    find_library(glfw glfw3 PATHS "${GLFW_PREFIX}/lib")
    include_directories(
        "${GLFW_PREFIX}/include"
    )
    target_link_libraries(${CMAKE_PROJECT_NAME}
        PRIVATE
            ${glfw}
    )
endif()

# DearImGui
if(USING_CONAN)
    find_package(DearImGui CONFIG REQUIRED)
    target_link_libraries(${CMAKE_PROJECT_NAME}
        PRIVATE
            DearImGui
    )
else() # add Dear ImGui sources manually
    list(APPEND sources
        ${CMAKE_CURRENT_SOURCE_DIR}/_dependencies/DearImGui/imgui.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/_dependencies/DearImGui/imgui_stdlib.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/_dependencies/DearImGui/imgui_draw.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/_dependencies/DearImGui/imgui_tables.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/_dependencies/DearImGui/imgui_widgets.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/_dependencies/DearImGui/imgui_impl_glfw.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/_dependencies/DearImGui/imgui_impl_opengl3.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/_dependencies/DearImGui/imgui_demo.cpp
    )
endif()

target_sources(${CMAKE_PROJECT_NAME}
    PRIVATE
        ${sources}
)

target_link_libraries(${CMAKE_PROJECT_NAME}
    PRIVATE
        ${OPENGL_gl_LIBRARY}
        glad
)

if (UNIX)
    if (APPLE)
        target_link_libraries(${CMAKE_PROJECT_NAME}
            PRIVATE
                "-framework Cocoa"
                "-framework IOKit"
                "-framework CoreVideo"
        )
    else()
        target_link_libraries(${CMAKE_PROJECT_NAME}
            PRIVATE
                ${CMAKE_THREAD_LIBS_INIT}
                ${X11_LIBRARIES}
                ${CMAKE_DL_LIBS}
        )
    endif()
endif()

# --- installation

include(GNUInstallDirs)

install(TARGETS ${CMAKE_PROJECT_NAME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}/${CMAKE_PROJECT_NAME}
)
install(FILES
    ${resource_files}
    DESTINATION ${CMAKE_INSTALL_BINDIR}/${CMAKE_PROJECT_NAME}
)
