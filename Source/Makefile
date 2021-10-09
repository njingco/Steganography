CC=gcc -Wall -g
CLIB = -lpthread -lrt
MW = `pkg-config --cflags --libs MagickWand`

all: dcstego  

dcstego: dcstego.o dcimage.o dcutils.o
	$(CC) -o dcstego dcstego.o dcimage.o dcutils.o $(MW)

dcutils: dcutils.o
	$(CC) -o dcutils dcutils.o 

dcimage: dcimage.o
	$(CC) -o dcimage dcimage.o $(MW)

clean:
	rm -f *.o  dcstego dcutils dcimage

dcstego.o:
	$(CC) -c dcstego.c $(MW)

dcutils.o:
	$(CC) -c dcutils.c

dcimage.o:
	$(CC) -c dcimage.c $(MW)
