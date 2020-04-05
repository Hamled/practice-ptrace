##
# ptrace practice
#
# @file
# @version 0.1

CC=gcc
CFLAGS=-Wall -Wextra -Werror
WRAPPER_FLAGS=-Iinclude -D_GNU_SOURCE -fPIC -ldl -shared

LEVELS = level-01

.PHONY: all
all: build test

define LEVEL_template =

$(1)/ptrace-client: $(1)/client.c
	$(CC) $(CFLAGS) -o $$@ $$<

$(1)/ptrace-wrapper.so: $(1)/wrapper.c
	$(CC) $(CFLAGS) $(WRAPPER_FLAGS) -o $$@ $$<

$(1): $(1)-build $(1)-test

$(1)-build: $(1)/ptrace-client $(1)/ptrace-wrapper.so

$(1)-test: $(1)
	@echo
	@echo '===Testing $(1) without wrapper'
	@echo '======program output:'
	@strace -e trace=write,ptrace -o ./strace_out ./$(1)/ptrace-client || true
	@echo
	@echo '======strace output:'
	@cat ./strace_out
	@rm -f ./strace_out

	@echo
	@echo
	@echo '===Testing $(1) with wrapper'
	@echo '======program output:'
	@strace -e trace=write,ptrace -o ./strace_out env LD_PRELOAD=./$(1)/ptrace-wrapper.so ./$(1)/ptrace-client || true
	@echo
	@echo '======strace output:'
	@cat ./strace_out
	@rm -f ./strace_out

$(1)-clean:
	rm -f $(1)/ptrace-client $(1)/ptrace-wrapper.so

endef

$(foreach level,$(LEVELS),$(eval $(call LEVEL_template,$(level))))

build: $(foreach level,$(LEVELS),$(level)-build)
test: $(foreach level,$(LEVELS),$(level)-test)
clean: $(foreach level,$(LEVELS),$(level)-clean)

# end
