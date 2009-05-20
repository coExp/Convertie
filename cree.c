/*******************/
/*  creer.c        */
/*******************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "convertie.h"
#include "argument.h"
#include "../Fonctions/fonctions.h"

int fcloseall( void ) ;

fpos_t NOM_pos ;
fpos_t REL_pos ;

int      Nbr_Sommet ;
char  ** Nom_Sommet ;

float ** Matrice_Max ;		// Matrice des capacite max entre sommet
float ** Matrice_Conso ;	// Matrice sur les conso deja existantes

/////////////////////////////////////////////////////////////////////////////////////
// CREE UNE RELATION ENTRE DEUX SOMMET
//
void CreerRelation ( int De, int Vers, float flux, float ** Matrice )
{
	if ( ( De == -1 ) || ( Vers == -1 ) )
		exit ( AfficheAide( 8, "Nom non trouvé dans la liste", "---" ) ) ;

	Matrice [ De ][ Vers ] = flux ;
}

/////////////////////////////////////////////////////////////////////////////////////
// CHARGE LES NOMS DES SOMMET QUI SONT ENTRE BALISE [NOM]
//
int Charge_Nom_Normal ( FILE * Source )
{
	char mot1 [ 50 ] ;

	fseek ( Source, 0L, SEEK_SET ) ;

	do 
	{
		fscanf ( Source, "%49s", mot1 );
		if ( feof( Source ) )
			return -1 ;

	} while ( strncasecmp ( mot1, "[NOM]", 5 ) != 0 ) ;

	for (;;)
	{
		fscanf ( Source, "%49s", mot1 );
		if ( ( feof( Source ) ) || ( 0 == strncasecmp(mot1,"[END]",5) ) )
			break ;

		AjouteNom ( mot1 , &Nom_Sommet, &Nbr_Sommet ) ;
	}
	return 0 ;
}

//////////////////////////////////////////////////////////////////////////////////
// LES NOMS DES SOMMETS NE SONT PAS ENTREES ENTRE BALISE : ON LES AJOUTE 
// 	EN LES LISANT
int Charge_Nom_De_Max ( FILE * Source )
{
	char mot1 [ 50 ] ;
	char mot2 [ 50 ] ;
	float flux ;
	int n1, n2 ;

	fseek ( Source, 0L, SEEK_SET ) ;
	do 
	{
		fscanf ( Source, "%49s", mot1 );
		if ( feof( Source ) )
			return -1 ;

	} while ( strncasecmp ( mot1, "[RELATION]", 5 ) != 0 ) ;
	//---------------------------------------------
	for (;;)
	{
		fscanf ( Source, "%49s", mot1 ) ;

		if ( 0 == strncasecmp( mot1, "[END]", 5 ) ) 
			break ;

		if ( feof( Source ) ) 	// Fin fichier alors que [end] attendu
			return -1 ;

		fscanf ( Source, " %49s %f\n", mot2, &flux );

		if ( ( n1 = Cherche ( mot1, Nom_Sommet, Nbr_Sommet ) )  == -1 )
			n1 = AjouteNom ( mot1, &Nom_Sommet, &Nbr_Sommet ) ;

		if ( ( n2 = Cherche ( mot2, Nom_Sommet, Nbr_Sommet ) ) == -1 )
			n2 = AjouteNom ( mot2, &Nom_Sommet, &Nbr_Sommet ) ;
	}
	return 0 ;
}

/////////////////////////////////////////////////////////////////////////////////////
// LIT LA SOURCE A LA RECHERHCE DES RELATION POUR Max OU Conso
//
int Charge_Relation ( FILE * Source, float *** Matrice, char * MotCle )
{
	char mot1 [ 50 ] ;
	char mot2 [ 50 ] ;
	float flux ;

	* Matrice = Creer_Matrice_Carre ( Nbr_Sommet ) ;
	fseek ( Source, 0L, SEEK_SET ) ;

	do 
	{
		fscanf ( Source, "%49s", mot1 );

		if ( feof( Source ) )
			return -1 ;

	} while ( strncasecmp ( mot1, MotCle, 5 ) != 0 ) ; //  "[RELATION]",

	for (;;) 
	{
		fscanf ( Source, "%49s", mot1 ) ;

		if ( 0 == strncasecmp( mot1, "[END]", 5 ) ) 
			break ;

		if ( feof( Source ) ) 	// Fin fichier alors que [end] attendu
			return -1 ;

		fscanf ( Source, " %49s %f\n", mot2, &flux );

		CreerRelation ( Cherche( mot1 , Nom_Sommet, Nbr_Sommet ), Cherche( mot2 , Nom_Sommet, Nbr_Sommet ), flux, * Matrice ) ;	
	}
	return 0 ;
}

/////////////////////////////////////////////////////////////////////////////////////
// LIT LA SOURCE ET LA CONVERTIE
//
int LitSource ( FILE * Source )
{
	if ( OPTIONS & O_NOM ) 
	{
		if ( Charge_Nom_Normal ( Source ) == -1 )
			return -1 ;	
	} else 
	{
		if ( Charge_Nom_De_Max ( Source ) == -1 )
			return -1 ;		
	}

	Charge_Relation ( Source, &Matrice_Max, "[RELATION]" ) ;

	if ( OPTIONS & O_CONSO )
		Charge_Relation ( Source, &Matrice_Conso, "[CONSO]" ) ;
	
	return 0 ;
}

/////////////////////////////////////////////////////////////////////////////////////
// INITIALISE
//
void Init ( void )
{
	Nom_Sommet = NULL ;
	Nbr_Sommet = 0 ;

	OPTIONS = 0 ;
	FLAGS = 0 ;
}

/////////////////////////////////////////////////////////////////////////////////////
// ECRIT LE FICHIER CIBLE AVEC LA MATRICE RELATION
//
int EcritMatrice ( FILE * cible, int nbr, char ** liste_nom, float ** mat_max, float ** mat_conso  )
{
	int L, C ;

	// Nombre de sommets
	fprintf ( cible, "%d\n", nbr ) ;

	// Nom des sommets
	if ( OPTIONS & O_NOM )	
	{	
		for ( L = 0 ; L < nbr ; L++ )
			fprintf ( cible, "%s ", liste_nom [ L ] ) ; 
		fprintf ( cible, "\n" ) ;
	}

	// Matrice flux max
	for ( L = 0 ; L < nbr ; L++ )
	{
		for ( C = 0 ; C < nbr ; C++ )
			fprintf ( cible, "%f ", mat_max [ L ][ C ] ) ;
		fprintf ( cible, "\n" ) ;
	}
	// ----------------
	fprintf ( cible, "---------\n" ) ;
	
	if ( OPTIONS & O_CONSO )
	{
		for ( L = 0 ; L < nbr ; L++ )
		{		
			for ( C = 0 ; C < nbr ; C++ )
				fprintf ( cible, "%f ", mat_conso [ L ][ C ] ) ;
			fprintf ( cible, "\n" ) ;	
		}
	}
	return 0 ;
}

/////////////////////////////////////////////////////////////////////////////////////
// 
//
int Main_Fichier_Out ( void )
{
	FILE * ptr_source ;
	FILE * ptr_tmp ;
	FILE * ptr_cible ;
	int i ;

	if ( ARG.Nbr_Files % 2 )
		return 1 ; 

	for ( i = 0 ; i < ARG.Nbr_Files ; i += 2 )
	{
		if ( NULL == ( ptr_source = fopen ( ARG.Files_Names [ i ] ,  "r" ) ) )
			return 2 ;
		if ( NULL == ( ptr_cible = fopen ( ARG.Files_Names [ i + 1 ], "w+" ) ) )
			return 3 ;
		if ( NULL == ( ptr_tmp = tmpfile () ) )
			return 4 ;
		if ( -1 == VerifSource ( ptr_source, ptr_tmp ) )
			return 5 ;
		if ( -1 == LitSource ( ptr_tmp ) )
			return 6 ;
		if ( -1 == EcritMatrice ( ptr_cible, Nbr_Sommet, Nom_Sommet, Matrice_Max, Matrice_Conso ) )
			return 7 ;

		fclose ( ptr_tmp ) ;
		fclose ( ptr_source ) ;
		fclose ( ptr_cible ) ;
	}
	return 0 ;
}

/////////////////////////////////////////////////////////////////////////////////////
// 
//
int Main_Pas_Fichier_Out ( void )
{
	FILE * ptr_source ;
	FILE * ptr_tmp ;
	FILE * ptr_cible ;
	char * Nom_Cible ;		
	int i ;

	for ( i = 0 ; i < ARG.Nbr_Files ; i++ )
	{
		Nom_Cible = Change_Extention ( ARG.Files_Names[ i ] , ".mat" ) ;

		if ( NULL == ( ptr_source = fopen ( ARG.Files_Names[ i ] ,  "r" ) ) )
			return 2 ; 
		if ( NULL == ( ptr_cible = fopen ( Nom_Cible , "w+" ) ) )		
			return 3 ; 		
		if ( NULL == ( ptr_tmp = tmpfile () ) )
			return 4 ; 
		if ( -1 == VerifSource ( ptr_source, ptr_tmp ) )
			return 5 ;
		if ( -1 == LitSource ( ptr_tmp ) )
			return 6 ; 
		if ( -1 == EcritMatrice ( ptr_cible , Nbr_Sommet, Nom_Sommet, Matrice_Max, Matrice_Conso ) )
			return 7 ; 

		free ( Nom_Cible ) ;
	 }	
	 return 0 ;
}

/////////////////////////////////////////////////////////////////////////////////////
// HAUT LES MAIN
//
int Main_Pas_Matrice ( void )
{
	FILE * ptr_source ;
	FILE * ptr_tmp ;
	int i ;

	for ( i = 0 ; i < ARG.Nbr_Files ; i++ )
	{
		if ( NULL == ( ptr_source = fopen ( ARG.Files_Names[ i ] ,  "r" ) ) )
			return 2 ; 

		if ( NULL == ( ptr_tmp = tmpfile () ) )
			return 4 ; 

		Convertie_Source ( ptr_source, ptr_tmp ) ;

		if ( 0 != fclose ( ptr_source ) )
			return 2 ;

		if ( NULL == ( ptr_source = fopen ( ARG.Files_Names[ i ] ,  "w" ) ) )
			return 2 ;

		Copy_File ( ptr_source, ptr_tmp ) ;
		fclose ( ptr_source ) ;
		fclose ( ptr_tmp ) ;
	 }	
	 return 0 ;
}

/////////////////////////////////////////////////////////////////////////////////////
// HAUT LES MAIN
//
int main ( int argc, char ** argv )
{
	int Code ;

	if ( -1 == Lit_Arguments ( argc, argv )  )
		return AfficheAide ( 1, "Erreur avec dans les argument", argv[ 0 ] ) ;

	if ( ! ARG.CONVERT )
		Code = Main_Pas_Matrice () ;
	else if ( ARG.FILE_OUT )
		Code = Main_Fichier_Out () ;
	else
		Code = Main_Pas_Fichier_Out () ;

	switch ( Code )
	{
		case 1 : AfficheAide( 1,"Fichier cible manquant", argv [ 0 ] ) ; break;
		case 2 : AfficheAide( 2,"Erreur avec fichier SOURCE" , argv[ 0 ] );break;
		case 3 : AfficheAide( 3,"Erreur avec fichier CIBLE", argv[ 0 ]  );break;
		case 4:AfficheAide(4,"Erreur avec fichier TEMPORAIRE", argv[ 0 ]);break;
		case 5 : AfficheAide( 5,"Erreur avec le format du fichier source",argv[ 0]);break;
		case 6 : AfficheAide( 6,"Erreur lors du decodage", argv[ 0 ]  ) ; break ;
		case 7 : AfficheAide( 7,"Erreur lors de l'ecriture du fichier CIBLE", argv[ 0 ]  ) ; break ;
	}

	fcloseall () ;

	return Code ;
}

