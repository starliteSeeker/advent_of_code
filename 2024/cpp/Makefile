SRCS = $(wildcard src/*.cpp)
EXES = $(patsubst src/%.cpp,%,$(SRCS))
FLAGS = -Wall -Wextra -Wpedantic -Wunused -O2 -std=c++17

.PHONY: all clean

all: $(EXES)

%: src/%.cpp
	clang++ $(FLAGS) $< -o $@

compile_commands.json: Makefile
	$(MAKE) clean
	bear -- $(MAKE)

clean:
	rm -f $(EXES) compile_commands.json

