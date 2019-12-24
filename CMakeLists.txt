cmake_minimum_required(VERSION 3.1.0)

project(hotspot-perfparser)

find_package(Qt6 COMPONENTS Network Core REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

file(GLOB SRCS "src/*.cpp" "include/*.h")

add_executable(${PROJECT_NAME} ${SRCS})
target_include_directories(${PROJECT_NAME} PRIVATE include)
target_include_directories(${PROJECT_NAME} PRIVATE ThirdParty/elfutils/include)
target_link_libraries(${PROJECT_NAME} Qt6::Core Qt6::Network)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

# TODO Perhaps, we also need to build perf2text binary

