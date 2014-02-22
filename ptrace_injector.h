#ifndef PTRACE_INJECTOR_H
# define PTRACE_INJECTOR_H

# include <sys/user.h>

void copy_into_mem(int pid, long size, void *dst, void *src);
void *call_mmap(int pid, void *dst);
void *inject_payload(int pid, long size, void *dst, void *payload);
void call_payload(int pid, void *dst, void *addr_payload, struct user_regs_struct *saved_regs, long saved_dst);

#endif