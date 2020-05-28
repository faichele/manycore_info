/* OpenCL error handling */

#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>

#include "ext.h"
#include "info_loc.h"
#include "fmtmacros.h"
#include "strbuf.h"

cl_int check_ocl_error(cl_int err, const char *what, const char *func, int line);
cl_int report_ocl_error_basic(struct _strbuf *str, cl_int err, const char *what, const char *func, int line);
cl_int report_ocl_error_loc(struct _strbuf *str, cl_int err, const char *fmt, const struct info_loc *loc);
void report_size_mismatch(struct _strbuf *str, size_t req, size_t ours, const struct info_loc *loc);

#define CHECK_ERROR(error, what) if (check_ocl_error(error, what, __func__, __LINE__)) exit(1)

#define REPORT_ERROR(str, err, what) report_ocl_error_basic(str, err, what, __func__, __LINE__)
#define REPORT_ERROR_LOC(ret, err, loc, what) report_ocl_error_loc(&((ret)->err_str), err, what, loc)
#define REPORT_SIZE_MISMATCH(str, loc, req, ours) report_size_mismatch(str, req, ours, loc)

#endif
