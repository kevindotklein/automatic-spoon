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
  return &(node_t){
    .type = type,
  };
}

node_t* random_tree(int depth) {
  if(depth == 0) {
    if(frand(0.L, 1.L) < 0.05) {
      node_t* n = new_node(VAR);
      return n;
    }else {
      node_t* n = new_node(CONST);
      n->value = (rand() % 5) - 2;
      return n;
    }
    
    node_t* n = new_node(OP);
    char ops[3] = {'+', '-', '*'};
    n->op = ops[rand() % 3];

    n->left = random_tree(depth - 1);
    n->right = random_tree(depth - 1);
    return n;
  }
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
  double err = 0.L;
  for(double x = -2; x <= 2; x+=1) {
    double d = eval(n, x) - target(x);
    err += d*d;
  }

  return err/5.L;
}

node_t* copy_tree(node_t* n) {
  if(!n) return NULL;
  return &(node_t){
    .op = n->op,
    .value = n->value,
    .left = copy_tree(n->left),
    .right = copy_tree(n->right),
  };
}

void mutate(node_t* n) {
  if(frand(0.L, 1.L) < MU_RATE) {
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
  print(n->left);
  printf(" %c ", n->op);
  print(n->right);
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

  for(int gen=0; gen<30; gen++) {

    for(int i=0; i<POP; i++) {
      fit[i] = mse(pop[i]);
    }

    int best = 0;
    for(int i=0; i<POP; i++) {
      if(fit[i] < fit[best]) best = i;
    }

    printf("gen: %d | better fit(MSE): %.4f | expr: ", gen, fit[best]);
    print_tree(pop[best]);
    printf("\n");
  }
  
	return 0;
}
