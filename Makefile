CC:=g++
exe:=main.out
obj:=main.o splendor.o
flag:=-Wall
test_mode:=-DTEST

all: $(obj);
	$(CC) -o $(exe) $(obj) $(flag)

test: $(obj);
	$(CC) -o $(exe) $(obj) $(flag) $(test_mode)

%.o: %.c
	$(CC) -c $^ -o $@

clean:
	rm -rf $(obj) $(exe)
