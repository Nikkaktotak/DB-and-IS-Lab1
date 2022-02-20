#pragma once
#pragma warning(disable:4996)

#include <stdio.h>
#include <stdlib.h>
#include "Structures.h"
#include "Checks.h"
#include "Store.h"

#define CINEMAS_DATA "games.fl"
#define CINEMAS_GARBAGE "games_garbage.txt"
#define GAME_SIZE sizeof(struct Cinema)


void reopenDatabase(FILE* database)
{
	fclose(database);
	database = fopen(CINEMAS_DATA, "r+b");
}

void linkAddresses(FILE* database, struct Store store, struct Cinema cinema)
{
	reopenDatabase(database);								

	struct Cinema previous;

	fseek(database, store.firstGameIdx, SEEK_SET);

	for (int i = 0; i < store.cinemaCount; i++)		    
	{
		fread(&previous, GAME_SIZE, 1, database);
		fseek(database, previous.nextIdx, SEEK_SET);
	}

	previous.nextIdx = cinema.selfIdx;				
	fwrite(&previous, GAME_SIZE, 1, database);		
}

void relinkAddresses(FILE* database, struct Cinema previous, struct Cinema cinema, struct Store* store)
{
	if (cinema.selfIdx == store->firstGameIdx)		
	{
		if (cinema.selfIdx == cinema.nextIdx)			
		{
			store->firstGameIdx = -1;					
		}
		else                                            
		{
			store->firstGameIdx = cinema.nextIdx;  
		}
	}
	else                                         
	{
		if (cinema.selfIdx == cinema.nextIdx)		
		{
			previous.nextIdx = previous.selfIdx; 
		}
		else                                     
		{
			previous.nextIdx = cinema.nextIdx;	
		}

		fseek(database, previous.selfIdx, SEEK_SET);
		fwrite(&previous, GAME_SIZE, 1, database);	
	}
}

void noteDeletedGame(long address)
{
	FILE* garbageZone = fopen(CINEMAS_GARBAGE, "rb");		

	int garbageCount;
	fscanf(garbageZone, "%d", &garbageCount);

	long* delAddresses = malloc(garbageCount * sizeof(long)); 

	for (int i = 0; i < garbageCount; i++)
	{
		fscanf(garbageZone, "%ld", delAddresses + i);
	}

	fclose(garbageZone);							
	garbageZone = fopen(CINEMAS_GARBAGE, "wb");				
	fprintf(garbageZone, "%ld", garbageCount + 1);			

	for (int i = 0; i < garbageCount; i++)
	{
		fprintf(garbageZone, " %ld", delAddresses[i]);		
	}

	fprintf(garbageZone, " %d", address);					
	free(delAddresses);										
	fclose(garbageZone);									
}

void overwriteGarbageAddress(int garbageCount, FILE* garbageZone, struct Cinema* record)
{
	long* delIds = malloc(garbageCount * sizeof(long));		

	for (int i = 0; i < garbageCount; i++)
	{
		fscanf(garbageZone, "%d", delIds + i);				
	}

	record->selfIdx = delIds[0];						
	record->nextIdx = delIds[0];

	fclose(garbageZone);									
	fopen(CINEMAS_GARBAGE, "wb");							    
	fprintf(garbageZone, "%d", garbageCount - 1);			

	for (int i = 1; i < garbageCount; i++)
	{
		fprintf(garbageZone, " %d", delIds[i]);				
	}

	free(delIds);											
	fclose(garbageZone);									
}

int insertGame(struct Store store, struct Cinema cinema, char* error)
{
	cinema.exists = 1;

	FILE* database = fopen(CINEMAS_DATA, "a+b");
	FILE* garbageZone = fopen(CINEMAS_GARBAGE, "rb");

	int garbageCount;											

	fscanf(garbageZone, "%d", &garbageCount);

	if (garbageCount)											
	{
		overwriteGarbageAddress(garbageCount, garbageZone, &cinema);
		reopenDatabase(database);								
		fseek(database, cinema.selfIdx, SEEK_SET);			
	}
	else                                                    
	{
		fseek(database, 0, SEEK_END);

		int dbSize = ftell(database);

		cinema.selfIdx = dbSize;
		cinema.nextIdx = dbSize;
	}

	fwrite(&cinema, GAME_SIZE, 1, database);					

	if (!store.cinemaCount)								    
	{
		store.firstGameIdx = cinema.selfIdx;
	}
	else                                                       
	{
		linkAddresses(database, store, cinema);
	}

	fclose(database);										

	store.cinemaCount++;										
	updateStore(store, error);								

	return 1;
}

int getSlave(struct Store store, struct Cinema* cinema, int productId, char* error)
{
	if (!store.cinemaCount)									
	{
		strcpy(error, "This master has no slaves yet");
		return 0;
	}

	FILE* database = fopen(CINEMAS_DATA, "rb");


	fseek(database, store.firstGameIdx, SEEK_SET);		
	fread(cinema, GAME_SIZE, 1, database);

	for (int i = 0; i < store.cinemaCount; i++)			
	{
		if (cinema->genreId == productId)					
		{
			fclose(database);
			return 1;
		}

		fseek(database, cinema->nextIdx, SEEK_SET);
		fread(cinema, GAME_SIZE, 1, database);
	}
	
	strcpy(error, "No such cinema in database");			
	fclose(database);
	return 0;
}

int updateSlave(struct Cinema cinema, int productId)
{
	FILE* database = fopen(CINEMAS_DATA, "r+b");

	fseek(database, cinema.selfIdx, SEEK_SET);
	fwrite(&cinema, GAME_SIZE, 1, database);
	fclose(database);
	
	return 1;
}

int deleteSlave(struct Store store, struct Cinema cinema, int productId, char* error)
{
	FILE* database = fopen(CINEMAS_DATA, "r+b");
	struct Cinema previous;

	fseek(database, store.firstGameIdx, SEEK_SET);

	do		                                                    
	{															
		fread(&previous, GAME_SIZE, 1, database);
		fseek(database, previous.nextIdx, SEEK_SET);
	}
	while (previous.nextIdx != cinema.selfIdx && cinema.selfIdx != store.firstGameIdx);

	relinkAddresses(database, previous, cinema, &store);
	noteDeletedGame(cinema.selfIdx);						

	cinema.exists = 0;											

	fseek(database, cinema.selfIdx, SEEK_SET);				
	fwrite(&cinema, GAME_SIZE, 1, database);					
	fclose(database);

	store.cinemaCount--;										
	updateStore(store, error);

}