#include <stdio.h>

int main(int argc, char *argv[])
{
	FILE *fp;
	FILE *stream;
	fp = fopen(argv[1], "a");
	stream = fopen("doub", "w");
char ch;
while((ch=fgetc(fp)) != EOF){
	if (ch == '\n'){
		fputc(ch, stream);
		}
	}
fclose(fp);
fclose(stream);
	return 0;
}

