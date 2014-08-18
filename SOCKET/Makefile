
CC		=	g++
CFLAGS		=	-W -Wall -ansi -pedantic
LDFLAGS		=	-lpthread
EXEC		=	test
SRC		=	$(wildcard *.cpp)
OBJ		=	$(SRC:.cpp=.o)

all:	$(OBJ)
	$(CC) -o $(EXEC) $^ $(LDFLAGS)

%.o:	%.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm *.o

fclean:	clean
	rm $(EXEC)

re:	fclean all
