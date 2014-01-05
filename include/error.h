#ifndef _error_h_
#define _error_h_

#include<stdio.h>
#include<errno.h>

void fail(char *msg)
{
    printf("FAIL: %s, caused by %s\n", msg, strerror(errno));
}

#endif
