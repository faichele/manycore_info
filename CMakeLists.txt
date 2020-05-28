cmake_minimum_required(VERSION 3.13)
PROJECT(manycore_info)

find_package(CUDA REQUIRED)
find_package(OpenCL REQUIRED)

message(STATUS "CUDA include directories: ${CUDA_INCLUDE_DIRS}")
message(STATUS "OpenCL include directories: ${OpenCL_INCLUDE_DIRS}")

SET(MANYCORE_INFO_HEADERS 
	src/error.h
	src/ext.h
	src/ctx_prop.h
	src/fmtmacros.h
	src/memory.h
	src/ms_support.h
	src/info_loc.h
	src/info_ret.h
	src/opt_out.h
	src/strbuf.h
	src/cuda_devices.h
	)
	
SET(MANYCORE_INFO_SOURCES
	src/cl_info.cpp
	src/cuda_info.cpp
	src/main.cpp
	src/strbuf.cpp
	src/error.cpp
	src/cuda_devices.cpp
	)

include_directories(${CUDA_INCLUDE_DIRS} ${OpenCL_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} ${MANYCORE_INFO_HEADERS} ${MANYCORE_INFO_SOURCES})
target_link_libraries(${PROJECT_NAME} ${CUDA_LIBRARIES} ${OpenCL_LIBRARIES})