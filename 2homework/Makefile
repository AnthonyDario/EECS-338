COMPILER = gcc

# compile our stuff
all:
	@clear
	@$(COMPILER) -o account.o account.c
	@chmod 700 account.o

# remove unecessary files
clean:
	@rm -f account.o

# easier to test
test:
	@clear
	./account.o 500 w 100 w 500 w 300 d 200 w 300 d 600
