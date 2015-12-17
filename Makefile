all:
	gcc -O0 generate_bingo.c -o run

clean:
	rm *.tex pdfs/* run log.txt