MODULE = bmp2header
MODULE2 = bmp2header_upsidedown
	
$(MODULE):
	gcc bmp2header.c -o bmp2header
	gcc bmp2header.c -DUPSIDEDOWN=1 -o bmp2header_us

clean:
	rm -rf bmp2header bmp2header_us
