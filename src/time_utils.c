#include <errno.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

/* msleep(): sleep for the requested number of milliseconds. */
/* from : https://stackoverflow.com/questions/1157209/is-there-an-alternative-sleep-function-in-c-to-milliseconds*/
int msleep(long msec){
    struct timespec ts;
    int res;
    if (msec < 0) {
        errno = EINVAL;
        return -1;
    }
    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;
    do {
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);
    return res;
}
/* mseconds(): get microsecond timestamp */
/* from : https://stackoverflow.com/questions/11765301/how-do-i-get-the-unix-timestamp-in-c-as-an-int */

int64_t mseconds() {

    struct timespec tms;

    if (! timespec_get(&tms, TIME_UTC)) {
        return -1;
    }
    /* seconds, multiplied with 1 million */
    int64_t micros = tms.tv_sec * 1000000;
    /* Add full microseconds */
    micros += tms.tv_nsec/1000;
    /* round up if necessary */
    if (tms.tv_nsec % 1000 >= 500) {
        ++micros;
    }
    return micros;
}

