#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void file_init();
void padder();
void insert_header();

char filelist[][20] = {	
			"boot.bin", 
			"kernel.bin",
			"junk.bin"
			};

struct file_vars {
	int fileindex, totalfiles;
	FILE *src, *tmp, *dst;
	uint8_t byte;
	void *ptr;
};

struct file_vars fv1;

int main(int argc, char ** argv)
{
	file_init();
	
	fv1.src = fopen(filelist[fv1.fileindex], "rb");
	
	while (1) {
		if (fread(fv1.ptr, 1, 1, fv1.src) == 1) {
			fprintf(stdout, 
				"%d -> %d : %02x\n", 
				ftell(fv1.src), ftell(fv1.dst), fv1.byte);
			
			fwrite(&fv1.byte, 1, 1, fv1.dst);		}
		else {
			if (fv1.fileindex < fv1.totalfiles) {
				fclose(fv1.src);
				/*
				 * zero pad here
				 */
				fv1.byte = 0;
				padder();
				++fv1.fileindex;
				fv1.src = fopen(filelist[fv1.fileindex], "rb");
			}
			else {
				break;
			}
		}
	}
	return 0;
}


void file_init()
{
	fv1.fileindex = 0;
	fv1.totalfiles = sizeof(filelist) / sizeof(filelist[0]);
	fv1.src = fopen(filelist[fv1.fileindex], "rb");
	fv1.dst = fopen("CrazyOS.bin", "wb+");
	fv1.byte = 0;
	fv1.ptr = &fv1.byte;
}


void padder()
{
	int count, flag;
	count = 0;
	flag = 0;
	fv1.byte = 0x90;
	while (count < 2) {
		if ((ftell(fv1.dst)) % 512 == 0 && flag == 0) {
			insert_header();
			++flag;
		}
		while ((ftell(fv1.dst)) % 512 != 0) {
			printf("%d\n", ftell(fv1.dst));
			fwrite(&fv1.byte, 1, 1, fv1.dst);
		}
		++count;
	}
}


void insert_header()
{
	printf("\nBINGO!!\n");
	/*
	 * Set jmp
	 * Set magic
	 * Set filename
	 */		
	fv1.tmp = fv1.dst;
	uint16_t magic_for_new_file = 0xaa55;
	uint8_t jmp = 0xeb;
	uint8_t disp = sizeof(magic_for_new_file) + sizeof(filelist[fv1.fileindex]) + 1;
	fwrite(&jmp, 1, 1, fv1.dst);
	fwrite(&disp, 1, 1, fv1.dst);
	fwrite(&magic_for_new_file, 2, 1, fv1.dst);	
	fwrite(filelist[fv1.fileindex], 1, 20, fv1.dst);
}
