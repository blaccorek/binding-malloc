CC	=	gcc

RM	=	rm -rf

LN	=	ln -fs

MK	=	mkdir -p

CFLAGS	+=	-W -Wall -Werror -Wextra
CFLAGS	+=	-I./include

LDFLAGS	=

SRCDIR	=	src
SRC		=	$(addprefix $(SRCDIR)/, $(SRCFILE))
SRCFILE	=	my_malloc.c\
			my_free.c\
			mutual.c\
			my_realloc.c\
			my_calloc.c

OBJDIR	=	obj
OBJ		=	$(addprefix $(OBJDIR)/, $(notdir $(SRC:.c=.o)))

NAME		=	libmy_malloc_$(shell uname).so
LINKNAME	=	libmy_malloc.so

all:		$(NAME)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ -fPIC

$(OBJDIR):
	$(MK) $@

$(NAME):	$(OBJDIR) $(OBJ)
	$(CC) $(OBJ) -o $@ -shared $(LDFLAGS);
	$(LN) $@ $(LINKNAME)

clean:
	$(RM) $(OBJDIR)

fclean:		clean
	$(RM) $(NAME) $(LINKNAME)

re:		fclean all

.PHONY:		all clean fclean re
