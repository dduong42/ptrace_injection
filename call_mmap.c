#include <sys/mman.h>
#include <sys/ptrace.h>
#include <sys/user.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ptrace_injector.h"
#define SYS_MMAP 9

void *call_mmap(int pid, void *dst)
{
	struct user_regs_struct regs;
	
	if (ptrace(PTRACE_GETREGS, pid, &regs, &regs) < 0)
	{
		perror("ptrace getregs");
		exit(1);
	}
	// 0f 05 = syscall
	copy_into_mem(pid, 2, dst, "\x0f\x05");
	regs.rax = SYS_MMAP;
	regs.rdi = 0;
	regs.rsi = 4096;
	regs.rdx = PROT_READ | PROT_EXEC;
	regs.r10 = MAP_PRIVATE | MAP_ANONYMOUS;
	regs.r8 = -1;
	regs.r9 = 0;
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
	if (ptrace(PTRACE_GETREGS, pid, &regs, &regs) < 0)
	{
		perror("ptrace getregs");
		exit(1);
	}
	return ((void *) regs.rax);
}