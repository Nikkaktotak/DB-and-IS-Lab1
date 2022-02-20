#pragma once
#include <stdio.h>
#include <string.h>
#include "Structures.h"
#include "Store.h"

void readStore(struct Store* store)
{
	char name[16];
	int status;

	name[0] = '\0';

	printf("Enter store\'s name: ");
	scanf("%s", name);

	strcpy(store->name, name);
}

void readCinemas(struct Cinema* cinema)
{
	int x;
	int price;

	char name[32];
	name[0] = '\0';

	printf("Enter cinema name: ");
	scanf("%s", name);
	strcpy(cinema->cinemaName , name);

	printf("Enter price: ");
	scanf("%d", &price);
	cinema->price = price;
}