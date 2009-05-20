//////////////////////////////////////////////////////////////////////////////////
///                                                                            ///
///    ELIMINE LES ESPCAES DOUBLE, LES LIGNES ENT TROP...                      /// 
///    VERIFIE SI LES OPTION SONT ENTREES CORRECTEMENTS                        /// 
///                                                                            ///
//////////////////////////////////////////////////////////////////////////////////

#define CONVERTIE

#include "convertie.h"
#include "argument.h"

//////////////////////////////////////////////////////////////////////////////////
// AJOUTE UN CARACTERE AU TAMPON
//
void AddChar ( char c )
{
	int i ;

	for ( i = 10 ; i > 0 ; i-- )
		TAMP [ i ] = TAMP [ i - 1 ] ;

	TAMP [ 0 ] = c ;
}

//////////////////////////////////////////////////////////////////////////////////
// TESTE LE TAMPON
//
void VerifOption ( void )
{
	if      ( strncasecmp ( TAMP, "]MON[", 5 ) == 0 )
	{
		if ( OPTIONS & O_PAR_OUVERT )
			OPTIONS |= O_ERREUR ;

		OPTIONS |= O_NOM | O_PAR_OUVERT ;

	}
	else if ( strncasecmp ( TAMP, "]NOITALER[", 10 ) == 0 )
	{
		if ( OPTIONS & O_PAR_OUVERT )
			OPTIONS |= O_ERREUR ;

		OPTIONS |= O_RELATION | O_PAR_OUVERT ;
	}
	else if ( strncasecmp ( TAMP, "]OSNOC[", 7 ) == 0 )
	{
		if ( OPTIONS & O_PAR_OUVERT )
			OPTIONS |= O_ERREUR ;

		OPTIONS |= O_CONSO | O_PAR_OUVERT ;
	}
	else if ( strncasecmp ( TAMP, "]DNE[", 5 ) == 0 )
	{
		if ( ! ( OPTIONS & O_PAR_OUVERT ) )
			OPTIONS |= O_ERREUR ;
		else
			OPTIONS ^= O_PAR_OUVERT ;
	}
}

//////////////////////////////////////////////////////////////////////////////////
// VERIFIE SI LE FICHIER EST FORMATER COMME IL FAUT
//
int VerifSource ( FILE * Source , FILE * Cible )
{
	unsigned char c ;
	int ic ;

	FLAGS = 0 ;
	
	while ( EOF != ( ic = getc( Source )  ) )
	{

		c = ( unsigned char ) ic ;
		AddChar ( c ) ;
		VerifOption () ;		
		switch ( c )
		{
			case '\n': FLAGS |= F_RETOUR ; break ;
			case ' ' : FLAGS |= F_ESPACE ; break ;
			default :
				if ( FLAGS & F_TRUC_LIGNE )
				{
					if ( FLAGS & F_RETOUR )					
						fprintf ( Cible, "\n" ) ;
					else if ( FLAGS & F_ESPACE )
						fprintf ( Cible,  " " ) ;				
				}
				FLAGS |= F_TRUC_LIGNE ;		
				A_NON_B ( FLAGS , F_RETOUR ) ;
				A_NON_B ( FLAGS , F_ESPACE ) ;
				fprintf ( Cible , "%c", c ) ;
			break ;				
		}
	}

	if ( OPTIONS & O_ERREUR )
		return -1 ;

	return 0 ;
}

//////////////////////////////////////////////////////////////////////////////////
// VERIFIE SI LE FICHIER EST FORMATER COMME IL FAUT
//
void Convertie_Source ( FILE * Source , FILE * Cible )
{
	unsigned char c ;
	int ic ;

	FLAGS = 0 ;
	
	while ( EOF != ( ic = getc( Source )  ) )
	{

		c = ( unsigned char ) ic ;
		switch ( c )
		{
			case '\n': FLAGS |= F_RETOUR ; break ;
			case ' ' : FLAGS |= F_ESPACE ; break ;
			default :
				if ( FLAGS & F_TRUC_LIGNE )
				{
					if ( FLAGS & F_RETOUR )					
						fprintf ( Cible, "\n" ) ;
					else if ( FLAGS & F_ESPACE )
						fprintf ( Cible,  " " ) ;				
				}
				FLAGS |= F_TRUC_LIGNE ;		
				A_NON_B ( FLAGS , F_RETOUR ) ;
				A_NON_B ( FLAGS , F_ESPACE ) ;
				fprintf ( Cible , "%c", c ) ;
			break ;				
		}
	}
}
