CC=gcc -Wall -g
CLIB = -lpthread -lrt
MW = `pkg-config --cflags --libs MagickWand`

all: stego dcstego dcutils dcimage 

stego: stego.o 
	$(CC) -o stego stego.o 

dcstego: dcstego.o
	$(CC) -o dcstego dcstego.o 

dcutils: dcutils.o
	$(CC) -o dcutils dcutils.o 

dcimage: dcimage.o
	$(CC) -o  dcimage dcimage.o $(MW)

clean:
	rm -f *.o  stego dcstego dcutils dcimage
	
stego.o:
	$(CC) -c stego.c

dcstego.o:
	$(CC) -c dcstego.c

dcutils.o:
	$(CC) -c dcutils.c

dcimage.o:
	$(CC) -c dcimage.c
