#include "ptrace_helper.h"

long ptrace_handler(ptrace_func real_ptrace, ptreq request, pid_t pid, void *addr, void *data)
{
    long real_ret = real_ptrace(request, pid, addr, data);

    // Fake success
    if(real_ret < 0) {
        return 0;
    }
    return real_ret;
}
