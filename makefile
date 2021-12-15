PROJ_NAME=prog

CC=g++

CPP_SOURCE=$(wildcard ./src/*.cpp)

H_SOURCE=$(wildcard ./libs/*.h)

OBJ=$(subst .cpp,.o,$(subst src,objects,$(CPP_SOURCE)))

CPP_FLAGS=-c -g -Wall

LDLIBS=-ldl

RM = rm -rf

all: objFolder $(PROJ_NAME)

$(PROJ_NAME): $(OBJ)
	@ echo 'Building binary using G++ linker: $@'
	$(CC) $^ -o $@ $(LDLIBS)
	@ echo 'Finished building binary: $@'
	@ echo ' '

# testar se funciona sem .h
./objects/%.o: ./src/%.cpp ./libs/%.h
	@ echo 'Building target using G++ compiler: $<'
	$(CC) $< $(CPP_FLAGS) -o $@
	@ echo ' '

# tirar H_SOURCE e testar
./objects/main.o: ./src/main.cpp $(H_SOURCE)
	@ echo 'Building target using GPP compiler: $<'
	$(CC) $< $(CPP_FLAGS) -o $@
	@ echo ' '

objFolder:
	@ mkdir -p objects

clean:
	@ $(RM) ./objects/*.o $(PROJ_NAME) *~
	@ rmdir objects

.PHONY: all clean