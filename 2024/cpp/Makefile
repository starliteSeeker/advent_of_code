SRCS = $(wildcard src/*.cpp)
EXES = $(patsubst src/%.cpp,%,$(SRCS))
FLAGS = -Wall -Wextra -Wpedantic -Wunused -O2

.PHONY: all clean

all: $(EXES)

%: src/%.cpp
	clang++ $(FLAGS) -stdlib='libc++' $< -o $@

compile_commands.json: Makefile
	$(MAKE) clean
	bear -- $(MAKE)

clean:
	rm -f $(EXES) compile_commands.json

