CC = g++ -fpermissive -O4

CODE = linearprobe.c cuckoo.c tabulation.c test.c
EXE = test-performance

all: test-performance

test-performance: $(CODE)
	$(CC) $(CODE) -o $(EXE)

test: test-performance
	./$(EXE)
