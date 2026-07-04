run:
	g++ main.cpp -std=c++20 -I . -Werror -Wall -Wextra -Wpedantic -Wreorder -Wunused-result
	./a.out
	rm a.out
