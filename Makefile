SRCS = copy_into_mem.c inject_payload.c call_mmap.c call_payload.c
OBJS = $(SRCS:.c=.o)

NAME = libinject.a
EXAMPLES = example/target example/injector

CC = gcc
CFLAGS = -Werror -Wall -Werror

all: $(NAME)

example: $(EXAMPLES)

debug: CFLAGS += -g
debug: re

example/target: example/target.c
	$(CC) $(CFLAGS) $< -o $@

example/injector: example/injector.c $(NAME)
	$(CC) $(CFLAGS) $^ -o $@	

$(NAME): $(OBJS)
	ar rcs $@ $^

clean:
	$(RM) $(OBJS)
	
fclean: clean
	$(RM) $(NAME) $(EXAMPLES)
	
re: fclean all