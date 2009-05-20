/*******************/
/*   argument.c    */
/*******************/

#define ARGUMENT

#include "argument.h"

///////////////////////////////////////////////////////////////////////////////
int Lit_Arguments ( int argc, char ** argv )
{	
	int opt, i, IN ;
	FILE * ptr ;

	while ( (opt = getopt(argc, argv, "hRrCcnN")) != -1  )
	{
		switch ( opt )
		{
			case 'h' : case 'H' :
				exit ( AfficheAide( 0 , "", argv[ 0 ] ) ) ;
			case 'r' : case 'R' :
				ARG.FILE_REN = 1 ;
				break ;
			case 'c' : case 'C' :
				ARG.FILE_OUT = 1 ;
				break ;
			case 'n' : case 'N' :
/*				if ( strcasecmp( optarg, "s" ) == 0 )
				{
					ARG.SPACE   = 1 ;
					ARG.LIGNE   = 0 ;
					ARG.CONVERT = 0 ;
				}
				else if ( strcasecmp( optarg, "n" ) == 0 )
				{
					ARG.SPACE   = 0 ;
					ARG.LIGNE   = 1 ;
					ARG.CONVERT = 0 ;
				}
				else if ( ( strcasecmp( optarg, "sn" ) == 0 ) || ( strcasecmp( optarg, "ns" ) == 0 ) )
				{*/
					ARG.SPACE   = 1 ;
					ARG.LIGNE   = 1 ;
					ARG.CONVERT = 0 ;
				//}
				break ;
			default :
				ARG.ERREUR = 1 ;
		}	
	}

	for ( i = optind, IN = 1 ; i < argc ; i++ )	
	{
		// On teste existence du fichier en entrer
		if ( ( ARG.FILE_IN  ) || ( IN ) )
		{
			if ( NULL == ( ptr = fopen ( argv[ i ] , "rb" ) ) )
				exit ( AfficheAide( 9, "Fichier inexistant OU probleme de droit", argv [ 0 ]  ) ) ;
			else 
				fclose ( ptr ) ;
			ARG.FILE_IN = 1 ;
		}

		AjouteNom ( argv[ i ] , &(ARG.Files_Names) , &(ARG.Nbr_Files) ) ;

		switch ( IN )
		{
			case 0 : IN = 1 ; break ;
			case 1 : IN = 0 ; break ;
		}
	}
	if ( ! ARG.FILE_IN )
		exit ( AfficheAide (0,"", argv [ 0 ]) ) ;
	return 0 ;
}

///////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
// AFFICHE UNE AIDE SOMMAIRE
//
int AfficheAide ( int Code, char * Message, char * Nom )
{
	if ( strcasecmp ( Message, "" ) != 0 )
		fprintf ( stderr, "\033[0;31m%s\n\033[0;m", Message ) ;

	printf ( "Usage : %s [OPTION] FICHIER [FICHIER]...\nConvertie FICHIER source dans le bon format\n", Nom ) ;
	printf ( "Fichier source decrit un graphe de flux, ses sommets, ses capacités entre sommets, voir ses consommations courrantes\nLe fichier cible sera une matrice exploitable par des algo de maximisation de flux.\n" ) ;
	printf ("\nOPTION :\n") ;
	printf ("  -R   cree le nom fichier cible (foo.txt -> foo.mat)\n") ;
	printf ("  -C   <file.in> <file.out> [...] ; ne cree pas les noms des fichiers cibles\n" ) ;
	printf ("  -N   enleve les \\n et espaces doublés, ne cree pas de matrice de conso, ecrase fichier source \n") ;
//	printf ("      n      enleve les retour de ligne doublees\n") ;
//	printf ("      s      enleve les espaces en doubles\n") ;
//	printf ("      sn     enlece les espaces et lignes doubles\n") ;
	printf ( "\nSyntaxe du fichier source :" ) ;
	printf ( "\t[NOM] nom1 nom2 ... [END]\n" ) ;
	printf ( "\t[RELATION]\n\tnom1 nom2 15\n\tnom5 nom4 20\n\t[END]\n" ) ;
	printf ( "\t[CONSO]\n\tnom1 nom2 5\n\tnom6 nom4 2\n\t[END]\n" ) ;
	return Code ;
}
