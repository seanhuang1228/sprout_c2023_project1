CC:=g++
exe:=main.out
obj:=main.o splendor.o
flag:=-Wall

all: $(obj);
	$(CC) -o $(exe) $(obj) $(flag)

%.o: %.c
	$(CC) -c $^ -o $@

clean:
	rm -rf $(obj) $(exe)
