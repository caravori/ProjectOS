CXX = gcc
CXXFLAGS = -Wall -Werror -Wextra -pedantic -g -fsanitize=address
LDFLAGS =  

SRC = main.c
OBJ = $(SRC:.cc=.o)
EXEC = main

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) $(LDFLAGS) -o $@ $(OBJ) $(LBLIBS)

clean:
	rm -rf $(EXEC)