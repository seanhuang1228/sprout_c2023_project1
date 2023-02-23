CC:=g++
exe:=main
obj:=main.o splendor.o
flag:=-Wall

all: $(obj);
	$(CC) -o $(exe) $(obj) $(flag)

%.o: %.c
	$(CC) -c $^ -o $@

clean:
	rm -rf $(obj) $(exe)
