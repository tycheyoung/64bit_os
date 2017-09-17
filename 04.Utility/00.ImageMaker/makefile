all: ImageMaker mov

ImageMaker: ImageMaker.c
	gcc -o $@ $<
mov:
	mv ImageMaker ../../

clean:	
	rm -f ImageMaker