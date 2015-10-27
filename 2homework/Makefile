COMPILER = gcc

# compile our stuff
all:
	@$(COMPILER) -o account.o account.c linkedlist.c
	@chmod 700 account.o

# remove unecessary files
clean:
	@rm -f account.o

# easier to test
test:
	@clear
	./account.o w 100 w 500 w 300 d 200 w 300 d 600
