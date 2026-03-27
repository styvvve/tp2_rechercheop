#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>


// Type définissant une instance du problème TSP
typedef struct {
    unsigned long nbr;
    double capacity;
    double *values;
    double *weights;
} knapsack_t;

// Problème du restaurant
double prices[80] = {
    391.75, 388.25, 399.25, 383.25, 389.25, 381.00, 378.50, 384.00, 385.00,
    390.25, 394.25, 396.00, 385.50, 389.25, 382.50, 397.25, 376.75, 377.50,
    384.25, 381.50, 390.50, 382.50, 381.25, 378.75, 383.25, 379.75, 394.75,
    378.25, 384.50, 388.00, 399.50, 378.75, 383.50, 389.75, 379.25, 378.75,
    384.25, 388.25, 378.25, 382.25, 386.50, 389.50, 375.50, 387.00, 385.75,
    389.25, 386.00, 398.75, 398.75, 386.50, 397.50, 385.50, 377.00, 384.75,
    391.25, 392.00, 204.25, 196.75, 212.25, 190.00, 209.50, 207.50, 209.75,
    202.25, 207.50, 212.25, 204.50, 199.00, 208.75, 199.00, 201.50, 191.00,
    210.00, 210.75, 197.25, 194.75, 204.25, 189.50, 209.25, 210.25 };
double addition = 14071.;


// Fonctions à écrire pendant le TP
double valeur(knapsack_t p, unsigned int *solution); 
unsigned int * greedy1(knapsack_t p); 
unsigned int * greedy2(knapsack_t p);
unsigned int * greedy3(knapsack_t p);
unsigned int * eager_thief(knapsack_t p, int nsteps);
unsigned int * recuit(knapsack_t p, double temp, double seuil, double loi);

// Mettre ici le code des fonctions du TP
// TODO

double valeur_totale(knapsack_t p) {
    double valeur = 0; 

    for (int i = 0; i < p.nbr; i++) {
        valeur += p.values[i]; 
    }

    return valeur; 
}

double poids_total(knapsack_t p) {
    double poids = 0; 

    for (int i = 0; i < p.nbr; i++) {
        poids += p.weights[i];
    }

    return poids; 
}

double poids_obj_sac(knapsack_t p, unsigned int *solution, int taille) {
    double poids = 0.0; 

    for (int i = 0; i < taille; i++) {
        poids += p.weights[solution[i]]; 
    }

    return poids; 
}





// Fonction fournie au début du TP

knapsack_t create_instance(unsigned long int n, unsigned long int cap,
                           unsigned long int classes, double frac,
                           double eps, unsigned long int small)
{
    knapsack_t p;
    p.nbr = n;
    classes--;

    p.values = malloc(n*sizeof(double));
    p.weights = malloc(n*sizeof(double));

    unsigned long int amountSmall = n*frac;
    unsigned long int am1 = (n-amountSmall)/classes;
    double denominator = 2.0;
    unsigned int amountCtr = 0;
    for(int j=0; j<classes; j++)
    {
        for(int i=0; i<am1; i++)
        {
            unsigned long int num1 = rand() % small + 1;
            unsigned long int num2 = rand() % small + 1;
            p.values[amountCtr] = (int)((1/denominator+eps)*cap+num1);
            p.weights[amountCtr] = (int)((1/denominator+eps)*cap+num2);
            amountCtr++;
        }
        denominator*=2;
    }
    for(int i=amountCtr; i<n; i++)
    {
        int num1 = rand() % small + 1;
        int num2 = rand() % small + 1;
        p.values[i] = (int)((1/denominator+eps)*cap+num1);
        p.weights[i] = (int)((1/denominator+eps)*cap+num2);
    }
    p.capacity = (double) cap;
    return p;
}


int main(void) {
    srand(time(NULL));
    srand(2004); 

    knapsack_t problem = create_instance(120, 100000, 4, 0.5, 0.1, 100);

    unsigned int *solutionGreedy1 = greedy1(problem); 
    unsigned int *solutionGreedy2 = greedy2(problem); 
    unsigned int *solutionGreedy3 = greedy3(problem); 


    // TODO
    printf("Le nombre total d'objets : %ld\n", problem.nbr); 
    printf("Capacité du sac à dos : %.2f\n", problem.capacity); 
    printf("La valeur totale des objets disponibles : %.2f\n", valeur_totale(problem)); 
    printf("Le poids total des objets disponibles : %.2f\n", poids_total(problem));

    printf("Valeur de greedy 1 : %.2f\n", valeur(problem, solutionGreedy1));
    printf("Valeur de greedy 2 : %.2f\n", valeur(problem, solutionGreedy2)); 
    printf("Valeur de greedy 3 : %.2f\n", valeur(problem, solutionGreedy3)); 

    
    free(solutionGreedy1); 
    free(solutionGreedy2); 
    free(solutionGreedy3); 

    return 0; 
}

double valeur(knapsack_t p, unsigned int *solution) {

    double valeur = 0; 

    //parcourir le tableau solution jusqu'à tomber sur p.nbr
    unsigned long i = 0; 
    while ((i < p.nbr) && (solution[i] != p.nbr)) {
        //pour chaque elt du tableau ajouter sa valeur dans valeur(double)
        valeur += p.values[solution[i]]; 
        i++; 
    }
    return valeur; 

}

