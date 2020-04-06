#include "ptrace_helper.h"

long ptrace_handler(ptrace_func real_ptrace, ptreq request, pid_t pid, void *addr, void *data)
{
    // Do something better than this
    return real_ptrace(request, pid, addr, data);
}
