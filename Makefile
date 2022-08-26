build:
	gcc gui.c -o gui -lncurses -lm
run:
	./gui img1
clean:
	rm gui