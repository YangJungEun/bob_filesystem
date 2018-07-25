#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>

struct GPThdr {
   unsigned char sign[8];
   unsigned char revs[4];
   unsigned char hdrSize[4];
   unsigned char crc[4];
   unsigned char rsvd[4];
   unsigned char present_LBA[8];
   unsigned char bklb[8];
   unsigned char fstlba[8];
   unsigned char lstlb[8];
   unsigned char dg[16];
   unsigned char pelba[8];
   unsigned char numP[4];
   unsigned char sizeOfPE[4];
   unsigned char pECRC[4];
}; 
struct GPTpe{
	unsigned char ptGUID[16];
	unsigned char upGUID[16];
	unsigned char fstLBA[8];
	unsigned char lstLBA[8];
	unsigned char flags[8];
	unsigned char ParName[72];
};

unsigned long lllle(unsigned char *buffer, int size)
{
	int j;
	unsigned long result=0;
	unsigned long c;
	for( c=0x1,j = 0; j<size; j+=1, c*=0x100)	result += buffer[j]*c;
	return result;
}
int main(void){
	int i, j;
	struct GPThdr header;
	struct GPTpe PE;
	FILE * fp;
	fp = fopen("gpt_128.dd","rb");
	if(!fp) exit(0);
	
	
	//////////////////////////////////////////////////
	
	
	fseek(fp, 0x400, SEEK_SET);
	for(i = 0; i<128;i++)
	{
		fseek(fp, 0x400 + i*0x80, SEEK_SET);
		fread(&PE, 1, sizeof(PE), fp);
		if(*PE.ptGUID == 0)	break;
		unsigned long LBAstart = lllle(PE.fstLBA,8);
		unsigned long size = (lllle(PE.lstLBA,8)-lllle(PE.fstLBA,8)+1);
		printf("start addr : %X, size : %X\n",LBAstart*512, size*0x200);		
	}
	return 0;
	
}


	
