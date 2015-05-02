CFLAGS=-I./tlib -I./people/

pucpeople:
	gcc -o main \
  tlib/*.c people/*.c \
	main.c \
	$(CFLAGS)
