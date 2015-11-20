COMPILER = gcc

all:
	@clear
	@rpcgen cookie.x
	@clear
	@echo generated
	@$(COMPILER) -o Tina.o Tina.c
	@$(COMPILER) -o Judy.o Judy.c
	@$(COMPILER) -o Mother.o Mother.c

clean:
	@echo cleaning...
	@rm *.o
	@rm cookie.h
	@rm cookie_clnt.c
	@rm cookie_svc.c
	@rm cookie_xdr.c
	@clear
	@echo clean
