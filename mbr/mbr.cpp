#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
struct PTE{
	unsigned char active;
	unsigned char ca1[3];
	unsigned char pt;
	unsigned char ca2[3];
	unsigned char las[4]={0,};
	unsigned char nos[4];
};
struct embr{
	unsigned char boot_code[446];
	struct PTE pte[4];
	unsigned char sign[2];
};

unsigned long lllle(unsigned char *buffer, int size)
{
	int j;
	unsigned long result=0;
	unsigned long i;
	for( i=0x1,j = 0; j<size; j+=1, i*=0x100)	result += buffer[j]*i;
	return result;
}

int main(void){
	int i, j=0;
	FILE * fp;
	fp = fopen("mbr_128.dd","rb");
	if(!fp) 	return -1;
	struct embr mbr;
	fread(&mbr,1,sizeof(mbr),fp);
	for(i=0;i<4;i++)
	{
		if(mbr.pte[i].pt==0x5)	
		{
			printf("mbr	\n");
			unsigned int base_addr=lllle(mbr.pte[i].las, 4)*0x200;
			struct embr ebr;			
			unsigned int ste_addr = 0;
			do
			{
				j++;
				
				fseek(fp,base_addr+ste_addr,SEEK_SET);
				fread(&ebr,1,sizeof(ebr),fp);
				printf("start addr : %08X	",base_addr+ste_addr+lllle(ebr.pte[0].las, 4)*0x200);
				printf("size : %08X\n",lllle(ebr.pte[0].nos, 4)*0x200);
				
				ste_addr=lllle(ebr.pte[1].las, 4)*0x200;
			}while(lllle(ebr.pte[1].nos, 4));
			
		}
		else if(mbr.pte[i].pt==0x7)	
		{
			printf("ntfs	");
			printf("start addr : %08X	",lllle(mbr.pte[i].las, 4)*0x200);
			printf("size : %08X\n",lllle(mbr.pte[i].nos, 4)*0x200);
		}
		
	}
	

	return 0;
}
