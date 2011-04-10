CC = gcc
CFLAGS = -g -Wall
tmux-mem-cpu-load : tmux-mem-cpu-load.c
	$(CC) $(CFLAGS) -o tmux-mem-cpu-load tmux-mem-cpu-load.c

clean:
	rm -f tmux-mem-cpu-load
