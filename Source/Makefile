CC=gcc -Wall -g
CLIB = -lpthread -lrt
MW = `pkg-config --cflags --libs MagickWand`

all: dcstego  

dcstego: dcstego.o dcimage.o dcutils.o helper.o
	$(CC) -o dcstego dcstego.o dcimage.o dcutils.o helper.o $(MW) 

dcutils: dcutils.o
	$(CC) -o dcutils dcutils.o $(MW) 

dcimage: dcimage.o
	$(CC) -o dcimage dcimage.o $(MW)

helper: helper.o
	$(CC) -o helper helper.o $(MW)

clean:
	rm -f *.o  dcstego dcutils dcimage helper secret 

dcstego.o:
	$(CC) -c dcstego.c $(MW) 

dcutils.o:
	$(CC) -c dcutils.c $(MW) 

dcimage.o:
	$(CC) -c dcimage.c $(MW)

helper.o:
	$(CC) -c helper.c $(MW)