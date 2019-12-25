cmake_minimum_required(VERSION 3.1.0)

project(hotspot-perfparser)

if (UNIX)
# Use $ORIGIN or @loader_path, which allow to pick up
# dependencies relative to the executable folder
if (APPLE)
set(CMAKE_INSTALL_RPATH "@loader_path" "@loader_path/lib" "@loader_path/../lib")
else()
set(CMAKE_INSTALL_RPATH "$ORIGIN" "$ORIGIN/lib" "$ORIGIN/../lib")
endif()
endif()

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

if (APPLE)
set(Intl_INCLUDE_DIR /usr/local/opt/gettext/include)
set(Intl_LIBRARY /usr/local/opt/gettext/lib/libintl.dylib)
endif()
find_package(Intl REQUIRED)

find_package(ZLIB REQUIRED)
find_package(LibLZMA REQUIRED)
find_package(BZip2 REQUIRED)

find_package(Qt6 COMPONENTS Network Core REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(ELFUTILS_INCLUDE_DIRS
	${Intl_INCLUDE_DIR} ${ZLIB_INCLUDE_DIRS} ${LIBLZMA_INCLUDE_DIRS} ${BZIP2_INCLUDE_DIRS}
	ThirdParty/elfutils/src/common
	ThirdParty/elfutils/include
	ThirdParty/elfutils/src/asm
	ThirdParty/elfutils/src/dwfl
	ThirdParty/elfutils/src/debuginfod
	ThirdParty/elfutils/src/dw
	ThirdParty/elfutils/src/dwelf
	ThirdParty/elfutils/src/ebl
	ThirdParty/elfutils/src/elf
	ThirdParty/elfutils/src/backends)

file(GLOB ELFUTILS_SRCS
	"ThirdParty/elfutils/src/common/tdestroy.c"
	"ThirdParty/elfutils/src/common/strchrnul.c"
	"ThirdParty/elfutils/src/elf/*.c"
	"ThirdParty/elfutils/src/dw/*.c"
	"ThirdParty/elfutils/src/dwfl/*.c"
	"ThirdParty/elfutils/src/dwelf/*.c"
	"ThirdParty/elfutils/src/ebl/*.c"
	"ThirdParty/elfutils/src/backends/*.c"
	"ThirdParty/elfutils/src/cpu/*.c")

add_library(elfutils STATIC ${ELFUTILS_SRCS})
target_include_directories(elfutils PRIVATE ${ELFUTILS_INCLUDE_DIRS})
target_compile_definitions(elfutils PRIVATE _GNU_SOURCE HAVE_CONFIG_H LOCALEDIR="" _FORTIFY_SOURCE=2 __BEGIN_DECLS=; __END_DECLS=;)
target_link_libraries(elfutils ${Intl_LIBRARY} ${ZLIB_LIBRARIES} ${LIBLZMA_LIBRARIES} ${BZIP2_LIBRARIES})

file(GLOB SRCS "src/*.cpp" "include/*.h")

add_executable(${PROJECT_NAME} ${SRCS})
target_include_directories(${PROJECT_NAME} PRIVATE include)
target_include_directories(${PROJECT_NAME} PRIVATE ThirdParty/elfutils/include)
target_link_libraries(${PROJECT_NAME} Qt6::Core Qt6::Network elfutils)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

# TODO Perhaps, we also need to build perf2text binary

string(TOLOWER ${CMAKE_BUILD_TYPE} BUILD_TYPE)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/TargetArch.cmake)
target_architecture(ARCH)

string(TOLOWER ${CMAKE_SYSTEM_PROCESSOR} ARCH)
string(TOLOWER ${CMAKE_SYSTEM_NAME} OS)
set(ARCHIVE "${PROJECT_NAME}-${ARCH}-${OS}-${BUILD_TYPE}")
set(CPACK_PACKAGE_FILE_NAME ${ARCHIVE})
set(CPACK_GENERATOR "ZIP")
set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)
set(CPACK_OUTPUT_FILE_PREFIX ${CMAKE_INSTALL_PREFIX})

include(CPack)

