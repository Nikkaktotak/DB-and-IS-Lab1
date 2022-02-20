#pragma once

#include <stdio.h>
#include "Store.h"
#include "Structures.h"

void printStore(struct Store store)
{
	printf("Store\'s name: %s\n", store.name);
	printf("Store\'s cinemas count: %d\n", store.cinemaCount);
}

void printGame(struct Cinema cinema, struct Store store)
{
	printf("Store: %s\n", store.name);
	printf("Name : %s\n", cinema.cinemaName);
	printf("Price: %d\n", cinema.price);
}