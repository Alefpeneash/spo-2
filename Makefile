build: mycp.c
	gcc -o mycp mycp.c

clean:
	rm mycp

debug: mycp.c
	gcc -g -o mycp mycp.c
