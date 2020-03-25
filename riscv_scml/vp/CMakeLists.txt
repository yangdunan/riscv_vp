cmake_minimum_required(VERSION 2.8.12)
project(riscv-vp)

set(SCML_ROOT $ENV{SCML_HOME})
set(SCML_INCLUDE64_DIR ${SCML_ROOT}/include)

set(SCML_LIB64_DIR -L${SCML_ROOT}/lib-linux64)
set(SCML_LIBRARIES scml)


option(USE_SYSTEM_SYSTEMC "use systemc version provided by the system" OFF)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-Wall")			#"-Wall -Wextra"
set(CMAKE_CXX_FLAGS_DEBUG "-g3")        #"-fsanitize=address -fno-omit-frame-pointer"
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR})

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

find_package( Boost REQUIRED COMPONENTS iostreams program_options )

if(NOT USE_SYSTEM_SYSTEMC)
	find_package( SystemC REQUIRED)
	include_directories( ${SystemC_INCLUDE_DIRS})
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -lsystemc")
endif()

find_package( SoftFloat REQUIRED )
include_directories( ${SoftFloat_INCLUDE_DIRS} )

subdirs(src)
