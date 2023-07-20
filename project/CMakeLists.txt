cmake_minimum_required(VERSION 3.16)

# set the project name and version
project(Tutorial VERSION 1.0.2)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# print compile time
string(TIMESTAMP COMPILE_TIME %Y%m%d-%H%M%S)

# set the src list
SET(SRC_LIST tutorial.cpp)

# add the executable
add_executable(${PROJECT_NAME} ${SRC_LIST})

# print version of the project
configure_file(TutorialConfig.h.in TutorialConfig.h)

# add the MathFunctions library
add_subdirectory(MathFunctions)

target_link_libraries(${PROJECT_NAME} PUBLIC MathFunctions)

# must be the end of CMakeLists.txt
# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
target_include_directories(${PROJECT_NAME} PUBLIC
                           ${PROJECT_BINARY_DIR}
                           ${PROJECT_SOURCE_DIR}/MathFunctions
                           )