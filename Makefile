FILES 	= modify_image.c

OBJS 	= ${FILES:.c=.o}

NAME 	= modify_image

${NAME}: ${OBJS}
	gcc -o ${NAME} ${OBJS}

clean:
	rm -f modify_image