#include <time.h>
#include <string.h>

 /* We will want to check for symbols in the OpenCL library.
  * On Windows, we must get the module handle for it, on Unix-like
  * systems we can just use RTLD_DEFAULT
  */
#ifdef _MSC_VER
# include <windows.h>
# define dlsym GetProcAddress
# define DL_MODULE GetModuleHandle("OpenCL")
#else
# include <dlfcn.h>
# define DL_MODULE ((void*)0) /* This would be RTLD_DEFAULT */
#endif

  /* Load STDC format macros (PRI*), or define them
   * for those crappy, non-standard compilers
   */
#include "fmtmacros.h"

   // More support for the horrible MS C compiler
#ifdef _MSC_VER
#include "ms_support.h"
#endif

#include "error.h"
#include "memory.h"
#include "strbuf.h"

#include "ext.h"
#include "ctx_prop.h"
#include "info_loc.h"
#include "info_ret.h"
#include "opt_out.h"
#include "cl_functions.h"

#include "cuda_devices.h"

#if defined __GNUC__ && ((__GNUC__*10 + __GNUC_MINOR__) < 46)
#pragma GCC diagnostic warning "-Wstrict-aliasing"
#endif

void version(void)
{
	puts("clinfo version 2.2.18.04.06");
}

void usage(void)
{
	version();
	puts("Display properties of all available OpenCL platforms and devices");
	puts("Usage: clinfo [options ...]\n");
	puts("Options:");
	puts("\t--all-props, -a\t\ttry all properties, only show valid ones");
	puts("\t--always-all-props, -A\t\tshow all properties, even if invalid");
	puts("\t--human\t\thuman-friendly output (default)");
	puts("\t--raw\t\traw output");
	puts("\t--offline\talso show offline devices");
	puts("\t--list, -l\tonly list the platforms and devices by name");
	puts("\t-h, -?\t\tshow usage");
	puts("\t--version, -v\tshow version\n");
	puts("Defaults to raw mode if invoked with");
	puts("a name that contains the string \"raw\"");
}

extern char *line_pfx;

int main(int argc, char *argv[])
{
	cl_uint p;
	cl_int err;
	int a = 0;

	struct opt_out output;

	struct platform_list plist;
	init_plist(&plist);

	output.mode = CLINFO_HUMAN;
	output.cond = COND_PROP_CHECK;
	output.brief = CL_FALSE;
	output.offline = CL_FALSE;
	output.check_size = CL_FALSE;

	/* if there's a 'raw' in the program name, switch to raw output mode */
	if (strstr(argv[0], "raw"))
		output.mode = CLINFO_RAW;

	/* process command-line arguments */
	while (++a < argc) {
		if (!strcmp(argv[a], "-a") || !strcmp(argv[a], "--all-props"))
			output.cond = COND_PROP_TRY;
		else if (!strcmp(argv[a], "-A") || !strcmp(argv[a], "--always-all-props"))
			output.cond = COND_PROP_SHOW;
		else if (!strcmp(argv[a], "--raw"))
			output.mode = CLINFO_RAW;
		else if (!strcmp(argv[a], "--human"))
			output.mode = CLINFO_HUMAN;
		else if (!strcmp(argv[a], "--offline"))
			output.offline = CL_TRUE;
		else if (!strcmp(argv[a], "-l") || !strcmp(argv[a], "--list"))
			output.brief = CL_TRUE;
		else if (!strcmp(argv[a], "-?") || !strcmp(argv[a], "-h")) {
			usage();
			return 0;
		}
		else if (!strcmp(argv[a], "--version") || !strcmp(argv[a], "-v")) {
			version();
			return 0;
		}
		else {
			fprintf(stderr, "ignoring unknown command-line parameter %s\n", argv[a]);
		}
	}
	output.detailed = !output.brief;

	err = clGetPlatformIDs(0, NULL, &plist.num_platforms);
	if (err != CL_PLATFORM_NOT_FOUND_KHR)
		CHECK_ERROR(err, "number of platforms");

	if (!output.brief)
		printf(I0_STR "%" PRIu32 "\n",
		(output.mode == CLINFO_HUMAN ?
			"Number of platforms" : "#PLATFORMS"),
			plist.num_platforms);
	if (!plist.num_platforms)
		return 0;

	alloc_plist(&plist);
	err = clGetPlatformIDs(plist.num_platforms, plist.platform, NULL);
	CHECK_ERROR(err, "platform IDs");

	ALLOC_TYPED(line_pfx, char*, 1, "line prefix");

	for (p = 0; p < plist.num_platforms; ++p) {
		gatherPlatformInfo(&plist, p, &output);
		if (output.detailed)
			puts("");
	}
	showDevices(&plist, &output);
	if (output.detailed) {
		if (output.mode != CLINFO_RAW)
			checkNullBehavior(&plist, &output);
		oclIcdProps(&plist, &output);
	}

	free_plist(&plist);

	cudaDevicesInfo(argc, argv);

	return 0;
}