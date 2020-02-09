#include <stdio.h>
#include <string.h>
#include <stdlib.h>
struct str{
	int year;
	int price;
	int number;
};
int main(int argc, char **argv){
	int num=0;
	printf("Введите число записей\n");
	scanf("%i", &num);
	struct str var[num];
	for (int i = 0; i < num; i++){
		printf("Введите год\n");
		scanf("%i", &var[i].year);
		printf("Введите цену\n");
		scanf("%i", &var[i].price);
		printf("Введите количество\n");
		scanf("%i", &var[i].number);
	}
	for(int i = 0; i < num; i++){
		printf("Год\n");
		printf("%d\n", var[i].year);
		printf("Цена\n");
		printf("%d\n", var[i].price);
		printf("Количество\n");
		printf("%d\n", var[i].number);
	}
	return 0;
}
