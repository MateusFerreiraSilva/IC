compressed_array.o:
	g++ -c -g ./src/compressed_array.cpp
	mv compressed_array.o ./resources

sample_pointers.o:
	g++ -c -g ./src/sample_pointers.cpp
	mv sample_pointers.o ./resources


compressed_bitvector.o:
	g++ -c -g ./src/compressed_bitvector.cpp
	mv compressed_bitvector.o ./resources

wavelet_tree.o:
	g++ -c -g ./src/wavelet_tree.cpp
	mv wavelet_tree.o ./resources

main.o:
	g++ -c -g ./src/main.cpp
	mv main.o ./resources

clean:
	rm ./resources/*.o	
	rm prog

all: compressed_array.o sample_pointers.o compressed_bitvector.o wavelet_tree.o main.o

build: all
	g++ -o prog ./resources/*.o

run: build
	./prog
