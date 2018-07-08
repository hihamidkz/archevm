CC := gcc
CFLAGS := -g -std=c99

main: myterm.o mybigchars.o mysimpc.o myreadkey.o main.o terminal.o
	$(CC) -o $@ $^
	
main.o: main.c
	$(CC) -c $(CFLAGS) $^ -I ~/orgevm/myterm 
	
mybigchars.o: mybigchars/mybigchars.c
	$(CC) -c $(CFLAGS) $^ -I ~/orgevm/myterm
	
myterm.o: myterm/myterm.c
	$(CC) -c $(CFLAGS) $^
	
mysimpc.o: mysimpc/mysimpc.c
	$(CC) -c $(CFLAGS) $^
	
myreadkey.o: myreadkey/myreadkey.c
	$(CC) -c $(CFLAGS) $^
	
terminal.o: terminal.c
	$(CC) -c $(CFLAGS) $^

.PHONY: clean
clean:
	rm *.o
