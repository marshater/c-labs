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
while ((ch=fgetc(fp)) != EOF){
	if (ch == *argv[2]){
		do{
		fputc(ch, stream);
		}while (ch == '\n');
		}
}
fclose(fp);
fclose(stream);
	return 0;
}
