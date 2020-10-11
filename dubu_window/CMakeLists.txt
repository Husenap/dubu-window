set(target_name "dubu_window")

set(src_precompiled
    "src/dubu_window/precompiled.h")

set(src_window
    "src/dubu_window/window/Definitions.h"
    "src/dubu_window/window/Events.h"
    "src/dubu_window/window/IWindow.cpp"
    "src/dubu_window/window/IWindow.h"
    "src/dubu_window/window/GLFWWindow.cpp"
    "src/dubu_window/window/GLFWWindow.h")

set(src_dubu_window
    "src/dubu_window/dubu_window.h")

set(src_files
    ${src_precompiled}
    ${src_window}
    ${src_dubu_window})

# Project
add_library(${target_name} STATIC ${src_files})
set_target_properties(${target_name} PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(${target_name}
    compiler_features
    compiler_warnings
    dubu_event
    glfw)

target_include_directories(${target_name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_precompile_headers(${target_name} PUBLIC ${src_precompiled})

source_group("src" FILES ${src_dubu_window})
source_group("src/window" FILES ${src_window})
source_group("src/precompiled" FILES ${src_precompiled})
source_group("src/precompiled" REGULAR_EXPRESSION "cmake_pch\.[ch]xx")

set_target_properties(${target_name} PROPERTIES FOLDER ${${PROJECT_NAME}_FOLDER})
