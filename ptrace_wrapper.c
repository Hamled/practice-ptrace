#include <sys/ptrace.h>
#include <sys/types.h>

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <dlfcn.h>

typedef long (*ptrace_func)(enum __ptrace_request, ...);

long ptrace(enum __ptrace_request request, ...)
{
    static ptrace_func real_ptrace = NULL;
    if(!real_ptrace) {
        real_ptrace = (ptrace_func)dlsym(RTLD_NEXT, "ptrace");
        if(!real_ptrace) {
            puts("Could not dynamically load ptrace symbol.");
            abort();
        }
    }

    va_list args;
    va_start(args, request);
    pid_t pid = va_arg(args, pid_t);
    void *addr = va_arg(args, void *);
    void *data = va_arg(args, void *);

    puts("ptrace wrapper called");
    return real_ptrace(request, pid, addr, data);
}
