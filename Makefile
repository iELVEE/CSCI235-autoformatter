CXX = g++
CXXFLAGS = -std=c++17 -g -Wall -O2

PROG ?= doxygen-formatter
OBJS = doxygen-formatter.o

all: $(PROG)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(PROG): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

# rm for linux, del for windows
clean:
	del -rf $(EXEC) *.o *.out main 

rebuild: clean all