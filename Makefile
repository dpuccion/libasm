# ASM=nasm
# ASFLAGS=-f elf64
# LDFLAGS=
# EXECUTABLE=test

# SRCS	=	ft_strlen.s				\

# OBJS	=   $(SRCS:.s=.o)
         
# all: $(SRCS) $(EXECUTABLE)
                 
# $(EXECUTABLE): $(OBJS)
# 		$(LD) $(LDFLAGS) $(OBJS) -o $@
                                              
# %.o: %.s
# 		$(ASM) $(ASFLAGS) $< -o $@





NAME 	= 	libasm.a

SRCS	=	ft_strlen.s \
			ft_strcpy.s \
			ft_strcmp.s \

OBJS	=   $(SRCS:.s=.o)

all     :   $(NAME)

$(NAME) :   $(OBJS)
	ar rcs $(NAME) $(OBJS)

%.o: %.s
	nasm -f elf64 -o $@ $<

clean   :
	/bin/rm -f $(OBJS)

fclean  :   clean   
	/bin/rm -f $(NAME)

re      :   fclean all

.PHONY: all clean fclean re
