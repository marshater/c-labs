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
#include <string.h>
#include <stdlib.h>
#define num 3
struct str
	{
	int year;
	int price;
	int number;
	};
int compare(const struct str **a, const struct str **b){
	return (*a)->price - (*b)->price;
}
int main(int argc, char **argv)
{
	struct str *var[num];
	for (int i = 0; i < num; i++)
	{
		var[i]=(struct str*)malloc(sizeof(struct str));
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
	}

}


