#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define N 3
#define TAILLE (N * N)

typedef struct {
    int valeur;
    int candidat[TAILLE];
    int nbcandidat;
} tCase2;


typedef int tGrille[TAILLE][TAILLE];
typedef tCase2 tGrilleCase[TAILLE][TAILLE];

void afficherStats(int *nbCasesVides,int *nbCasesVidesAvantSolver, int *nombreDecandidat, int *nombreDecandidatAvantSolver,char *nomFichier);

void initCase(tCase2 *Case);

int initNbCaseVide(tGrille grille);
void ajouterCandidats(tCase2 *Case, int valeur);
void retirerCandidats(tCase2 *Case, int valeur);
bool estCandidats(tGrille grille, int valeur, int numLignes, int numColonne);
void chargerGrille(tGrille g);
void affichageGrille(tGrille grille);
void resolverSudoku(tGrille grille);

int main(){
    tGrille grille;
    chargerGrille(grille);

    printf("Grille initiale :\n");
    affichageGrille(grille);

    resolverSudoku(grille);

    printf("Grille résolue :\n");
    affichageGrille(grille);

    return 0;
}

void initTab(tCase2 *Case) {
    Case->valeur = 0;
    Case->nbCandidats = TAILLE;

    for (int i = 1; i <= TAILLE; i++) {
        Case->candidats[i] = true;
    }
    Case->nbcandidat = 0;
}


int initNbCaseVide(tGrille grille){
    int nbCaseVide = 0;
    for (int i = 0; i < TAILLE; i++)
    {
        for (int j = 0; j < TAILLE; j++)
        {
            if (grille[i][j] == 0)
            {
                nbCaseVide++;
            }
            
        }
        
    }
    return nbCaseVide;
    
}

void ajouterCandidats(tCase2 *Case, int valeur) {
    Case->candidats[valeur] = false;
    Case->nbCandidats--;

    if (Case->nbCandidats == 1) {
        // S'il ne reste qu'un candidat, met à jour la valeur de la case
        for (int i = 1; i <= TAILLE; i++) {
            if (Case->candidats[i]) {
                Case->valeur = i;
                break;
            }
        }
    }
}

void retirerCandidats(tCase2 *Case, int valeur) {
    Case->candidats[valeur] = false;
    Case->nbCandidats--;
}

bool estcandidat(tGrilleCase grilleCase, int valeur, int numLignes , int numColonne){
    int i, j, coinBlocLigne, coinBlocColonne;
    bool possible = true;
    for (j = 0; j < TAILLE; j++) {
        if (grilleCase[numLignes][j].valeur == valeur && j != numColonne) {
            possible = false;
        }
    }

    j = numColonne;
    i = 0;

    for (i = 0; i < TAILLE; i++) {
        if (grille[i][numColonne] == valeur) {
            return false;  // Valeur déjà présente sur la colonne
        }
    }

    coinLigne = (numLignes / N) * N;
    coinColonne = (numColonne / N) * N;

    for (int i = coinLigne; i < coinLigne + N; i++) {
        for (int j = coinColonne; j < coinColonne + N; j++) {
            if (grille[i][j] == valeur) {
                return false;  // Valeur déjà présente dans le bloc
            }
        }
    }

    return true;
}

void chargerGrille(tGrille g) {
    char nomFichier[30];
    FILE *f;
    printf("Nom du fichier ?\n");
    scanf("%s", nomFichier);
    f = fopen(nomFichier, "rb");

    if (f == NULL) {
        printf("\nERREUR sur le fichier %s\n", nomFichier);
    } else {
        fread(g, sizeof(int), TAILLE * TAILLE, f);
    }
    fclose(f);
}

void affichageGrille(tGrille g) {
    // affichage des numéros de colonne 
    printf("     ");
    for (int nb = 1; nb <= TAILLE; nb++){
        if(nb > 3 && nb % N == 3) //c'était 3 avant tqt
        {
            printf(" ");
        }
    }
    printf("\n");
    printf("    ");

    // affichage de la séparation du haut 
    for (int i = 0; i < N; i++)
    {
        printf("+");
        printf("--");
        for (int j = 0; j < N; j++)
        {
            printf("--");
        }    
        printf("-");   
    }
    printf("+");
    printf("\n");

    // boucle qui fait l'affichage chacune des lignes
    for (int i = 0; i < TAILLE; i++){
        printf("%2d  ",i+1);
        printf("|");

        // boucle qui affiche chacune des colonnes
        for (int j = 0; j < TAILLE; j++) {
            printf(" %d ", g[i][j]);
            if ((j+1) % N == 0){ // si le reste de la division de j par 3 donne 0, on affiche un pipe pour séparer les blocs de 3 par 3
                printf("|");
            }
        }
        printf("\n");

        // séparations avec + et - entre les blocs de 3 par 3
        if ((i+1) % N == 0 && i+1 !=TAILLE) 
        {
            printf("    ");
            for (int i = 0; i < N; i++)
            {
                printf("+");
                printf("--");
                for (int j = 0; j < N; j++)
                {
                    printf("--");
                }    
                printf("-");   
    }
    printf("+");
    printf("\n");
        } 
    }
    // affichage du dernier séparateur avec + et -
    printf("    ");
    for (int i = 0; i < N; i++)
    {
        printf("+");
        printf("--");
        for (int j = 0; j < N; j++)
        {
            printf("--");
        }    
        printf("-");   
    }
    printf("+");
    printf("\n");

}

void resolverSudoku(tGrille grille) {
    bool progression = true;

    while (progression) {
        progression = false;

        for (int i = 0; i < TAILLE; i++) {
            for (int j = 0; j < TAILLE; j++) {
                if (grille[i][j] == 0) {
                    tCase2 Case;
                    initTab(&Case);

                    for (int compt = 1; compt <= TAILLE; compt++) {
                        if (estCandidats(grille, compt, i, j)) {
                            ajouterCandidats(&Case, compt);
                        }
                    }

                    if (Case.nbCandidats == 1) {
                        grille[i][j] = Case.valeur;
                        progression = true;
                    }
                }
            }
        }
    }
}











