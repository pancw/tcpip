#include <stdio.h>
#define BUF_SIZE 3

// 使用标准I/O函数将得到额外的I/O缓冲
// 比直接使用套接字会有更好的性能
int main()
{
	FILE * fp1;
	FILE * fp2;
	char buf[BUF_SIZE];

	fp1=fopen("news.txt", "r");
	fp2=fopen("cpy.txt", "w");

	while(fgets(buf, BUF_SIZE, fp1)!=NULL)
		fputs(buf, fp2);

	fclose(fp1);
	fclose(fp2);
	return 0;
}
