cmake_minimum_required(VERSION 3.1.0)

project(hotspot-perfparser)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

if (APPLE)
set(Intl_INCLUDE_DIR /usr/local/opt/gettext/include)
set(Intl_LIBRARY /usr/local/opt/gettext/lib/libintl.dylib)
endif()
find_package(Intl REQUIRED)

if (APPLE)
set(Argp_ROOT_DIR /usr/local/opt/argp-standalone)
endif()
find_package(argp REQUIRED)

find_package(ZLIB REQUIRED)
find_package(LibLZMA REQUIRED)
find_package(BZip2 REQUIRED)

find_package(Qt6 COMPONENTS Network Core REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(ELFUTILS_INCLUDE_DIRS
	${Intl_INCLUDE_DIR} ${ARGP_INCLUDE_DIRS} ${ZLIB_INCLUDE_DIRS} ${LIBLZMA_INCLUDE_DIRS} ${BZIP2_INCLUDE_DIRS}
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

file(GLOB ELF_SRCS "ThirdParty/elfutils/src/elf/*.c")

add_library(elf STATIC ${ELF_SRCS})
target_include_directories(elf PRIVATE ${ELFUTILS_INCLUDE_DIRS})
target_compile_definitions(elf PRIVATE _GNU_SOURCE HAVE_CONFIG_H LOCALEDIR="" _FORTIFY_SOURCE=2)
target_link_libraries(elf ${Intl_LIBRARY})

file(GLOB DW_SRCS "ThirdParty/elfutils/src/dw/*.c")

add_library(dw STATIC ${DW_SRCS})
target_include_directories(dw PRIVATE ${ELFUTILS_INCLUDE_DIRS})
target_compile_definitions(dw PRIVATE _GNU_SOURCE HAVE_CONFIG_H LOCALEDIR="" _FORTIFY_SOURCE=2)
target_link_libraries(dw ${Intl_LIBRARY})

file(GLOB DWFL_SRCS "ThirdParty/elfutils/src/dwfl/*.c")

add_library(dwfl STATIC ${DWFL_SRCS})
target_include_directories(dwfl PRIVATE ${ELFUTILS_INCLUDE_DIRS})
target_compile_definitions(dwfl PRIVATE _GNU_SOURCE HAVE_CONFIG_H LOCALEDIR="" _FORTIFY_SOURCE=2)
target_link_libraries(dwfl ${Intl_LIBRARY} ${ARGP_LIBRARIES} ${ZLIB_LIBRARIES} ${LIBLZMA_LIBRARIES} ${BZIP2_LIBRARIES})

file(GLOB DWELF_SRCS "ThirdParty/elfutils/src/dwelf/*.c")

add_library(dwelf STATIC ${DWELF_SRCS})
target_include_directories(dwelf PRIVATE ${ELFUTILS_INCLUDE_DIRS})
target_compile_definitions(dwelf PRIVATE _GNU_SOURCE HAVE_CONFIG_H LOCALEDIR="" _FORTIFY_SOURCE=2)
target_link_libraries(dwelf ${Intl_LIBRARY})

file(GLOB EBL_SRCS "ThirdParty/elfutils/src/ebl/*.c")

add_library(ebl STATIC ${EBL_SRCS})
target_include_directories(ebl PRIVATE ${ELFUTILS_INCLUDE_DIRS})
target_compile_definitions(ebl PRIVATE _GNU_SOURCE HAVE_CONFIG_H LOCALEDIR="" _FORTIFY_SOURCE=2)
target_link_libraries(ebl ${Intl_LIBRARY})

file(GLOB BACKENDS_SRCS "ThirdParty/elfutils/src/backends/*.c")

add_library(backends STATIC ${BACKENDS_SRCS})
target_include_directories(backends PRIVATE ${ELFUTILS_INCLUDE_DIRS})
target_compile_definitions(backends PRIVATE _GNU_SOURCE HAVE_CONFIG_H LOCALEDIR="" _FORTIFY_SOURCE=2 __BEGIN_DECLS=; __END_DECLS=;)
target_link_libraries(backends ${Intl_LIBRARY})

file(GLOB SRCS "src/*.cpp" "include/*.h")

add_executable(${PROJECT_NAME} ${SRCS})
target_include_directories(${PROJECT_NAME} PRIVATE include)
target_include_directories(${PROJECT_NAME} PRIVATE ThirdParty/elfutils/include)
target_link_libraries(${PROJECT_NAME} Qt6::Core Qt6::Network elf dw dwfl dwelf ebl backends)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

# TODO Perhaps, we also need to build perf2text binary

