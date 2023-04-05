CC:=g++
exe:=main.out
obj:=main.o splendor.o
flag:=-Wall

all: $(obj)
	$(CC) -o $(exe) $(obj) $(flag)

%.o: %.c
	$(CC) -c $^ -o $@

ai:
	$(CC) -o $(exe) splendor.hpp main.cpp splendor.cpp ai.cpp $(flag) -DAI

clean:
	rm -rf $(obj) $(exe)
