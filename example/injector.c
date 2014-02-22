#include <stdlib.h>
#include <sys/ptrace.h>
#include <sys/user.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include "../ptrace_injector.h"

/*
**  Injector side:
**
**  $ ./injector 8748 0x0000000000400000
**  saved_dst: 0x10102464c457f
**  [*] The payload is located at 0x7f924db1c000
**
**  Target side:
**
**  $ ./target
**  I am the target and my pid is: 8748
**  I am the payload :)
**  The end.
*/

char payload[] =
"\x50\x57\x56\x52"
"\xb8\x01\x00\x00\x00\xbf\x01\x00\x00\x00"
"\x48\x8d\x35\x0c\x00\x00\x00\xba\x14\x00"
"\x00\x00\x0f\x05\x5a\x5e\x5f\x58"
"\xc3I am the payload :)\n";

int main(int ac, char **av)
{
	struct user_regs_struct regs;
	void *ret_mmap;
	long saved_dst;
	int pid;
	void *dst;

	if (ac != 3)
	{
		fprintf(stderr, "Usage: %s [pid] [addr]\n", av[0]);
		return (1);
	}
	pid = atoi(av[1]);
	sscanf(av[2], "%p", &dst);
	if (ptrace(PTRACE_ATTACH, pid, 0, 0) < 0)
	{
		perror("ptrace attach");
		return (1);
	}
	waitpid(pid, 0, 0);
	if (ptrace(PTRACE_GETREGS, pid, &regs, &regs) < 0)
	{
		perror("ptrace getregs");
		exit(1);
	}
	if ((saved_dst = ptrace(PTRACE_PEEKTEXT, pid, dst, 0)) == -1)
	{
		perror("ptrace peektext");
		exit(1);
	}
	printf("saved_dst: %#lx\n", saved_dst);
	ret_mmap = inject_payload(pid, sizeof(payload) - 1, dst, payload);
	call_payload(pid, dst, ret_mmap, &regs, saved_dst);
	if (ptrace(PTRACE_DETACH, pid, 0, 0) < 0)
	{
		perror("ptrace detach");
		return (1);
	}
	return (0);
}