#pragma once

#include <stdio.h>
#include <stdlib.h>
#include "Cinema.h"
#include "Store.h"

int checkFileExistence(FILE* indexTable, FILE* database, char* error)
{
	if (indexTable == NULL || database == NULL)
	{
		strcpy(error, "database files are not created yet");
		return 0;
	}

	return 1;
}

int checkIndexExistence(FILE* indexTable, char* error, int id)
{
	fseek(indexTable, 0, SEEK_END);

	long indexTableSize = ftell(indexTable);

	if (indexTableSize == 0 || id * sizeof(struct Indexer) > indexTableSize)
	{
		strcpy(error, "no such ID in table");
		return 0;
	}

	return 1;
}

int checkRecordExistence(struct Indexer indexer, char* error)
{
	if (!indexer.exists)
	{
		strcpy(error, "the record you\'re looking for has been removed");
		return 0;
	}

	return 1;
}

int checkKeyPairUniqueness(struct Store store, int productId)
{
	FILE* cinemaDB = fopen(CINEMAS_DATA, "r+b");
	struct Cinema cinema;

	fseek(cinemaDB, store.firstCinemaIdx, SEEK_SET);

	for (int i = 0; i < store.cinemaCount; i++)
	{
		fread(&cinema, CINEMA_SIZE, 1, cinemaDB);
		fclose(cinemaDB);

		if (cinema.genreId == productId)
		{
			return 0;
		}

		cinemaDB = fopen(CINEMAS_DATA, "r+b");
		fseek(cinemaDB, cinema.nextIdx, SEEK_SET);
	}

	fclose(cinemaDB);

	return 1;
}

void info()
{
	FILE* indexTable = fopen("master.ind", "rb");

	if (indexTable == NULL)
	{
		printf("Error: database files are not created yet\n");
		return;
	}

	int storeCount = 0;
	int cinemaCount = 0;

	fseek(indexTable, 0, SEEK_END);
	int indAmount = ftell(indexTable) / sizeof(struct Indexer);

	struct Store store;

	char dummy[51];

	for (int i = 1; i <= indAmount; i++)
	{
		if (getStore(&store, i, dummy))
		{
			storeCount++;
			cinemaCount += store.cinemaCount;

			printf("Store #%d has %d cinema(s)\n", i, store.cinemaCount);
		}
	}

	fclose(indexTable);

	printf("Total stores: %d\n", storeCount);
	printf("Total games: %d\n", cinemaCount);
}
