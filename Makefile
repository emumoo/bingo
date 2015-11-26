all:
	gcc generate_bingo.c -o run

clean:
	rm bingo*.log bingo*.aux bingo*.pdf bingo*.tex run tex_files/*