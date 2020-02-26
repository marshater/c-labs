#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LIM 100

void removing(FILE*, FILE*, int);

int main(int argc, char *argv[])
{
	if (argc < 2)
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

	removing(fp, stream, atoi(argv[2]));

	fclose(fp);
	fclose(stream);

	return 0;
}
void removing(FILE *fp, FILE *stream, int num)
{
	char ch;
	int count = 0;
	char *p = NULL;
	p = (char*)calloc(LIM, sizeof(char));
	while ((ch=fgetc(fp)) != EOF)
	{
		if(ch != '\n')
		{
			p[count] = ch;
			count++;
		}
			else if(count < num)
			{
				fputs(p, stream);
				fputs("\n", stream);
				count = 0;
				memset(p, 0, LIM);
			}
			else
			{
				count = 0;
				memset(p, 0, LIM);
			}
	};
	free (p);
}
