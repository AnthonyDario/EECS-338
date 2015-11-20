COMPILER = gcc
CLIENT = cookie_clnt.c
XDR = cookie_xdr.c
SVC = cookie_svr.c

all:
	@clear
	@rpcgen cookie.x
	@clear
	@echo generated
	@$(COMPILER) -o Tina.o Tina.c $(CLIENT) $(XDR) -lnsl
	@$(COMPILER) -o Judy.o Judy.c $(CLIENT) $(XDR) -lnsl
	@$(COMPILER) -o Mother.o Mother.c $(SVC) $(XDR) -lnsl

clean:
	@echo cleaning...
	@rm cookie.h
	@rm cookie_clnt.c
	@rm cookie_svc.c
	@rm cookie_xdr.c
	@rm *.o
	@clear
	@echo clean
