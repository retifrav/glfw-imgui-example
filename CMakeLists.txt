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

set(GLAD_PREFIX
    "${CMAKE_CURRENT_SOURCE_DIR}/_dependencies/glad"
    CACHE STRING "Path to glad"
)
set(GLFW_PREFIX
    "${CMAKE_CURRENT_SOURCE_DIR}/_dependencies/glfw"
    CACHE STRING "Path to GLFW"
)
set(DEAR_IMGUI_PREFIX
    "${CMAKE_CURRENT_SOURCE_DIR}/_dependencies/DearImGui"
    CACHE STRING "Path to Dear ImGui"
)

option(CRT_LINKAGE_STATIC          "Link MSVC runtime statically"                       0)
option(USING_PACKAGE_MANAGER       "Using a package manager for resolving dependencies" 0)
option(USING_PACKAGE_MANAGER_CONAN "Using Conan package manager"                        0)
option(USING_PACKAGE_MANAGER_VCPKG "Using Conan package manager"                        0)

if(WIN32 AND CRT_LINKAGE_STATIC)
    #add_compile_options("/MT")
    if(POLICY CMP0091)
        cmake_policy(SET CMP0091 NEW)
    endif()
    set_property(TARGET ${CMAKE_PROJECT_NAME}
        PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
    )
endif()

if(USING_PACKAGE_MANAGER_CONAN OR USING_PACKAGE_MANAGER_VCPKG)
    set(USING_PACKAGE_MANAGER 1)
endif()

# ---

if(USING_PACKAGE_MANAGER)
    if(USING_PACKAGE_MANAGER_CONAN)
        include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
        conan_basic_setup()
    endif()
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
if (USING_PACKAGE_MANAGER)
    find_package(glad CONFIG REQUIRED)
    if(USING_PACKAGE_MANAGER_VCPKG)
        target_link_libraries(${CMAKE_PROJECT_NAME}
            PRIVATE
                glad::glad
        )
    else()
        target_link_libraries(${CMAKE_PROJECT_NAME}
            PRIVATE
                glad
        )
    endif()
else()
    add_library(glad "${GLAD_PREFIX}/src/glad.c")
    include_directories(
        "${GLAD_PREFIX}/include"
    )
    target_link_libraries(${CMAKE_PROJECT_NAME}
        PRIVATE
            glad
    )
endif()

# GLFW
if(USING_PACKAGE_MANAGER)#OR (NOT WIN32))
    find_package(glfw3 CONFIG REQUIRED)
else()
    #find_library(glfw glfw3 PATHS "${GLFW_PREFIX}/lib")
    set(GLFW_BUILD_EXAMPLES OFF)
    set(GLFW_BUILD_TESTS OFF)
    set(GLFW_BUILD_DOCS OFF)
    add_subdirectory(${GLFW_PREFIX})
    include_directories(
        "${GLFW_PREFIX}/include"
    )
    # target_link_libraries(${CMAKE_PROJECT_NAME}
    #     PRIVATE
    #         ${glfw}
    # )
endif()

# Dear ImGui
if(USING_PACKAGE_MANAGER)
    find_package(DearImGui CONFIG REQUIRED)
    target_link_libraries(${CMAKE_PROJECT_NAME}
        PRIVATE
            DearImGui
    )
else() # add Dear ImGui sources manually
    list(APPEND sources
        ${DEAR_IMGUI_PREFIX}/imgui.cpp
        ${DEAR_IMGUI_PREFIX}/imgui_stdlib.cpp
        ${DEAR_IMGUI_PREFIX}/imgui_draw.cpp
        ${DEAR_IMGUI_PREFIX}/imgui_tables.cpp
        ${DEAR_IMGUI_PREFIX}/imgui_widgets.cpp
        ${DEAR_IMGUI_PREFIX}/imgui_impl_glfw.cpp
        ${DEAR_IMGUI_PREFIX}/imgui_impl_opengl3.cpp
        ${DEAR_IMGUI_PREFIX}/imgui_demo.cpp
    )
endif()

target_sources(${CMAKE_PROJECT_NAME}
    PRIVATE
        ${sources}
)

target_link_libraries(${CMAKE_PROJECT_NAME}
    PRIVATE
        ${OPENGL_gl_LIBRARY}
        glfw
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
    COMPONENT ${CMAKE_PROJECT_NAME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}/${CMAKE_PROJECT_NAME}
)
install(FILES
    ${resource_files}
    DESTINATION ${CMAKE_INSTALL_BINDIR}/${CMAKE_PROJECT_NAME}
    COMPONENT ${CMAKE_PROJECT_NAME}
)
