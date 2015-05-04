all: run

compile: 
	clang++ -stdlib=libc++ -std=c++11 -c *

link:	compile
	clang++ -stdlib=libc++ -std=c++11 *o -o sudoku

run: link
	./sudoku

clean:
	rm -f *gch *o sudoku
