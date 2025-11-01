stego = $(patsubst %.c, %.o, $(wildcard *.c))
stego.out : $(stego)
	gcc -o $@ $^
clean :
	rm *.out *.o
