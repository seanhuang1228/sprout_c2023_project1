CC:=g++
exe:=main.out
obj:=main.o splendor.o
flag:=-Wall

all: $(obj)
	$(CC) -o $(exe) $(obj) $(flag)

%.o: %.c
	$(CC) -c $^ -o $@

test:
	$(CC) -o $(exe) main.cpp splendor.cpp $(flag) -DDEBUG

clean:
	rm -rf $(obj) $(exe)
