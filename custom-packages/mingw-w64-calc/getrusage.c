/*
 * Copyright (c) 2014 Nicira, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <errno.h>
#include <processthreadsapi.h>
#include <psapi.h>
#include <sys/resource.h>
#include <time.h>

static void
usage_to_timeval(FILETIME *ft, struct timeval *tv)
{
    ULARGE_INTEGER time;

    time.LowPart = ft->dwLowDateTime;
    time.HighPart = ft->dwHighDateTime;
    tv->tv_sec = time.QuadPart / 10000000;
    tv->tv_usec = (time.QuadPart % 10000000) / 10;
}

int
getrusage(int who, struct rusage *usage)
{
    FILETIME creation_time, exit_time, kernel_time, user_time;
    PROCESS_MEMORY_COUNTERS pmc;

    memset(usage, 0, sizeof(struct rusage));
    if (usage == (struct rusage *) NULL) {
        errno = EFAULT;
        return -1;
    }
    else if (who == RUSAGE_SELF) {
        if (!GetProcessTimes(GetCurrentProcess(), &creation_time, &exit_time,
                             &kernel_time, &user_time)) {
	    errno = EPERM;
            return -1;
        }
        if (!GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
	    errno = EPERM;
            return -1;
        }
        usage_to_timeval(&kernel_time, &usage->ru_stime);
        usage_to_timeval(&user_time, &usage->ru_utime);
        usage->ru_majflt = pmc.PageFaultCount;
        usage->ru_maxrss = pmc.PeakWorkingSetSize / 1024;
        return 0;
    } else if (who == RUSAGE_THREAD) {
        if (!GetThreadTimes(GetCurrentThread(), &creation_time, &exit_time,
                            &kernel_time, &user_time)) {
	    errno = EPERM;
            return -1;
        }
        usage_to_timeval(&kernel_time, &usage->ru_stime);
        usage_to_timeval(&user_time, &usage->ru_utime);
        return 0;
    } else {
        /* Only RUSAGE_SELF and RUSAGE_THREAD are supported in this implementation for now */
        errno = EINVAL;        return -1;
    }
}


#ifdef TEST
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include <stdio.h>

int main() {
    struct rusage usage;
    struct timeval sstart, send, ustart, uend;
    int i, j, k = 0;

    getrusage(RUSAGE_SELF, &usage);
    sstart = usage.ru_stime;
    ustart = usage.ru_utime;
    for (i = 0; i < 10000; i++) {
        /* Double loop for more interesting results. */
        for (j = 0; j < 10000; j++) {
            k += 20; 
        }
    }
    getrusage(RUSAGE_SELF, &usage);
    send = usage.ru_stime;
    uend = usage.ru_utime;
    printf("Started at: sys=%ld.%lds, user=%ld.%lds\n", 
	   sstart.tv_sec, 
	   sstart.tv_usec, ustart.tv_sec, ustart.tv_usec);
    printf("Ended at:   sys=%ld.%lds, user=%ld.%lds\n", send.tv_sec, 
	   send.tv_usec, uend.tv_sec, 
	   uend.tv_usec);
    return 0;
}
#endif
