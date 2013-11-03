CC	:=	gcc
CFLAGS	:=	-Wall
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


clean:
	$(RM) $(OBJS)
distclean:
	$(RM) $(TARGET)
#	$(RM) TAGS Makefile config.status
.PHONY: clean dist-clean
