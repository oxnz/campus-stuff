CC	=	gcc
FLAGS	=	-Wall
OBJS	=	main.o \
		database.o \
		new.o \
		person.o \
		student.o \
		common.o \
		utility.o

TARGET	=	stu-manager
$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(FLAGS)
%.c.o:
	$(CC) -c
.PHONY: clean

clean:
	-rm $(OBJS)
