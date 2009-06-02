#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// pour FLAGS -----------
#define F_CIBLE 	1
#define F_ESPACE	2
#define F_RETOUR	4
#define F_TRUC_LIGNE 	8
#define F_COMMENT	16

// pour OPTIONS ---------
#define O_NOM		1
#define O_RELATION	2
#define O_CONSO		4
#define O_ERREUR	16
#define O_PAR_OUVERT	32

// ----------------------
#define A_NON_B(a,b) ( (a) & (b) ? (a) ^= (b) : (a)  )


void Convertie_Source ( FILE * Source , FILE * Cible ) ;
int VerifSource ( FILE * Source , FILE * Cible ) ;
void AddChar ( char c ) ;
void VerifOption ( void ) ;

#ifdef CONVERTIE

    long OPTIONS ;
    long FLAGS ;
    char TAMP[ 11 ] = {'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'} ;

#else

    extern long OPTIONS ;
    extern long FLAGS ;

#endif
