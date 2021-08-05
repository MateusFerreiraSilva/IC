bitarray.o:
	g++ -c -g ./src/bitarray.cpp
	mv bitarray.o ./resources

sample_pointers.o:
	g++ -c -g ./src/sample_pointers.cpp
	mv sample_pointers.o ./resources

combination.o:
	g++ -c -g ./src/combination.cpp
	mv combination.o ./resources

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

all: bitarray.o sample_pointers.o combination.o compressed_bitvector.o wavelet_tree.o main.o

build: all
	g++ -o prog ./resources/*.o

run: build
	./prog
