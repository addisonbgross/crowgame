cmake_minimum_required(VERSION 2.8)
project(opulence)

IF (WIN32)
    add_definitions(-DGLEW_STATIC)
ENDIF()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${opulence_SOURCE_DIR}/")

find_package(Threads)
find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLM REQUIRED)

IF (NOT WIN32)
    find_package(GLEW REQUIRED)
ENDIF()


include_directories(${PROJECT_BINARY_DIR}
                    ${OPENGL_INCLUDE_DIRS}
                    ${SDL2_INCLUDE_DIR}
                    ${GLEW_INCLUDE_DIR}
                    ${GLM_INCLUDE_DIRS})

IF (WIN32)
    set(SOURCE_FILES
        ../main.cpp
        opulence.cpp
        src/service/glew/glew.h
        src/service/glew/wglew.h
        src/service/glew/glew.c)
ELSE()
    set(SOURCE_FILES
        ../main.cpp
        opulence.cpp)
ENDIF()

set(HEADER_FILES)
set_source_files_properties(../main.cpp
                            opulence.cpp
                            PROPERTIES
                            LANGUAGE CXX)

add_executable(opulence
            ${SOURCE_FILES}

            # add your cpp and hpp files here!
            # one per line
)

target_link_libraries(opulence
                    ${SDL2_LIBRARY}
                    ${GLEW_LIBRARY}
                    ${OPENGL_LIBRARIES})
