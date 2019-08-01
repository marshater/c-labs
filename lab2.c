/*
 * без имени.c
 * 
 * Copyright 2019 PEKA <peka@peka-VirtualBox>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


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
	string_array = calloc(num_lines, sizeof(char*));
	if (NULL == string_array)
	{
		printf("error \n");
		return -1;
	}
		for (int i = 0; i < num_lines; i++)
	{
		string_array[i] = calloc(1, MAX_LEN);
		if (NULL == string_array[i])
		{
			printf("<%s> %d: calloc failed\n", __FUNCTION__, __LINE__);
			return -1;
		}
		gets(string_array[i]);
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
	printf("%d""\n",swap_num);
	for (int i = 0; i < num_lines; i++)
	{
		free(string_array[i]);
		string_array[i] = NULL;
	}

	free(string_array);

	return 0;
}

