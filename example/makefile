ui: ui.o attaavv.o
	gcc -o ui ui.o attaavv.o

ui.o: ./src/ui.c ./src/attaavv.h
	gcc -Wall -W -ansi -pedantic -c ./src/ui.c

attaavv.o: ./src/attaavv.c ./src/attaavv.h
	gcc -Wall -W -ansi -pedantic -c ./src/attaavv.c        
