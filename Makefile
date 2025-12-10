# libasm Makefile
NAME    = libasm.a

SRCS    = ft_strlen.s \
          ft_strcpy.s \
          ft_strcmp.s \
          ft_strdup.s \
          ft_read.s  \
          ft_write.s

OBJS    = $(SRCS:.s=.o)

NASM    = nasm
NASMFLAGS = -f elf64
AR      = ar
ARFLAGS = rcs
RM      = /bin/rm -f

all: $(NAME)

$(NAME): $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

%.o: %.s
	$(NASM) $(NASMFLAGS) -o $@ $<

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
