# ptrace wrapper practice
This repo is for practicing build ptrace wrappers to defeat varying levels of
ptrace-based anti-debugging techniques.

## Structure
Each particular approach to anti-debugging with ptrace is implemented in a
`ptrace-client` program, and has an associated `ptrace-wrapper.so` library that
should be used by setting `LD_PRELOAD`.

The goal with these wrappers is to be able to run `strace` or `gdb` on a process to
help with the static analysis necessary to do more persistent defeats of the
anti-debugging code (e.g. NOPing the ptrace calls). Depending on how intense the
obfuscation is for the code, it might be easier with some dynamic analysis as well.


## Levels
The different anti-debugging implementations are organized into levels,
hopefully sorted in order of increasing complication and/or difficulty to
circumvent with a ptrace wrapper.

### Level 01
This is the simplest anti-debugging technique with ptrace, just calling
`ptrace()` once and checking for a negative return value to indicate the
presence of an attached debugger.

### Level 02
This is slightly more involved than the previous level, it calls `ptrace()` a
second time and checks that this second call fails because of the first attach.

The code for this was inspired by [a blog post from Sebastian Auberger][0].


## Building
The examples build with GCC 9.3.0 on my Arch Linux machine. There should be no
dependencies outside of libdl which comes with glibc.

The `Makefile` also has some default tests which run the client program with
`strace`, first without the wrapper (to confirm that the client detects it) and
again with the `LD_PRELOAD` set so you can check whether the wrapper is working.

#### Build and test everything
Just run `make` or `make all`.

#### Specific levels
* Build and test: `make level-NN`
* Just build: `make level-NN-build`
* Just test: `make level-NN-test`

#### Cleaning
* Everything: `make clean`
* Specific level: `make level-NN-clean`



[0]: https://seblau.github.io/posts/linux-anti-debugging
