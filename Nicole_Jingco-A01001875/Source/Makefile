CC=gcc -Wall -g
CLIB = -lpthread -lrt
MW = `pkg-config --cflags --libs MagickWand`
SSL = -lssl -lcrypto

all: dcstego  

dcstego: dcstego.o dcimage.o dcutils.o helper.o
	$(CC) -o dcstego dcstego.o dcimage.o dcutils.o helper.o $(MW) $(SSL)

dcutils: dcutils.o
	$(CC) -o dcutils dcutils.o $(MW) $(SSL)

dcimage: dcimage.o
	$(CC) -o dcimage dcimage.o $(MW)

helper: helper.o
	$(CC) -o helper helper.o $(MW)

clean:
	rm -f *.o  dcstego dcutils dcimage helper 

dcstego.o:
	$(CC) -c dcstego.c $(MW) $(SSL)

dcutils.o:
	$(CC) -c dcutils.c $(MW) 

dcimage.o:
	$(CC) -c dcimage.c $(MW)

helper.o:
	$(CC) -c helper.c $(MW)