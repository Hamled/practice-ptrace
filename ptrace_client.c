#include <sys/ptrace.h>

#include <stdlib.h>
#include <stdio.h>

int main() {
    ptrace(PTRACE_TRACEME, 0, 0L, 0L);
    puts("Testing");
    return EXIT_SUCCESS;
}
