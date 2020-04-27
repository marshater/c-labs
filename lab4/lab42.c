#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	if (argc < 3)
	{
	fprintf (stderr, "Мало аргументов. Используйте <имя файла> <удаляемый символ>\n");
	}

	FILE *fp;
	FILE *stream;

	fp = fopen(argv[1], "r");
	stream = fopen("editing", "w");

	if((fp=fopen(argv[1], "r"))==NULL||(stream=fopen("editing", "w"))==NULL) 
	{
    	printf("Не удается открыть файл.\n");
    	exit(1);
	}

	deleting(fp, stream, *argv[2]);

	fclose(fp);
	fclose(stream);

	return 0;
}

void deleting(FILE *fp, FILE *stream, char del)
{
	char ch;
	while ((ch=fgetc(fp)) != EOF)
	{
		if(ch != del)
		{
			fputc(ch, stream);
		};
	}
}
