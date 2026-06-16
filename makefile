all : gpm
	./gpm < inp > out
	cmp out exp
	./gpm < forth.inp > forth.out
	cmp forth.out forth.exp
clean:
	rm -f gpm out forth.out
