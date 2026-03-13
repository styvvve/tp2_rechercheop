#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>


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

    knapsack_t problem = create_instance(50, 5000, 14, 0.3, 1e-5, 100);

    unsigned int *solution = malloc(problem.nbr * sizeof(unsigned int)); 

    // TODO
    printf("Le nombre total d'objets : %ld\n", problem.nbr); 
    printf("Capacité du sac à dos : %.2f\n", problem.capacity); 
    printf("La valeur totale des objets disponibles : %.2f\n", valeur_totale(problem)); 
    printf("Le poids total des objets disponibles : %.2f\n", poids_total(problem));
    
    free(solution); 

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

    unsigned int *solution = malloc(p.nbr * sizeof(unsigned int)); 

    //copie du tableau initial
    double values_cp[p.nbr]; 
    memcpy(values_cp, p.values, p.nbr * sizeof(double)); 

    tri_insertion(values_cp, p.nbr); 
}
