#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#pragma pack(push)
#pragma pack(1)
typedef struct {
	unsigned short type;
	unsigned int size;
	unsigned short reserved1, reserved2;
	unsigned int offset;
} HEADER;

typedef struct {
	unsigned int size;/* Info Header size in bytes */
	int width,height;/* Width and height of image */
	unsigned short int planes;/* Number of colour planes */
	unsigned short int bits; /* Bits per pixel */
	unsigned int compression; /* Compression type */
	unsigned int imagesize; /* Image size in bytes */
	int xresolution,yresolution; /* Pixels per meter */
	unsigned int ncolours; /* Number of colours */
	unsigned int importantcolours; /* Important colours */
} INFOHEADER;
#pragma pack(pop)

int openBMP(char *filename)
{
	char name[100];
	unsigned char *buf;
	FILE *fd, *fd2;
	HEADER header;
	INFOHEADER infoheader;
	int rowsize;
	long filesize, i, j, k, num;

	printf("BMP Filename\t: %s\n",filename);
	fd = fopen(filename, "rb");
	fread(&header, 1, sizeof(HEADER), fd);

	if(header.type != 0x4d42) {
		printf("%s is not bmp\n",filename);
		return -1;
	}

	filename[strlen(filename)-4] = '\0';
	sprintf(name,"oem_splash_%s.h", filename);

	fread(&infoheader, 1, sizeof(INFOHEADER), fd);

	fseek(fd, 0L, SEEK_END);
	filesize = ftell(fd);
	fseek(fd, header.offset, SEEK_SET);

	fd2 = fopen(name, "w");
	printf("HEADER Filename\t: %s\n", name);
	name[strlen(name)-2]='\0';

	rowsize = (infoheader.width + 1);
	rowsize *= 3;
	rowsize /= 4;
	rowsize *= 4;

	printf("Array name \t: %s\n", name);
	printf("width \t: %d\n", infoheader.width);
	printf("height \t: %d\n", infoheader.height);
	
	fprintf(fd2, "static int %s_width = %d;\n", name, infoheader.width);
	fprintf(fd2, "static int %s_height = %d;\n", name, infoheader.height);
	fprintf(fd2, "\nstatic char %s[] = {\n", name);

	buf = (unsigned char *)malloc(infoheader.width*rowsize);
	num = 0;
#ifdef UPSIDEDOWN
	for (i=infoheader.height-1; i>=0; i--) {
		fseek(fd, header.offset+rowsize*i, SEEK_SET);
		fread(buf, 1, rowsize, fd);
		for (j=0; j<infoheader.width; j++) {
#else
	for (i=0; i<infoheader.height; i++) {
		fread(buf, 1, rowsize, fd);
		for (j=infoheader.width-1; j>=0; j--) {
#endif
			for(k=j*3 ; k<j*3+3; k++) { 	
				fprintf(fd2, " 0x%02x,", buf[k]);
				num = (num + 1) % 16;
				if(!num) {
					fprintf(fd2, "\n");
				}
			}
		}
	}

	fprintf(fd2, "\n};\n");
	return 0;
}

int main(int argc, char *argv[])
{
	if(argc != 2) {
		printf("COMMAND ERROR\n");
		return -1;
	}

	openBMP(argv[1]);

	return 1;
}
