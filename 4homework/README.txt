Assignment 6: RPC Programming

The (Revised) Cookie Jar Problem

Three Processes:
    1. Sister Tina (client)
    2. Sister Judy (client)
    3. Mother       (server)

Tina and Judy both request cookies from their mother at random times. The mother
will give Tina a cookie whenever she want, Judy will only get a cookie after 
Tina has already recieved two cookies. The mother has a cookie jar with 20 
cookies in it. When the cookies run out if either sister requests cookies the 
mother will tell them "no cookies for you" and they will stop requesting 
cookies.

To run this:
    1. Copy all files to three servers.
    2. run "make" on all three servers.
    3. run "Mother.o" on a server.
    4. run "Judy.o" and "Tina.o" passing in the address of the mothers server
    5. WATCH THE COOKIES
