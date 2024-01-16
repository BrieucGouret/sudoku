#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define N 3
#define TAILLE (N*N)

typedef struct {
    int valeur;
    bool candidat[TAILLE + 1];
    int nbcandidat;
} tCase2;


typedef int tGrille[TAILLE][TAILLE];
typedef tCase2 tGrilleCase[TAILLE][TAILLE];

void afficherStats(int *nbCasesVides,int *nbCasesVidesAvantSolver, int *nombreDecandidat, int *nombreDecandidatAvantSolver,char *nomFichier);

void initCase(tCase2 *Case);

int initNbCaseVide(tGrille grille);

//prends la grille et regarde les différentas case vides
int nombreDecandidat(tGrilleCase grilleCase);

void initcandidat(tGrilleCase grilleCase);

void chargerGrille(tGrille grille,char *nomFichier);

//estcandidat teste si une valeur est possible à mettre
bool estcandidat(tGrilleCase grilleCase, int valeur, int numLignes , int numColonne  );

//l'ajout du candidat au tablo booléen
void ajoutercandidat(tCase2 *Case, int valeur);

void ajouterValeur(tCase2 *Case, tGrilleCase grilleCase, int numLigne, int numColonne);

//retire le candidat du tablo
void retirercandidat(tCase2 *Case, int valeur);

void retirercandidatColonne(tGrilleCase grilleCase, int numColonne, int valeur);
void retirercandidatBloc(tGrilleCase grilleCase, int numLigne, int numColonne, int valeur);
void retirercandidatLigne(tGrilleCase grilleCase, int numLigne, int valeur);

void afficherGrille(tGrille grille);

void affecterValeur(tGrille grille,tGrilleCase grilleCase);

void singletonCache(tGrille grille,tGrilleCase grilleCase, int numLigne, int numColonne, int valeur,int *nbCasesVides);

void singletonCacheLigne(tGrilleCase grilleCase, tCase2 *Case, int numLigne, int valeur);
void singletonCacheColonne(tGrilleCase grilleCase, tCase2 *Case, int numColonne, int valeur);
void singletonCacheBloc(tGrilleCase grilleCase, tCase2 *Case, int numLigne, int numColonne, int valeur);

int main(){
    tGrille grille;
    tGrilleCase grilleCase;
    char nomgrille[30];
    bool progression = true;
    int nbCasesVidesAvant;
    int nombrescandidat = 0,nbcandidatAvantSolver;
    int nbCasesVides,nbCasesVidesAvantSolver;
    int nbCaseRempli = 0 ;
    int toursSansAvance = 0;

    /*float tauxDeRemplissage;
    int candidatElimine;
    float pourcentageElemination;
    */
    chargerGrille(grille,nomgrille);
    // se deplace dans le tableau jusuqu'à trouvé une case qui à la valeur 0 et après test les candidat possisble et ajoute cela au tableau de booléen 
    // après re tester pour savoir les valeurs à enlever les candidat savoir les candidat uniques
    nbCasesVides = initNbCaseVide(grille);
    nbCasesVidesAvantSolver = initNbCaseVide(grille);
    printf(" nombre de case vides %d\n",nbCasesVides);
    printf("grille initial\n");
    afficherGrille(grille);
    affecterValeur(grille,grilleCase);
    initcandidat(grilleCase);
    
    nbcandidatAvantSolver = nombreDecandidat(grilleCase);
    printf("nb candidat %d\n",nombrescandidat);
    

    //se deplace dans la grille et cherche les cases qui n'ont que un seul candidat 
    while (nbCasesVides != 0 && progression == true && toursSansAvance < 5000) {
        nbCasesVidesAvant = nbCasesVides;

        for (int i = 0; i < TAILLE; i++) {
            for (int j = 0; j < TAILLE; j++) {

                if (grilleCase[i][j].nbcandidat == 1) {
                    ajouterValeur(&grilleCase[i][j], grilleCase, i, j);
                    grille[i][j] = grilleCase[i][j].valeur;
                    // Retirer les candidat dans la ligne, colonne et bloc
                    retirercandidatLigne(grilleCase, i, grilleCase[i][j].valeur);
                    retirercandidatColonne(grilleCase, j, grilleCase[i][j].valeur);
                    retirercandidatBloc(grilleCase, i, j, grilleCase[i][j].valeur);
                    nbCasesVides--;
                    nombrescandidat++;
                    nbCaseRempli++;
                    progression = true;
                }
                if (grilleCase[i][j].valeur == 0 && grilleCase[i][j].nbcandidat != 1)
                {
                    for (int val = 1; val <= TAILLE; val++) {
                        if (grilleCase[i][j].candidat[val] == true) {
                            singletonCache(grille, grilleCase, i, j, val , &nbCasesVides);
                        }
                    }
                }

                if (nbCasesVidesAvant == nbCasesVides) {
                    toursSansAvance++;
                } else {
                    toursSansAvance = 0;
                }
                if (toursSansAvance == 5000) {
                    progression = false;
                    printf("tours sans avance %d\n", toursSansAvance);
                }
                
  
            }
        }
        
    }
    
    printf("nb candidat %d\n",nbcandidatAvantSolver);
    printf("nb case vide %d\n", nbCasesVides);
    
    printf("fin de prog\n");
    afficherGrille(grille);
    
    afficherStats(&nbCaseRempli,&nbCasesVidesAvantSolver,&nombrescandidat,&nbcandidatAvantSolver,nomgrille);
    
}


