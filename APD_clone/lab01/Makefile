build:
	gcc example.c -o example -lpthread
	gcc add_serial.c -o add_serial -lpthread
build_debug:
	gcc example.c -o example -lpthread -DDEBUG -g3 -O0 -Werror -Wall
	gcc add_serial.c -o add_serial -DDEBUG -g3 -O0 -Werror -Wall
clean:
	rm example add_serial