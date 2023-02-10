all:	x

x:	main.o
	g++ -o x main.o
	clear
	./x

main.o:	main.cpp QueueT.h QFunctions.h
	g++ -c main.cpp

clean:	
	rm -f x *.o

