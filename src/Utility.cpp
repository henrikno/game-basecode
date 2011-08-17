#include <sys/time.h>
#include <stdio.h>
#include "Utility.h"

double getTime() {
    struct timeval tv;
    const int rc = gettimeofday(&tv, 0);
    if (rc == -1) {
        printf("ERROR: Bad call to gettimeofday\n");
        return -1;
    }
    return (double)tv.tv_sec + 1.0e-6 * (double)tv.tv_usec;
}
