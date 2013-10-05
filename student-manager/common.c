/********************************************************************************
*	File name:		utility.c
*	Author:			YangChao
*	Last Modified:		2011/12/22
********************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "database.h"

#define X_BUF_SIZE 1024

/*******************************************************************************
* Description: main menu
* @paremeters: void
* @return:	   void
*******************************************************************************/
void mainMenu(void)
{
	printf("=========================================\n");
	printf("|\tStudent-Information Manager 0.1\t|\n");
	printf("-----------------------------------------\n");
	//printf("\t1.Start with a new database.\n");
	printf("\t2.Load a database from file.\n");
	printf("\t3.Option current database.\n");
	printf("\t4.Display current database.\n");
	printf("\t5.Save current database.\n");
	printf("\t6.Free current database.\n");
	printf("\t8.Help.\n");
	printf("\t9.About information.\n");
	printf("\t0.Exit.\n");
	printf("----------------------------------------\n");
}

/*******************************************************************************
* Description: sub menu
* @paremeters: void
* @return:	   void
*******************************************************************************/
void subMenu(void)
{
	printf("========================================\n");
	printf("\t1.Insert a record.\n");
	printf("\t2.Search by ID.\n");
	printf("\t3.Search by Name.\n");
	printf("\t4.Erase by ID.\n");
	printf("\t4.Modify by ID.\n");
	printf("\t0.Back to main menu.\n");
	printf("----------------------------------------\n");
}

/*******************************************************************************
* Description:  get user's confirm
* @paremeters: char *prompt
* @return:Choice
*******************************************************************************/
Choice getConfirm(char *prompt)
{
	char confirm;
	printf("%s([y/n]\n",prompt);
	confirm = getchar();

	while (confirm != 'y' && confirm != 'Y' 
		&& confirm != 'n' && confirm != 'N')
	{
		printf("Unknown choice!	     Please enter again.\n");
		confirm = getchar();
	}

	return confirm;

}

/*******************************************************************************
* Description:  get user's option
* @paremeters: void(*menu(void) int reange
* @return:Choice
*******************************************************************************/
Choice getOpt(void(*menu)(void),int range)
{
	Choice choice;
	menu();
	scanf("%hu",&choice);	// unsigned short

	while (choice <0 || choice > range)
	{
		printf("Undifined choice!   Please enter again.\n");
		scanf("%hu",&choice);
	}
	
	return choice;
}

