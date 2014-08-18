
CC		=	g++
CFLAGS		=	-g -W -Wall -ansi -pedantic
LDFLAGS		=	-lpthread -lboost_thread-mt
EXEC		=	test
SRC		=	$(wildcard *.cpp */*.cpp)
OBJ		=	$(SRC:.cpp=.o)

all:	$(OBJ)
	$(CC) -o $(EXEC) $^ $(LDFLAGS)

%.o:	%.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -f */*.o *.o
	rm -f */*~ *~

fclean:	clean
	rm -f $(EXEC)

re:	fclean all
