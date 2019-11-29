SRC = src/

build: $(SRC)
	gcc -Wall -Wextra --std=gnu99 -pedantic -Werror -pthread -o mycp $(SRC)/*.c

clean:
	rm mycp

debug: mycp.c
	gcc -Wall -Wextra --std=gnu99 -pedantic -Werror -pthread -g -o mycp src/*.c 
