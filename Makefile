CFLAGS=-I./tlib -I./people/

pucpeopple:
	gcc -o main \
  tlib/*.c people/*.c \
	main.c \
	$(CFLAGS)
