SRCS 	= functions/main.c functions/ft_load_save.c functions/ft_blur.c functions/ft_brightness.c functions/ft_contrast.c functions/ft_mirror.c functions/ft_pixelize.c functions/ft_rotate.c functions/ft_scale.c functions/ft_threshold.c functions/ft_translate.c functions/ft_choix.c functions/ft_welcome.c

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