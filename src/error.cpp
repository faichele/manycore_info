#include "error.h"

cl_int check_ocl_error(cl_int err, const char *what, const char *func, int line)
{
	if (err != CL_SUCCESS) {
		fflush(stdout);
		fflush(stderr);
		fprintf(stderr, "%s:%u: %s : error %d\n",
			func, line, what, err);
		fflush(stderr);
	}
	return err;
}

cl_int report_ocl_error_basic(struct _strbuf *str, cl_int err, const char *what, const char *func, int line)
{
	if (err != CL_SUCCESS) {
		snprintf(str->buf, str->sz, "<%s:%d: %s : error %d>",
			func, line, what, err);
	}
	return err;
}


cl_int
report_ocl_error_loc(struct _strbuf *str, cl_int err, const char *fmt,
	const struct info_loc *loc)
{
	static char full_fmt[1024];
	if (err != CL_SUCCESS) {
		snprintf(full_fmt, 1024, "<%s:%" PRIuS ": %s : error %d>",
			loc->function, loc->line, fmt, err);
		snprintf(str->buf, str->sz, full_fmt, loc->sname);
	}
	return err != CL_SUCCESS;
}

void
report_size_mismatch(struct _strbuf *str, size_t req, size_t ours,
	const struct info_loc *loc)
{
	snprintf(str->buf, str->sz, "<%s:%" PRIuS ": %s : size mismatch "
		"(requested %" PRIuS ", we offer %" PRIuS ")>",
		loc->function, loc->line, loc->sname,
		req, ours);
}