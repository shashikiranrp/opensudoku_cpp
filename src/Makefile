all: clean run

compile: 
	clang++ -stdlib=libc++ -std=c++11 -c *.cpp

link:	compile
	clang++ -stdlib=libc++ -std=c++11 *o -o sudoku

run: link
	yes 0 | head -n 81 | ./sudoku || ( echo "BUILD FAILED"; exit 1 )

clean:
	rm -f *gch *o sudoku
