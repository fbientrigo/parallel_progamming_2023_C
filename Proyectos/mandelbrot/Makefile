# Compiler options
CXX = g++
CXXFLAGS = -Isrc/Include
LDFLAGS = -Lsrc/lib
LDLIBS = -lmingw32 -lSDL2main -lSDL2

# Source files, incluye aqui otros .cpp
SRCS = main.cpp
OBJS = $(SRCS:.cpp=.o)
EXEC = main

# Default rule
all: $(EXEC)

# Rule to build the target
$(EXEC): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

# Rule to build object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule
#rm -f $(OBJS) $(EXEC) for linux
clean:
	del -f $(OBJS) $(EXEC).exe
