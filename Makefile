COMPILER = gcc
FLAGS = -Wall -g 

all: Project2main.out 
Project2main: Project2main.out 


Project2main.o: Project2main.c 
	$(COMPILER) $(FLAGS) -c Project2main.c

Project2main.out: Project2main.o
	$(COMPILER) $(FLAGS) -o Project2main.out Project2main.o -lpthread


clean:
	rm -f *.o *.out