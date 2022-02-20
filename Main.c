﻿#include <stdio.h>
#include "Structures.h"
#include "Cinema.h"
#include "Store.h"
#include "Input.h"
#include "Output.h"


int main()
{
	struct Store store;
	struct Cinema cinema;

	while (1)
	{  
		int choice;
		int id;
		char error[51];

		printf("Choose option:\n0 - Quit\n1 - Insert Store\n2 - Get Store\n3 - Update Store\n4 - Delete Store\n5 - Insert Cinema\n6 - Get Cinema\n7 - Update Cinema\n8 - Delete Cinema\n9 - Info\n");
		scanf("%d", &choice);

		switch (choice)
		{
			case 0:
				return 0;

			case 1:
				readStore(&store);
				insertStore(store);
				break;
				
			case 2:
				printf("Enter ID: ");
				scanf("%d", &id);
				getStore(&store, id, error) ? printStore(store) : printf("Error: %s\n", error);
				break;

			case 3:
				printf("Enter ID: ");
				scanf("%d", &id);

				store.id = id;
				
				readStore(&store);
				updateStore(store, error) ? printf("Updated successfully\n") : printf("Error: %s\n", error);
				break;

			case 4:
				printf("Enter ID: ");
				scanf("%d", &id);
				deleteStore(id, error) ? printf("Deleted successfully\n") : printf("Error: %s\n", error);
				break;

			case 5:
				printf("Enter store\'s ID: ");
				scanf("%d", &id);

				if (getStore(&store, id, error))
				{
					cinema.storeId = id;
					printf("Enter genre ID: ");
					scanf("%d", &id);

					cinema.genreId = id;
					readGames(&cinema);
					insertGame(store, cinema, error);
					printf("Inserted successfully. To access, use store\'s and genre\'s IDs\n");
				}
				else
				{
					printf("Error: %s\n", error);
				}
				break;

			case 6:
				printf("Enter store\'s ID: ");
				scanf("%d", &id);

				if (getStore(&store, id, error))
				{
					printf("Enter genre ID: ");
					scanf("%d", &id);
					getSlave(store, &cinema, id, error) ? printGame(cinema, store) : printf("Error: %s\n", error);
				}
				else
				{
					printf("Error: %s\n", error);
				}
				break;

			case 7:
				printf("Enter store\'s ID: ");
				scanf("%d", &id);

				if (getStore(&store, id, error))
				{
					printf("Enter genre ID: ");
					scanf("%d", &id);
					
					if (getSlave(store, &cinema, id, error))
					{
						readGames(&cinema);
						updateSlave(cinema, id, error);
						printf("Updated successfully\n");
					}
					else
					{
						printf("Error: %s\n", error);
					}
				}
				else
				{
					printf("Error: %s\n", error);
				}
				break;

			case 8:
				printf("Enter store\'s ID: ");
				scanf("%d", &id);

				if (getStore(&store, id, error))
				{
					printf("Enter genre ID: ");
					scanf("%d", &id);

					if (getSlave(store, &cinema, id, error))
					{
						deleteSlave(store, cinema, id, error);
						printf("Deleted successfully\n");
					}
					else
					{
						printf("Error: %s\n", error);
					}
				}
				else
				{
					printf("Error: %s\n", error);
				}
				break;

			case 9:
				info();
				break;

			default:
				printf("Invalid input, please try again\n");
		}

		printf("---------\n");
	}

	return 0;
}
