# Used "TA's Makefile Example" imsample.zip provided under "make help" module
# as a reference to write this one.

PROJ = genetic

CXX = g++

SRC += chromosome.cpp
SRC += population.cpp
TSRC := $(SRC)
TSRC += testChromosome.cpp
SRC += geneticTSP.cpp

OBJ = $(SRC:.cpp=.o)
TOBJ = $(TSRC:.cpp=.o)

OUT = main
TEST = test

CFLAGS = -g -Wall -pedantic --std=c++11

default: clean $(OUT)

$(OUT): $(OBJ)
	@echo "CC	$@"
	@$(CXX) $(CFLAGS) $^ -o $@
	@echo "RM	*.o"
	@rm -f *.o

$(TEST): $(TOBJ)
	@echo "CC    $@"
	@$(CXX) $(CFLAGS) $^ -o $@
	@echo "RM  *.o"
	@rm -f *.o

%.o: %.cpp
	@echo "CC	$^"
	@$(CXX) $(CFLAGS) -c $^

zip:
	zip $(PROJ).zip *.cpp *.hpp makefile

clean: $(CLEAN)
	@echo "RM	*.o"
	@echo "RM	$(OUT)"
	@rm -f *.o  $(OUT)
	@echo "RM   $(TEST)"
	@rm -f *.o  $(TEST)
