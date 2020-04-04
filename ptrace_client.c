#include <sys/ptrace.h>

#include <stdlib.h>
#include <stdio.h>

int main() {
    if(ptrace(PTRACE_TRACEME, 0, 0L, 0L) < 0) {
        puts("ptrace was already attached!");
        return EXIT_FAILURE;
    }
    puts("Testing");
    return EXIT_SUCCESS;
}
