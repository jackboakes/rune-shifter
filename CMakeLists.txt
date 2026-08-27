cmake_minimum_required(VERSION 3.24...3.30)
project(raylib-6.x-gamejam LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS True)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/code/third_party/raylib/CMakeLists.txt")
    add_subdirectory(code/third_party/raylib)
else()
    message(WARNING "raylib subdirectory not found at code/third_party/raylib.")
endif()

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE
    code/game.cpp
    code/game.h
	code/assets.cpp
	code/assets.h
    code/common/types.h
)

if ("${PLATFORM}" STREQUAL "Web")
    target_sources(${PROJECT_NAME} PRIVATE code/main_web.cpp)
else()
    target_sources(${PROJECT_NAME} PRIVATE code/main_desktop.cpp)
endif()
target_include_directories(${PROJECT_NAME} PRIVATE 
    code
    code/third_party/raylib/src
)

if (WIN32 AND NOT "${PLATFORM}" STREQUAL "Web")
    if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/raylib_game.rc")
        target_sources(${PROJECT_NAME} PRIVATE raylib_game.rc)
    endif()
    
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE $<IF:$<CONFIG:Release>,TRUE,FALSE>
    )
endif()

if ("${PLATFORM}" STREQUAL "Web")
    add_custom_command(
        TARGET ${PROJECT_NAME} PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/data $<TARGET_FILE_DIR:${PROJECT_NAME}>/../data
    )
else()
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/data $<TARGET_FILE_DIR:${PROJECT_NAME}>/data
    )
endif()

if (TARGET raylib)
    target_link_libraries(${PROJECT_NAME} PRIVATE raylib)
else()
    find_package(raylib REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE raylib)
endif()

if ("${PLATFORM}" STREQUAL "Web")
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")
    if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/minshell.html")
        set(MINSHELL_PATH "${CMAKE_CURRENT_SOURCE_DIR}/minshell.html")
    else()
        set(MINSHELL_PATH "${CMAKE_CURRENT_SOURCE_DIR}/code/third_party/raylib/src/minshell.html")
    endif()
    target_link_options(${PROJECT_NAME} PUBLIC 
        -sUSE_GLFW=3 
        --preload-file ${CMAKE_CURRENT_SOURCE_DIR}/data@data
        --shell-file ${MINSHELL_PATH}
    )
endif()

if(MSVC)
    # Sets the Visual Studio startup project to our project otherwise it will default to ALL_BUILD
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
endif()