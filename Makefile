CXX = gcc
CXXFLAGS = -Wall -Werror -Wextra -pedantic -g -fsanitize=address -std=c18 -pthread
LDFLAGS = -Wall -Werror -Wextra -pedantic -O1 -g -std=c18 -pthread

SRC = main.c
OBJ = $(SRC:.cc=.o)
EXEC = main

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) $(LDFLAGS) -o $@ $(OBJ) $(LBLIBS)

clean:
	rm -rf $(EXEC) debug

debug:
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ) $(LBLIBS)