CC = g++
CCFLAGS = -lgtest -lpthread -Wall -Wextra -Wno-unused-parameter -O2 -g

LIB_SRC = $(wildcard ../lib/*.h) $(wildcard ../lib/*.c)
TEST_OBJ = $(patsubst %.cpp, %.o, $(wildcard *.cpp))
MAIN_TEST = main_test

all: $(TEST_OBJ) $(MAIN_TEST)

%.o: %.cpp $(LIB_SRC)
	$(CC) -c $< -o $@ $(CCFLAGS)

$(MAIN_TEST): $(TEST_OBJ)
	$(CC) $(TEST_OBJ) -o $@ -lgtest_main $(CCFLAGS)

test: $(MAIN_TEST)
	@./$(MAIN_TEST) --gtest_repeat=1

clean:
	@rm -rf $(TEST_OBJ) $(MAIN_TEST)

.PHONY: test clean
