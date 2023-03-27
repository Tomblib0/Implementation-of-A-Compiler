CC=gcc
FLEX=flex
BISON=bison
ip:
		$(BISON) -d syntax.y
		$(FLEX) lex.l
		$(CC) lex.yy.c syntax.tab.c test_c.c -lfl -o splc
clean:
		@rm -f lex.yy.c syntax.tab.c syntax.tab.h *.out
.PHONY: splc