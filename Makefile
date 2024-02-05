FILES 	= modify_image.c

OBJS 	= ${FILES:.c=.o}

NAME 	= modify_images

${NAME}: ${OBJS}
	gcc -o ${NAME} ${OBJS}

clean:
	rm -f modify_images