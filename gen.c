#include <stdio.h>
#include "define.h"

Node *Oparser(FILE *fp);
void printTree(Node *node);

static int top = 0;

static void OparserCode(Node *node) {
  if (node->left == NULL) {
    printf("(add, T%d, %s, 0)\n", top, node->token->string);
    top++;
  }else {
    OparserCode(node->left);
    OparserCode(node->right);
    printf("(%s, T%d, T%d, T%d)\n", node->token->string,
        top-2, top-2, top-1);
    top--;
  }
}

void Code(FILE *fp) {
  Node *node = Oparser(fp);
  printTree(node);
  printf("\n");
  OparserCode(node);
}
