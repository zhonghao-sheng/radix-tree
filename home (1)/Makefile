all: dict1 dict2 dict3

# The first target:
dict1: driver.o data.o list.o array.o radix.o
	gcc -o dict1 driver.o data.o list.o array.o radix.o

# The second target:
dict2: driver.o data.o list.o array.o radix.o
	gcc -o dict2 driver.o data.o list.o array.o radix.o
	
# The third target:
dict3: driver.o data.o list.o array.o radix.o
	gcc -o dict3 driver.o data.o list.o array.o radix.o

# Other targets specify how to get the .o files
driver.o: driver.c
	gcc -g -c driver.c 

data.o: data.c data.h  
	gcc -g -c data.c 

list.o: list.c list.h
	gcc -g -c list.c

array.o: array.c array.h
	gcc -g -c array.c

radix.o: radix.c radix.h
	gcc -g -c radix.c

clean:
	rm -f *.o dict1 dict2 dict3

