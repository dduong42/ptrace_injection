#include <stdlib.h>
#include <sys/mman.h>
#include <stdio.h>
#include "ptrace_injector.h"

void *inject_payload(int pid, long size, void *dst, void *payload)
{
	void *ret_mmap;
	
	ret_mmap = call_mmap(pid, dst);
	if (ret_mmap == MAP_FAILED)
	{
		fprintf(stderr, "mmap has failed :(\n");
		exit(1);
	}
	copy_into_mem(pid, size, ret_mmap, payload);
	printf("[*] The payload is located at %p\n", ret_mmap);
	return (ret_mmap);
}