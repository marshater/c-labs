#include <stdio.h>
#include <stdlib.h>
#define MAX_LEN 128

int main(int argc, char **argv)
{
	int swap_num=0;
	printf("Enter N of lines\n");
	int num_lines = 0;
	scanf("%d\n", &num_lines);
	char ** string_array = NULL;
	char ** buff_array = NULL;
	buff_array = calloc(num_lines, sizeof(char*));
	string_array = calloc(num_lines, sizeof(char*));
	if (NULL == string_array)
		{
		printf("error \n");
		return -1;
		}
		for (int i = 0; i < num_lines; i++)
		{
		string_array[i] = calloc(1, MAX_LEN);
		buff_array[i] = calloc(1, MAX_LEN);
			if (NULL == string_array[i])
			{
			printf("<%s> %d: calloc failed\n", __FUNCTION__, __LINE__);
			return -1;
			}
				if (NULL == buff_array[i])
				{
				printf("<%s> %d: calloc failed\n", __FUNCTION__, __LINE__);
				return -1;
				}
		gets(string_array[i]);
		strcpy(buff_array[i],string_array[i]);
		}
	
	int sorting(const void * x, const void * y)
	{
		return **((char**)y)-**((char **)x);
	}
	qsort(string_array, num_lines, sizeof(char *), sorting);
	
	printf("sorted:\n");
	for (int i = 0; i<num_lines; i++)
	{
		puts(string_array[i]);
	}
	int i = strlen((char**)string_array[0]);
	printf("%d""\n",i);
	for (int i = 0; i<num_lines; i++)
	{
		if (*buff_array[i] != *string_array[i])
		{
			swap_num++;
		}
		
	}
	printf("%d""\n",swap_num);
	for (int i = 0; i < num_lines; i++)
	{
		free(string_array[i]);
		free(buff_array[i]);
	}
	free(buff_array);
	free(string_array);

	return 0;
}
