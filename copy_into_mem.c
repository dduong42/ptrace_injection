#include <sys/ptrace.h>
#include <stdio.h>
#include <stdlib.h>

void copy_into_mem(int pid, long size, void *dst, void *src)
{
	long *ptr_dst, *ptr_src;

	ptr_dst = (long *) dst;
	ptr_src = (long *) src;
	for (; size > 0; size -= sizeof(void *), ++ptr_dst, ++ptr_src)
	{
		if (ptrace(PTRACE_POKETEXT, pid, ptr_dst, (void *)*ptr_src) < 0)
		{
			perror("copy_into_mem");
			exit(EXIT_FAILURE);
		}
	}
}