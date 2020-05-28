#pragma once

#include <cuda_runtime.h>
#include "helper_cuda.h"

#include <iostream>
#include <memory>
#include <string>

#if CUDART_VERSION < 5000

// CUDA-C includes
#include <cuda.h>

// This function wraps the CUDA Driver API into a template function
template <class T>
inline void getCudaAttribute(T *attribute, CUdevice_attribute device_attribute,
	int device) {
	CUresult error = cuDeviceGetAttribute(attribute, device_attribute, device);

	if (CUDA_SUCCESS != error) {
		fprintf(
			stderr,
			"cuSafeCallNoSync() Driver API error = %04d from file <%s>, line %i.\n",
			error, __FILE__, __LINE__);

		exit(EXIT_FAILURE);
	}
}

#endif /* CUDART_VERSION < 5000 */

void cudaDevicesInfo(int argc, char** argv);