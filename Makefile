FILES 	= modify_images.c

OBJS 	= ${FILES:.c=.o}

NAME 	= modify_images

${NAME}: ${OBJS}
	gcc -o ${NAME} ${OBJS}

clean:
	rm -f modify_images