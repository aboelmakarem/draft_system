# Declaration of variables
CC = g++
CC_FLAGS = -O2

# File names
EXEC = dc
SOURCES = Token.cpp \
	Scanner.cpp \
	Parser.cpp \
	Driver.cpp 

OBJECTS = $(SOURCES:.cpp=.o)

# Main target
$(EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXEC)


# To obtain object files
%.o: %.cpp
	$(CC) -c $(CC_FLAGS) $< -o $@


# To remove generated files
clean:
	rm -f $(EXEC) ./*.o



