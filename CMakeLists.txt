project(hotspot-perfparser)

find_package(Qt6 COMPONENTS Core REQUIRED)

file(GLOB SRCS "src/*.cpp")

add_executable(${PROJECT_NAME} ${SRCS})

target_link_libraries(${PROJECT_NAME} Qt6::Core)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

# TODO Perhaps, we also need to build perf2text binary

