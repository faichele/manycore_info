#include "strbuf.h"

void set_separator(const char* _sep)
{
	sep = _sep;
	sepsz = strlen(sep);
}

/* Note that no overflow check is done: it is assumed that _strbuf will have enough room */
void add_separator(struct _strbuf *str, size_t *offset)
{
	if (*offset)
		*offset += bufcpy_len(str, *offset, sep, sepsz);
}