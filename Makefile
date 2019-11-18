build: mycp.c
	gcc -Wall -Wextra --std=gnu99 -pedantic -Werror -o mycp mycp.c

clean:
	rm mycp

debug: mycp.c
	gcc -Wall -Wextra --std=gnu99 -pedantic -Werror -g -o mycp mycp.c
