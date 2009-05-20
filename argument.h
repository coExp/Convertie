#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

struct argument 
{
	unsigned int SPACE : 1 ;	// Vire les espaces en double
	unsigned int LIGNE : 1 ;	// Vire les ligne en trop
	unsigned int CONVERT : 1 ;	// Convertie en matrice
	unsigned int FILE_IN : 1 ;	// Nom fichier IN
	unsigned int FILE_OUT: 1 ;	// Nom fichier OUT
	unsigned int FILE_REN: 1 ;	// Nom fichier Renome les fichier
	unsigned int ERREUR  : 2 ;	// 0: pas erreur   1: Incompatible    2:     3:	

	char ** Files_Names ;	// Future tableau avec noms fichiers

	int Nbr_Files ;			// 
};


#ifdef ARGUMENT 
	struct argument ARG =
	{       0, 1, 1,
		0, 0, 0,
		0,
		NULL, 0
	};

	int AfficheAide ( int Code, char * Message, char * Nom ) ;
	int Lit_Arguments ( int argc, char ** argv ) ;
	extern int AjouteNom ( char * Nom, char *** Liste, int * Nbr ) ;
#else
	extern struct argument ARG ;
	extern int Lit_Arguments ( int argc, char ** argv ) ;
	extern int AfficheAide ( int Code, char * Message, char * Nom ) ;
#endif
