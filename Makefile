SRCS 	= main.c ft_choix.c ft_welcome.c ft_images.c ft_load_save.c

OBJS 	= ${SRCS:.c=.o}

NAME 	= modify_images

CC		= gcc
RM		= rm -f

CFLAGS	= -Wall -Wextra -Werror -g

${NAME}:	${OBJS}
			${CC} ${CFLAGS} -o ${NAME} ${OBJS}

all:		${NAME}

clean:
			${RM} ${OBJS}

fclean:		clean
			${RM} ${NAME}
			
re:			fclean all

.PHONY:		all clean fclean ffclean re