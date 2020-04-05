#ifndef __PTRACE_HELPER_H_
#define __PTRACE_HELPER_H_

#include <sys/ptrace.h>
#include <sys/types.h>

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <dlfcn.h>

typedef enum __ptrace_request ptreq;
typedef long (*ptrace_func)(ptreq, ...);

void log_ptrace(ptreq request, pid_t pid, void *addr, void *data);

long ptrace(ptreq request, ...)
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
    va_end(args);

    log_ptrace(request, pid, addr, data);
    long real_ret = real_ptrace(request, pid, addr, data);

    // Fake success
    if(real_ret < 0) {
        return 0;
    }
    return real_ret;
}

// Logging code

const char *ptrace_req_str(ptreq);

void log_ptrace(ptreq request, pid_t pid, void *addr, void *data)
{
    const char *req_str = ptrace_req_str(request);
    printf("ptrace(%s, %ld, %p, %p)\n", req_str, (long)pid, addr, data);
}

const char *ptrace_req_str(ptreq request)
{
    const char *req_str = "UNKNOWN";
    switch(request) {
        case           PTRACE_TRACEME:
            req_str = "PTRACE_TRACEME"; break;

        case           PTRACE_ATTACH:
            req_str = "PTRACE_ATTACH"; break;
        case           PTRACE_DETACH:
            req_str = "PTRACE_DETACH"; break;
        case           PTRACE_SEIZE:
            req_str = "PTRACE_SEIZE"; break;
        case           PTRACE_INTERRUPT:
            req_str = "PTRACE_INTERRUPT"; break;
        case           PTRACE_LISTEN:
            req_str = "PTRACE_LISTEN"; break;

        case           PTRACE_CONT:
            req_str = "PTRACE_CONT"; break;
        case           PTRACE_SINGLESTEP:
            req_str = "PTRACE_SINGLESTEP"; break;
        case           PTRACE_SYSCALL:
            req_str = "PTRACE_SYSCALL"; break;
        case           PTRACE_KILL:
            req_str = "PTRACE_KILL"; break;

        case           PTRACE_SYSEMU:
            req_str = "PTRACE_SYSEMU"; break;
        case           PTRACE_SYSEMU_SINGLESTEP:
            req_str = "PTRACE_SYSEMU_SINGLESTEP"; break;

        case           PTRACE_PEEKTEXT:
            req_str = "PTRACE_PEEKTEXT"; break;
        case           PTRACE_PEEKUSER:
            req_str = "PTRACE_PEEKUSER"; break;
        case           PTRACE_PEEKDATA:
            req_str = "PTRACE_PEEKDATA"; break;

        case           PTRACE_POKETEXT:
            req_str = "PTRACE_POKETEXT"; break;
        case           PTRACE_POKEUSER:
            req_str = "PTRACE_POKEUSER"; break;
        case           PTRACE_POKEDATA:
            req_str = "PTRACE_POKEDATA"; break;

        case           PTRACE_GETREGS:
            req_str = "PTRACE_GETREGS"; break;
        case           PTRACE_GETFPREGS:
            req_str = "PTRACE_GETFPREGS"; break;
        case           PTRACE_GETFPXREGS:
            req_str = "PTRACE_GETFPXREGS"; break;
        case           PTRACE_SETREGS:
            req_str = "PTRACE_SETREGS"; break;
        case           PTRACE_SETFPREGS:
            req_str = "PTRACE_SETFPREGS"; break;
        case           PTRACE_SETFPXREGS:
            req_str = "PTRACE_SETFPXREGS"; break;
        case           PTRACE_GETREGSET:
            req_str = "PTRACE_GETREGSET"; break;
        case           PTRACE_SETREGSET:
            req_str = "PTRACE_SETREGSET"; break;

        case           PTRACE_GET_THREAD_AREA:
            req_str = "PTRACE_GET_THREAD_AREA"; break;
        case           PTRACE_SET_THREAD_AREA:
            req_str = "PTRACE_SET_THREAD_AREA"; break;

        case           PTRACE_GETSIGINFO:
            req_str = "PTRACE_GETSIGINFO"; break;
        case           PTRACE_SETSIGINFO:
            req_str = "PTRACE_SETSIGINFO"; break;
        case           PTRACE_PEEKSIGINFO:
            req_str = "PTRACE_PEEKSIGINFO"; break;
        case           PTRACE_GETSIGMASK:
            req_str = "PTRACE_GETSIGMASK"; break;
        case           PTRACE_SETSIGMASK:
            req_str = "PTRACE_SETSIGMASK"; break;

        case           PTRACE_ARCH_PRCTL:
            req_str = "PTRACE_ARCH_PRCTL"; break;
        case           PTRACE_SINGLEBLOCK:
            req_str = "PTRACE_SINGLEBLOCK"; break;
        case           PTRACE_SETOPTIONS:
            req_str = "PTRACE_SETOPTIONS"; break;
        case           PTRACE_GETEVENTMSG:
            req_str = "PTRACE_GETEVENTMSG"; break;
        case           PTRACE_SECCOMP_GET_FILTER:
            req_str = "PTRACE_SECCOMP_GET_FILTER"; break;
        case           PTRACE_SECCOMP_GET_METADATA:
            req_str = "PTRACE_SECCOMP_GET_METADATA"; break;
        case           PTRACE_GET_SYSCALL_INFO:
            req_str = "PTRACE_GET_SYSCALL_INFO"; break;
    }

    return req_str;
}

#endif // __PTRACE_HELPER_H_
