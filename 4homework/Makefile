COMPILER = gcc
CLIENT = cookie_clnt.c
XDR = cookie_xdr.c
SVC = cookie_svc.c
SERVER = eecslinab3.case.edu

all:
	@clear
	@rpcgen cookie.x
	@clear
	@echo generated
	@$(COMPILER) -o Tina.o Tina.c $(CLIENT) $(XDR) -lnsl
	@$(COMPILER) -o Judy.o Judy.c $(CLIENT) $(XDR) -lnsl
	@$(COMPILER) -o Mother.o Mother.c $(SVC) $(XDR) -lnsl
	@echo compiled

clean:
	@echo cleaning...
	@rm cookie.h
	@rm cookie_clnt.c
	@rm cookie_svc.c
	@rm cookie_xdr.c
	@rm *.o
	@clear
	@echo clean

# this will run the process with mother or linab3, 
# Tina on linab2 and judy on linab7, you need my password for this though
run:
	@echo running mother...
	@ssh -f ard74@eecslinab3.case.edu "./4homework/Mother.o"
	@echo running Judy...
	@ssh -f ard74@eecslinab7.case.edu "./EECS-338/4homework/Judy.o $(SERVER)"
	@echo running Tina...
	@ssh -f ard74@eecslinab2.case.edu "./EECS-338/4homework/Tina.o $(SERVER)"