void tri_insertion(double tab[], int taille) {

    for (int i = 1; i < taille; i++) {
        double value = tab[i]; 

        //decaler vers la gauche tant que p.values[i] > p.values
        int j = i; 
        while (j > 0 && value > tab[j]) {
            double tmp = tab[i-1];
            tab[i-1] = value; 
            tab[i] = tmp; 
            j--; 
        }
    }
}


unsigned int * greedy1(knapsack_t p) {

    unsigned int *items = malloc(p.nbr * sizeof(unsigned int)); 
    unsigned int *solution = malloc(p.nbr * sizeof(unsigned int)); 
    unsigned int n = p.nbr; //valeur sentinelle

    for (int i = 0; i < p.nbr; i++) items[i] = i; 
    //tri insertion
    for (int i = 1; i < p.nbr; i++) {
        unsigned int x = items[i]; 

        //decaler vers la gauche tant que p.values[i] > p.values
        int j = i; 
        while ((j > 0) && (p.values[items[j-1]] < p.values[items[j]])) {
            items[j] = items[j-1]; 
            j--; 
        }
        items[j] = x; 
    }

    double w = 0; 
    int c = 0; 
    for (int i = 0; i < p.nbr; i++) {
       if ((w+p.weights[items[i]] < p.capacity)) {
        solution[c] = items[i]; 
        w += p.weights[items[i]]; 
        c++; 
       }
    }

    if (c < n) solution[c] = n; 
    free(items); 
    return solution; 
    
}

unsigned int * greedy2(knapsack_t p) {
    unsigned int *items = malloc(p.nbr * sizeof(unsigned int)); 
    unsigned int *solution = malloc(p.nbr * sizeof(unsigned int)); 
    unsigned int n = p.nbr; //valeur sentinelle

    for (int i = 0; i < p.nbr; i++) items[i] = i; 
    //tri insertion
    for (int i = 1; i < p.nbr; i++) {
        unsigned int x = items[i]; 

        //decaler vers la gauche tant que p.values[i] < p.values
        int j = i; 
        while ((j > 0) && (p.values[items[j-1]] > p.values[items[j]])) {
            items[j] = items[j-1]; 
            j--; 
        }
        items[j] = x; 
    }

    double w = 0; 
    int c = 0; 
    for (int i = 0; i < p.nbr; i++) {
       if ((w+p.weights[items[i]] < p.capacity)) {
        solution[c] = items[i]; 
        w += p.weights[items[i]]; 
        c++; 
       }
    }

    if (c < n) solution[c] = n; 
    free(items); 
    return solution; 
    
}

unsigned int * greedy3(knapsack_t p) {
    unsigned int *items = malloc(p.nbr * sizeof(unsigned int)); 
    unsigned int *solution = malloc(p.nbr * sizeof(unsigned int)); 
    unsigned int n = p.nbr; //valeur sentinelle

    for (int i = 0; i < p.nbr; i++) items[i] = i; 
    //tri insertion
    for (int i = 1; i < p.nbr; i++) {
        unsigned int x = items[i]; 

        //decaler vers la gauche 
        int j = i; 
        while ((j > 0) && ((p.values[items[j-1]]/p.weights[items[j-1]]) < (p.values[items[j]]/p.weights[items[j-1]]))) {
            items[j] = items[j-1]; 
            j--; 
        }
        items[j] = x; 
    }

    double w = 0; 
    int c = 0; 
    for (int i = 0; i < p.nbr; i++) {
       if ((w+p.weights[items[i]] < p.capacity)) {
        solution[c] = items[i]; 
        w += p.weights[items[i]]; 
        c++; 
       }
    }

    if (c < n) solution[c] = n; 
    free(items); 
    return solution; 
}

bool elt_present_sac(unsigned int elt, unsigned int* slt, int taille) {
    for (int i = 0; i < taille; i++) {
        if (slt[i] == elt) return true; 
        else continue; 
    }

    return false; 
}

//fonction qui va prendre un elt ds le sac et le decaler vers la gauche jusqu'à la position voulue
void super_swap_left(unsigned int elt, unsigned int *solution) {

}


/*
– le cambrioleur sélectionne un objet qu’il n’a pas encore mis dans son sac ;
– tant qu’il ne reste pas assez de place dans le sac pour y mettre l’objet sélectionné, on
retire successivement des objets au hasard ;
– dès qu’il y a suffisamment de place dans le sac, on y met l’objet sélectionné ;
– si la nouvelle valeur totale ainsi obtenue est inférieure à la valeur initiale du début de
l’étape, on rétablit la sélection précédente (sinon on poursuit avec ce nouveau choix).
 */
/*unsigned int * eager_thief(knapsack_t p, int nsteps) {
    unsigned int * solution = malloc(p.nbr * sizeof(unsigned int)); 
    unsigned int *deja_vus = malloc(p.nbr * sizeof(unsigned int));
    int nb_obj = 0; 

    for (int i = 0; i < p.nbr; i++) solution[i] = i; 
    for (int i = 0; i < p.nbr; i++) deja_vus[i] = 0; 
    int elt_deja_vus = 0; 

    for (int i = 0; i < nsteps; i++) {

        unsigned int elt_selectionne = solution[0]; //au hasard

        for (int j = 0; j < p.nbr; j++) {
            if ((!(elt_present_sac(solution[j], solution, nb_obj))) && p.capacity > (poids_obj_sac(p, solution, nb_obj)+p.weights[j]) && !elt_present_sac(solution[j])) {
                elt_selectionne = solution[j]; 
                deja_vus[elt_deja_vus] = solution[j]; 
                elt_deja_vus++; 

            }
        }
    }
    
}*/
