CC 	:= gcc
SRC := src/*.c
build: $(SRC)
	gcc -Wall -Wextra --std=gnu99 -pedantic -Werror -pthread -o mycp $(SRC)

clean:
	rm mycp

debug: $(SRC)
	gcc -Wall -Wextra --std=gnu99 -pedantic -Werror -pthread -g -o mycp $(SRC) 
