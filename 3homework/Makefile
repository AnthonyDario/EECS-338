# gcc is my favorite compiler
COMPILER = gcc

# let's make!
all:
	@clear
	@$(COMPILER) -pthread -o readers-writers.o readers-writers.c
	@chmod 700 readers-writers.o

# I hate extra files
clean:
	@rm -f *.o

# I hate typing
test:
	@./readers-writers.o
