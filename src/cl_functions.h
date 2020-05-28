#pragma once
#include "memory.h"

/* Data for the OpenCL library / ICD loader */
struct icdl_data {
	/* auto-detected OpenCL version support for the ICD loader */
	cl_uint detected_version;
	/* OpenCL version support declared by the ICD loader */
	cl_uint reported_version;
};

void gatherPlatformInfo(struct platform_list *plist, cl_uint p, const struct opt_out *output);
void showDevices(const struct platform_list *plist, const struct opt_out *output);
void checkNullBehavior(const struct platform_list *plist, const struct opt_out *output);
struct icdl_data oclIcdProps(const struct platform_list *plist, const struct opt_out *output);