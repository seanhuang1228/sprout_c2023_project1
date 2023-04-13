CC := g++
S := src
T := bin
EXE := $T/main.exe
OBJ := $S/main.o $S/splendor.o
FLAG := --std=c++11
MODE := normal

ifeq ($(MODE), ai)
	FLAG += -DAI
	OBJ += $S/ai.o
endif

all: $(OBJ)
	-mkdir bin
	$(CC) -o $(EXE) $(OBJ) $(FLAG)

%.o: %.cpp
	$(CC) -c $^ -o $@ $(FLAG)

clean:
	rm -rf $(OBJ) $(EXE) $S/ai.o
