CC=gcc -Wall -g
CLIB = -lpthread -lrt
MW = `pkg-config --cflags --libs MagickWand`

all: dcstego 
# all: stego dcstego dcutils dcimage 

dcstego: dcstego.o
	$(CC) -o dcstego dcstego.o 

# dcutils: dcutils.o
# 	$(CC) -o dcutils dcutils.o 

# dcimage: dcimage.o
# 	$(CC) -o  dcimage dcimage.o $(MW)

clean:
	rm -f *.o  dcstego 
	# rm -f *.o  dcstego dcutils dcimage

dcstego.o:
# 	$(CC) -c dcstego.c

# dcutils.o:
# 	$(CC) -c dcutils.c

# dcimage.o:
# 	$(CC) -c dcimage.c
