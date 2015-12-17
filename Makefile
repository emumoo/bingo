all:
	gcc generate_bingo.c -o run

clean:
	rm *.tex pdfs/* run log.txt