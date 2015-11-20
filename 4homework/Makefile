COMPILER = gcc

all:
	@clear
	@$(COMPILER) -o Tina.o Tina.c

clean:
	@echo cleaning...
	@rm *.o
	@rm cookie.h
	@rm cookie_clnt.c
	@rm cookie_svc.c
	@rm cookie_xdr.c
	@clear
	@echo clean

rpc:
	@rpcgen cookie.x
	@clear
	@echo generated
