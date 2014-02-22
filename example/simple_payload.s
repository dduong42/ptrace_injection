%define WRITE 1
%define FORK 57
%define EXIT 60

GLOBAL _start:

_start:

push rax
push rdi
push rsi
push rdx

mov rax, WRITE
mov rdi, 1
lea rsi, [rel string]
mov rdx, end_string - string
syscall

end:
pop rdx
pop rsi
pop rdi
pop rax
ret

string:
db "I am the payload :)", 0xa
end_string: