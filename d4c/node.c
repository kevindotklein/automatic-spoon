#include "node.h"

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
    } else if (frand(0., 1.) < 0.1) {
      node_t* n = new_node(NEG_VAR);
      return n;
    } else {
      node_t* n = new_node(CONST);
      n->value = (rand() % 5) - 2;
      return n;
    }
  }
    
  node_t* n = new_node(OP);
  char ops[4] = {'+', '-', '*', '^'};
  n->op = ops[rand() % 4];

  n->left = random_tree(depth - 1);
  n->right = random_tree(depth - 1);
  return n;
}

double eval(node_t* n, double x) {
  if(n->type == VAR) return x;
  if(n->type == NEG_VAR) return -x;
  if(n->type == CONST) return n->value;

  double a = eval(n->left, x);
  double b = eval(n->right, x);

  switch(n->op) {
    case '+': return a + b;
    case '-': return a - b;
    case '*': return a * b;
    case '^': return pow(a, b);
  }

  return 0;
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
      char ops[4] = {'+', '-', '*', '^'};
      n->op = ops[rand() % 4];
    } else if(n->type == CONST) {
      // -2 .. 2
      n->value = (rand() % 5) - 2;
    }
  }

  if(n->left) mutate(n->left);
  if(n->right) mutate(n->right);
}

void crossover(node_t* a, node_t* b) {
  if(frand(0., 1.) > CROSS_RATE) return;
  node_t* temp = a->left;
  a->left = b->left;
  b->left = temp;
}

void print_tree(node_t* n) {
  if (n->type == VAR) { printf("x"); return; }
  if (n->type == NEG_VAR) { printf("-x"); return; }
  if (n->type == CONST) { printf("%.2f", n->value); return; }

  printf("(");
  print_tree(n->left);
  printf(" %c ", n->op);
  print_tree(n->right);
  printf(")");
}
