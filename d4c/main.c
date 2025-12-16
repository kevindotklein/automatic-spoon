#include <math.h>
#include <time.h>

#include "node.h"
#include "shared.h"

#define POP 50
#define MAX_DEPTH 4

// f(x) = x^2 - x + 1
double target(double x) {
  return x*x - x + 1;
}

double mse(node_t* n) {
  double err = 0.;
  for(double x = -2; x <= 2; x+=1) {
    double d = eval(n, x) - target(x);
    err += d*d;
  }

  return err/5.;
}

node_t* tournament(node_t** pop, double* fit) {
  int a = rand() % POP;
  int b = rand() % POP;
  return fit[a] < fit[b] ? pop[a] : pop[b];
}

int main(void) {
  //srand(time(NULL));
  srand(1);

  node_t* pop[POP];
  double fit[POP];

  for(int i=0; i<POP; i++) {
    pop[i] = random_tree(MAX_DEPTH);
  }

  for(int gen=0; gen<5000; gen++) {

    for(int i=0; i<POP; i++) {
      fit[i] = mse(pop[i]);
    }

    int best = 0;
    for(int i=0; i<POP; i++) {
      if(fit[i] < fit[best]) best = i;
    }

    printf("gen: %d | better fit(MSE): %.4f | expr: ", gen+1, fit[best]);
    print_tree(pop[best]);
    printf("\n");
    if(fit[best] == 0.) return 0;

    node_t* new_pop[POP];
    for(int i=0; i<POP; i++) {
      node_t* a = tournament(pop, fit);
      node_t* b = tournament(pop, fit);

      node_t* ac = copy_tree(a);
      node_t* bc = copy_tree(b);

      crossover(ac, bc);
      mutate(ac);

      new_pop[i] = ac;
    }

    for(int i=0; i<POP; i++) {
      pop[i] = new_pop[i];
    }
  }
  return 0;
}
