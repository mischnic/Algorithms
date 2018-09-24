CC=/usr/local/opt/llvm/bin/clang
CFLAGS=-Ofast -march=native

# CFLAGS=$(CFLAGS) -fsanitize=address
# LDARGS=-fsanitize=address

all: damen
	./damen 8 BegrenzteEnumeration

damen: damen.o
	$(CC) $(LDARGS) $^ -o damen

clean:
	rm -f damen damen.o
