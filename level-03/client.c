#include <sys/prctl.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    pid_t my_pid = getpid(), child_pid;
    if((child_pid = fork()) == 0) {
        raise(SIGSTOP);
        if(ptrace(PTRACE_ATTACH, my_pid, 0L, 0L) < 0) {
            puts("child ptrace returned failure.");
            kill(my_pid, SIGKILL);
            return EXIT_FAILURE;
        }
    } else {
        // Trace the child as well
        prctl(PR_SET_PTRACER, child_pid);
        if(ptrace(PTRACE_ATTACH, child_pid, 0L, 0L) < 0) {
            puts("parent ptrace returned failure.");
            kill(child_pid, SIGKILL);
            return EXIT_FAILURE;
        }
        ptrace(PTRACE_CONT, child_pid, 0L, 0L);

        wait(NULL);
        puts("Client running normally.");
    }
    return EXIT_SUCCESS;
}
