#include <sys/ptrace.h>

#include <stdlib.h>
#include <stdio.h>

int main() {
    // First ptrace should return zero if not attached
    if(ptrace(PTRACE_TRACEME, 0, 0L, 0L) < 0) {
        puts("ptrace was already attached.");
        return EXIT_FAILURE;
    }

    // Second ptrace should return -1 since we are waiting
    // for parent to attach
    if(ptrace(PTRACE_TRACEME, 0, 0L, 0L) != -1) {
        puts("Second ptrace returned spurious success.");
        return EXIT_FAILURE;
    }

    puts("Client running normally.");
    return EXIT_SUCCESS;
}
