Anthony Dario
EECS 338
Fall 2015

This is an implementation of the solution to the savings account problem. This
implementation uses a circular array instead of a linked list due to
complexities with using a linked list in shared memory. 

Running "make" will build the program to ./account.o. You can run the program
with ./account.o <#> <Ww><Dd> <#> ... where the first number represents the
initial balance of the account, and every letter represents a withdrawer or a
depositor. The number following the letter is the amount to be withdrawn or
deposited.

You can see an example below or by running "make test"

EXAMPLE:
./account.0 500 w 100 w 500 w 300 d 200 w 300 d 600

will initalize the balance to 500
send a withdrawer trying to withdraw 100
send a withdrawer trying to withdraw 500
send a withdrawer trying to withdraw 300
send a depositor to deposit 200
send a withdrawer trying to withdraw 300
send a depositor to deposit 600
