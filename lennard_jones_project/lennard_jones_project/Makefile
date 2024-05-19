CC = g++
CFLAGS = -Wall -std=c++11
LDFLAGS =

SRCDIR = .
INCDIR = include
TESTDIR = .

SRCS = $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(SRCS:.cpp=.o)
DEPS = $(wildcard $(INCDIR)/*.h)
TESTS = $(TESTDIR)/tests.cpp

EXEC = my_program

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(EXEC) $(LDFLAGS)

$(OBJS): $(SRCS) $(DEPS)
	$(CC) $(CFLAGS) -c $(SRCDIR)/$*.cpp -o $@

tests: $(TESTS)
	$(CC) $(CFLAGS) $(TESTS) $(SRCS) -o test/test_suite $(LDFLAGS)

clean:
	rm -f $(SRCDIR)/*.o $(EXEC) test/test_suite

