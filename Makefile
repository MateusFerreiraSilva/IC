compressed_array.o:
	g++ -c ./src/compressed_array.cpp
	mv compressed_array.o ./resources

sample_pointers.o:
	g++ -c ./src/sample_pointers.cpp
	mv sample_pointers.o ./resources


compressed_bitvector.o:
	g++ -c ./src/compressed_bitvector.cpp
	mv compressed_bitvector.o ./resources

clean:
	rm ./resources/*.o	

all: compressed_array.o sample_pointers.o compressed_bitvector.o
