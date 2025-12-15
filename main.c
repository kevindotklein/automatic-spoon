#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define POP 50
#define MAX_DEPTH 4
#define CROSS_RATE 0.9f
#define MU_RATE 0.05f

typedef enum { OP, VAR, CONST } node_type_t;

typedef struct node_t {
  node_type_t type;
  char op;
  double value;
  struct node_t* left;
  struct node_t* right;
} node_t;

double frand(double min, double max) {
  return min + (max - min) * ((double)rand() / RAND_MAX);
}

// f(x) = x^2 - x + 1
double target(double x) {
  return x*x - x + 1;
}

node_t* new_node(node_type_t type) {
  node_t* n = malloc(sizeof(node_t));
  n->type = type;
  n->left = n->right = NULL;
  return n;
}

node_t* random_tree(int depth) {
  if(depth == 0) {
    if(frand(0., 1.) < 0.1) {
      node_t* n = new_node(VAR);
      return n;
    }else {
      node_t* n = new_node(CONST);
      n->value = (rand() % 5) - 2;
      return n;
    }
  }
    
  node_t* n = new_node(OP);
  char ops[3] = {'+', '-', '*'};
  n->op = ops[rand() % 3];

  n->left = random_tree(depth - 1);
  n->right = random_tree(depth - 1);
  return n;
  
}

double eval(node_t* n, double x) {
  if(n->type == VAR) return x;
  if(n->type == CONST) return n->value;

  double a = eval(n->left, x);
  double b = eval(n->right, x);

  switch(n->op) {
    case '+': return a + b;
    case '-': return a - b;
    case '*': return a * b;
  }

  return 0;
}

double mse(node_t* n) {
  double err = 0.;
  for(double x = -2; x <= 2; x+=1) {
    double d = eval(n, x) - target(x);
    err += d*d;
  }

  return err/5.;
}

node_t* copy_tree(node_t* n) {
  if(!n) return NULL;
  node_t* c = new_node(n->type);
  c->op = n->op;
  c->value = n->value;
  c->left = copy_tree(n->left);
  c->right = copy_tree(n->right);
  return c;
}

void mutate(node_t* n) {
  if(frand(0., 1.) < MU_RATE) {
    if(n->type == OP) {
      char ops[3] = {'+', '-', '*'};
      n->op = ops[rand() % 3];
    } else if(n->type == CONST) {
      // -2 .. 2
      n->value = (rand() % 5) - 2;
    }
  }

  if(n->left) mutate(n->left);
  if(n->right) mutate(n->right);
}

node_t* tournament(node_t** pop, double* fit) {
  int a = rand() % POP;
  int b = rand() % POP;
  return fit[a] < fit[b] ? pop[a] : pop[b];
}

void crossover(node_t* a, node_t* b) {
  node_t* temp = a->left;
  a->left = b->left;
  b->left = temp;
}

void print_tree(node_t* n) {
  if (n->type == VAR) { printf("x"); return; }
  if (n->type == CONST) { printf("%.2f", n->value); return; }

  printf("(");
  print_tree(n->left);
  printf(" %c ", n->op);
  print_tree(n->right);
  printf(")");
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
