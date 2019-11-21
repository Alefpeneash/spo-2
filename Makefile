build: mycp.c
	gcc -Wall -Wextra --std=gnu99 -pedantic -Werror -pthread -o mycp mycp.c

clean:
	rm mycp

debug: mycp.c
	gcc -Wall -Wextra --std=gnu99 -pedantic -Werror -pthread -g -o mycp mycp.c