void initCase(tCase2 *Case){
    for (int i = 1; i <= TAILLE; i++)
    {
        Case->candidat[i] = false;
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

void initcandidat(tGrilleCase grilleCase){
    bool nbEstcandidat;
    for (int i = 0; i < TAILLE; i++)
    {
        for (int j = 0; j < TAILLE; j++)
        {
            if (grilleCase[i][j].valeur == 0)
            {
                for (int compt = 1; compt <= TAILLE; compt++) {
                    nbEstcandidat = estcandidat(grilleCase, compt, i, j);
                    if (nbEstcandidat == true) {
                        ajoutercandidat(&grilleCase[i][j], compt);
                    }
                }
            }
            
            
        }
        
    }
     
}

int nombreDecandidat(tGrilleCase grilleCase){
    int sommecandidat = 0;
    for (int i = 0; i < TAILLE; i++)
    {
        for (int j = 0; j < TAILLE; j++)
        {
            sommecandidat += grilleCase[i][j].nbcandidat;
        }
        
    }
    
    return sommecandidat;
}

void ajoutercandidat(tCase2 *Case, int valeur){
    Case->candidat[valeur] = true;
    Case->nbcandidat++;
      
}

void retirercandidat(tCase2 *Case, int valeur){
    if (Case->candidat[valeur] == true) {
        Case->candidat[valeur] = false;
        Case->nbcandidat--;
    }
}

void retirercandidatColonne(tGrilleCase grilleCase, int numColonne, int valeur) {
    for (int i = 0; i < TAILLE; i++) {
        if (grilleCase[i][numColonne].valeur == 0) {
            retirercandidat(&grilleCase[i][numColonne], valeur);
            printf("candidat retirer %d a le place %d %d\n",valeur,i,numColonne);
        }
    }
}

void retirercandidatBloc(tGrilleCase grilleCase, int numLigne, int numColonne, int valeur) {
    int coinLigne = (numLigne / 3) * 3;
    int coinColonne = (numColonne / 3) * 3;

    for (int i = coinLigne; i < coinLigne + 3; i++) {
        for (int j = coinColonne; j < coinColonne + 3; j++) {
            if (grilleCase[i][j].valeur == 0) {
                retirercandidat(&grilleCase[i][j], valeur);
                printf("candidat retirer %d a le place %d %d\n",valeur,i,j);
            }
        }
    }
}

void retirercandidatLigne(tGrilleCase grilleCase, int numLigne, int valeur) {
    for (int j = 0; j < TAILLE; j++) {
        if (grilleCase[numLigne][j].valeur == 0) {
            retirercandidat(&grilleCase[numLigne][j], valeur);
            printf("candidat retirer %d a le place %d %d\n",valeur,numLigne,j);
        }
    }
}

bool estcandidat(tGrilleCase grilleCase, int valeur, int numLignes , int numColonne){
    int i, j, coinLigne, coinColonne;
    bool possible = true;
    for (j = 0; j < TAILLE; j++) {
        if (grilleCase[numLignes][j].valeur == valeur && j != numColonne) {
            possible = false;
        }
    }

    j = numColonne;
    i = 0;

    for (i = 0; i < TAILLE; i++) {
        if (grilleCase[i][numColonne].valeur == valeur && i != numLignes) {
            possible = false;
        }
    }

    coinLigne = (numLignes / 3) * 3;
    coinColonne = (numColonne / 3) * 3;

    for (int i = coinLigne; i < coinLigne + 3; i++) {
        for (int j = coinColonne; j < coinColonne + 3; j++) {
            if (grilleCase[i][j].valeur == valeur && (i != numLignes || j != numColonne)) {
                possible = false;
            }
        }
    }
    return possible;  
}

void affecterValeur(tGrille grille, tGrilleCase grilleCase) {
    for (int i = 0; i < TAILLE; i++) {
        for (int j = 0; j < TAILLE; j++) {
            if (grille[i][j] != 0) {
                grilleCase[i][j].valeur = grille[i][j];
                initCase(&grilleCase[i][j]);
            }else{
                grilleCase[i][j].valeur = 0;
                initCase(&grilleCase[i][j]);
            }
        }
    }

    
}

void ajouterValeur(tCase2 *Case, tGrilleCase grilleCase, int numLigne, int numColonne) {
    int i = 1;
    while (Case->candidat[i] != true && i <= TAILLE) {
        i++;
    }
    Case->valeur = i;
    retirercandidat(Case, i);

    // Mettre à jour la grilleCase et retirer les candidat associés à la nouvelle valeur
    grilleCase[numLigne][numColonne].valeur = Case->valeur;
    retirercandidatLigne(grilleCase, numLigne, Case->valeur);
    retirercandidatColonne(grilleCase, numColonne, Case->valeur);
    retirercandidatBloc(grilleCase, numLigne, numColonne, Case->valeur);
}


// Fonction singletonCache réintégrée
void singletonCache(tGrille grille, tGrilleCase grilleCase, int numLigne, int numColonne, int valeur, int *nbCasesVides) {
    int coinColonne, coinLigne;

    int nbcandidatTrouves = 0;

    // Vérifie dans la ligne
    for (int j = 0; j < TAILLE; j++) {
        if (j != numColonne && grilleCase[numLigne][j].candidat[valeur] == true) {
            nbcandidatTrouves++;
        }
    }

    // Vérifie dans la colonne
    for (int i = 0; i < TAILLE; i++) {
        if (i != numLigne && grilleCase[i][numColonne].candidat[valeur] == true) {
            nbcandidatTrouves++;
        }
    }

    coinLigne = (numLigne / 3) * 3;
    coinColonne = (numColonne / 3) * 3;

    // Vérifie dans le bloc
    for (int i = coinLigne; i < coinLigne + 3; i++) {
        for (int j = coinColonne; j < coinColonne + 3; j++) {
            if ((i != numLigne || j != numColonne) && grilleCase[i][j].candidat[valeur] == true) {
                nbcandidatTrouves++;
            }
        }
    }

    if (nbcandidatTrouves == 1 ) {
        
        // Retirer le candidat seulement dans la case correspondante
        for (int val = 1; val <= TAILLE; val++) {
            if (val != valeur && grilleCase[numLigne][numColonne].candidat[val] == true) {
                retirercandidat(&grilleCase[numLigne][numColonne], val);
            }
        }
        printf("Singleton trouvé en %d %d valeur = %d\n", numLigne, numColonne, valeur);
    }
}

void singletonCacheLigne(tGrilleCase grilleCase, tCase2 *Case, int numLigne, int valeur) {
    for (int j = 0; j < TAILLE; j++) {
        if (j != numLigne && grilleCase[numLigne][j].valeur == 0 && grilleCase[numLigne][j].candidat[valeur] == true) {
            retirercandidat(&grilleCase[numLigne][j], valeur);
        }
    }
}



void singletonCacheColonne(tGrilleCase grilleCase, tCase2 *Case, int numColonne, int valeur) {
    for (int i = 0; i < TAILLE; i++) {
        if (i != numColonne && grilleCase[i][numColonne].valeur == 0 && grilleCase[i][numColonne].candidat[valeur] == true) {
            retirercandidat(&grilleCase[i][numColonne], valeur);
        }
    }
}

void singletonCacheBloc(tGrilleCase grilleCase, tCase2 *Case, int numLigne, int numColonne, int valeur) {
    int coinLigne = (numLigne / 3) * 3;
    int coinColonne = (numColonne / 3) * 3;

    for (int i = coinLigne; i < coinLigne + 3; i++) {
        for (int j = coinColonne; j < coinColonne + 3; j++) {
            if ((i != numLigne || j != numColonne) && grilleCase[i][j].valeur == 0 && grilleCase[i][j].candidat[valeur] == true) {
                retirercandidat(&grilleCase[i][j], valeur);
            }
        }
    }
}




/*
void paireNues (tCase2 *Case){
    //regarde si il y a une paire
    int compt = 0;
    for (int i = 0; i < TAILLE; i++)
    {
        if (Case->candidat == true)
        {
            compt++;
        }
        
    }
    //rajouter le fait qu'il faut stocker qu'il y une 2 paire identiaues
    //rajouter que si compt > 2 ba on enleve du compteur de paire
    //supprimer cette paire du bloc, de la lignes ou de la colonne
    

}



*/

void afficherStats(int *nbCasesVides,int *nbCasesVidesAvantSolver,int *nombreDecandidat, int *nombreDecandidatAvantSolver,char *nomFichier){
    float tauxDeRemplissage = ((((float)*nbCasesVides) /  *nbCasesVidesAvantSolver) * 100);
    float pourcentageElemination = ((((float)*nombreDecandidat) /  *nombreDecandidatAvantSolver) * 100);
    printf("*********   RESULTATS POUR %s  ***********\n",nomFichier);
    printf("nombres de cases remplies = %d sur %d    Taux de remplissage = %.3f%% \n ",*nbCasesVides,*nbCasesVidesAvantSolver , tauxDeRemplissage);
    printf("nombres de candidat éliminé = %d sur %d     pourcentage de elimination =  %.3f%% \n",*nombreDecandidat,*nombreDecandidatAvantSolver,pourcentageElemination);
}

void chargerGrille(tGrille g,char *nomFichier) {
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

void afficherGrille(tGrille grille) {
    int i, j, compteur1, compteur2 = 1;
    printf("  ");
    for (compteur1 = 1; compteur1 < TAILLE + 1; compteur1++) {
        printf("%3d", compteur1);
        if (compteur1 == 6 || compteur1 == 3) {
            printf(" ");
        }
    }
    printf("\n  +---------+---------+---------+\n");
    for (i = 0; i < TAILLE; i++) {
        printf("%d |", compteur2);
        for (j = 0; j < TAILLE; j++) {
            if (grille[i][j] == 0) {
                printf(" . ");
            } else {
                printf("%2d ", grille[i][j]);
            }
            if (j == 2 || j == 5) {
                printf("|");
            }
        }
        printf("|\n");
        compteur2++;
        if (compteur2 == 4 || compteur2 == 7) {
            printf("  +---------+---------+---------+\n");
        }
    }
    printf("  +---------+---------+---------+\n");
}