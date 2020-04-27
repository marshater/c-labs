

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define num 3

struct str
{
	int year;
	int price;
	int number;
};

int compare(const struct str **a, const struct str **b)
{
	return (*a)->price - (*b)->price;
}

int main(int argc, char **argv)
{
	struct str *var[num];

	for (int i = 0; i < num; i++)
	{
		var[i]=(struct str*)malloc(sizeof(struct str));
			if (NULL == var[i])
			{
			printf("<%s> %d: calloc failed\n", __FUNCTION__, __LINE__);
			return -1;
			}
		printf("Введите год\n");
		scanf("%i", &var[i]->year);
		printf("Введите цену\n");
		scanf("%i", &var[i]->price);
		printf("Введите количество\n");
		scanf("%i", &var[i]->number);
	}

	qsort(var, num, sizeof(struct str*), compare);

	for(int i = 0; i < num; i++)
	{
		printf("Год\n");
		printf("%d\n", var[i]->year);
		printf("Цена\n");
		printf("%d\n", var[i]->price);
		printf("Количество\n");
		printf("%d\n", var[i]->number);
		free(var[i]);
	}
}
