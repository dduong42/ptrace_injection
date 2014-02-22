#include <sys/ptrace.h>
#include <sys/user.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ptrace_injector.h"

void call_payload(int pid, void *dst, void *addr_payload, struct user_regs_struct *saved_regs, long saved_dst)
{
	struct user_regs_struct regs;
	
	memcpy(&regs, saved_regs, sizeof(regs));
	// ff e0 = jmp rax
	copy_into_mem(pid, 2, dst, "\xff\xe0");
	regs.rax = (long) addr_payload;
	regs.rsp -= 8;
	copy_into_mem(pid, 8, (void *) regs.rsp, (void *) &regs.rip);
	regs.rip = (long) dst;
	if (ptrace(PTRACE_SETREGS, pid, &regs, &regs) < 0)
	{
		perror("ptrace setregs");
		exit(1);
	}
	if (ptrace(PTRACE_SINGLESTEP, pid, 0, 0) < 0)
	{
		perror("ptrace singlestep");
		exit(1);
	}
	waitpid(pid, 0, 0);
	copy_into_mem(pid, 2, dst, (void *) &saved_dst);
}