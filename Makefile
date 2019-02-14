build: TestHash.c hash.c lista.h materie.h student.h
	gcc TestHash.c hash.c -Wall -o hash_table
clean:
	rm -fr hash_table
