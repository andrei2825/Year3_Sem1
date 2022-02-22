# APD - Tema 1
# Octombrie 2021
# -Werror
build:
	@echo "Building..."
	@gcc -pthread -o tema1_par tema1_par.c genetic_algorithm_par.c -lm -Wall
	@gcc -o tema1 tema1.c genetic_algorithm.c -lm -Wall
	@echo "Done"

build_debug:
	@echo "Building debug..."
	@gcc -pthread -o tema1_par tema1_par.c genetic_algorithm_par.c -lm -Wall -O0 -g3 -DDEBUG
	@gcc -o tema1 tema1.c genetic_algorithm.c -lm -Wall -O0 -g3 -DDEBUG
	@echo "Done"

clean:
	@echo "Cleaning..."
	@rm -rf tema1_par
	@rm -rf tema1
	@echo "Done"
# build:
# 	@echo "Building..."
# 	@gcc -o tema1 tema1.c genetic_algorithm.c -lm -Wall
# 	@echo "Done"

# build_debug:
# 	@echo "Building debug..."
# 	@gcc -o tema1 tema1.c genetic_algorithm.c -lm -Wall -O0 -g3 -DDEBUG
# 	@echo "Done"

# clean:
# 	@echo "Cleaning..."
# 	@rm -rf tema1
# 	@echo "Done"