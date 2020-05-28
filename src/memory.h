/* Memory handling */

#ifndef MEMORY_H
#define MEMORY_H

#include <stdlib.h>
#include <CL/cl.h>

#define CHECK_MEM(var, what) do { \
	if (!(var)) { \
		fprintf(stderr, "%s:%d: %s : Out of memory\n", \
			__func__, __LINE__, what); \
		exit(1); \
	} \
} while (0)

#define ALLOC(var, num, what) do { \
	var = calloc(num, sizeof(*(var))); \
	CHECK_MEM(var, what); \
} while (0)

#define ALLOC_TYPED(var, type, num, what) do { \
	var = (type)calloc(num, sizeof(*(var))); \
	CHECK_MEM(var, what); \
} while (0)

#define REALLOC(var, num, what) do { \
	var = realloc(var, (num)*sizeof(*(var))); \
	CHECK_MEM(var, what); \
} while (0)

#define REALLOC_TYPED(var, type, num, what) do { \
	var = (type)realloc(var, (num)*sizeof(*(var))); \
	CHECK_MEM(var, what); \
} while (0)

struct platform_list {
	/* Number of platforms in the system */
	cl_uint num_platforms;
	/* Total number of devices across all platforms */
	cl_uint ndevs_total;
	/* Number of devices allocated in all_devs array */
	cl_uint alloc_devs;
	/* Highest OpenCL version supported by any platform.
	 * If the OpenCL library / ICD loader only supports
	 * a lower version, problems may arise (such as
	 * API calls causing segfaults or any other unexpected
	 * behavior
	 */
	cl_uint max_plat_version;
	/* Largest number of devices on any platform */
	cl_uint max_devs;
	/* Length of the longest platform sname */
	cl_int max_sname_len;
	/* Array of platform IDs */
	cl_platform_id *platform;
	/* Array of device IDs (across all platforms) */
	cl_device_id *all_devs;
	/* Array of offsets in all_devs where the devices
	 * of each platform begin */
	cl_uint *dev_offset;
	/* Array of clinfo-specific platform data */
	struct platform_data *pdata;
	/* Arrau of clinfo-specifici platform checks */
	struct platform_info_checks *platform_checks;
};

void alloc_plist(struct platform_list *plist);
void free_plist(struct platform_list *plist);
void init_plist(struct platform_list *plist);

#endif
