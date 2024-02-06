SRCS 	= modify_images.c

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

ffclean: 	fclean
			${RM} image_*
			
re:			fclean all

.PHONY:		all clean fclean ffclean re