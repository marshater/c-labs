#include <stdio.h>

int main(int argc, char *argv[])
{
	FILE *fp;
	FILE *stream;
	printf(argv[1],"first\n");
	printf(argv[2],"second\n");
	fp = fopen(argv[1], "r+");
	stream = fopen("doub", "w");
	char ch;
	char *buff;
while ((ch=fgetc(fp)) != EOF){
	buff = &ch;
		do{
			fputc(*buff++, stream);
			}while (*buff == *argv[2]);

}
fclose(fp);
fclose(stream);
	return 0;
}
