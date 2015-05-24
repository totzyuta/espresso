#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "define.h"

TokenSt* nextToken(FILE *fp);
void ungetToken(void);
void printTree(Node *node);
void freeTree(Node *node);
Node* Array(Node *node, FILE *fp);
static Node *Stack[2][MaxStack];
static int Sptr[2]={0,0}; 

static OrderType orderMatrix[][5] = {
  // row:TopNode, column:Operator
/* ot_PlusMinus, ot_MultiDiv, ot_Lpar,  ot_Rpar,  ot_Dollar, ot_error */
  {greater,      less,        less,     greater,  greater             }, // ot_PlusMinus
  {greater,      greater,     less,     greater,  greater             }, // ot_MultiDiv
  {less,         less,        less,     equal,    uncorrect           }, // ot_Lpar 
  {greater,      greater,     uncorrect,greater,  greater             }, // ot_Rpar
  {less,         less,        less,     uncorrect,end                 }, // ;ot_Dollar
};

static OpeType typeToOpeType(TokenType type) {
  if (type == ADD || type == SUB) return ot_PlusMinus;
  if (type == MUL || type == DIV) return ot_MultiDiv;
  if (type == LPAREN) return ot_LPar;
  if (type == RPAREN) return ot_RPar;
  else return ot_Dollar;
}

void push(int S, Node *n) { 
  if (Sptr[S] == MaxStack) {
    fprintf(stderr, "Stack Overflow\n");
    exit(1);
  } else {
    Stack[S][Sptr[S]] = n;
    Sptr[S] = Sptr[S] + 1;
  }
}

Node* pop(int S) { 
  if (Sptr[S] == 0) {
    fprintf(stderr, "Stack Underflow\n");
    exit(1);
  } else {
    Sptr[S] = Sptr[S] - 1;
    return Stack[S][Sptr[S]];
  }
}

Node* Top() { 
  if (Sptr[1] == 0) {
    fprintf(stderr, "Empty Stack\n");
    exit(1);
  } else {
    return Stack[1][Sptr[1] - 1]; 
  }
}

int Check(Node *Operator) { 
  Node *N;
  Node *topNode;
  OrderType order;
  
  topNode = Top(); 
  order = orderMatrix[typeToOpeType(topNode->token->type)][typeToOpeType(Operator->token->type)];
  
  if (order == end){ 
    pop(1);
    return 1; 
  }

  if (order == uncorrect) { 
    ungetToken();
    return 1;
  }

  if (Sptr[1] == 0 || order == less) {
    push(1, Operator); 
    return 0; 
  } else if (topNode->token->type == LPAREN) {  
    pop(1); 
    return 0; 
  } else { 
    N = pop(1);
    N->right = pop(0);
    N->left = pop(0);
    push(0, N);
    return Check(Operator); 
  }
}


Node* Oparser(FILE *fp){
  TokenSt *token;
  Node *node,*temp,*node2;
  int final; 
  TokenType tmp = DOLLAR;

  final = 0;

  node = (Node *)malloc(sizeof(Node));
  node->token = (TokenSt *)malloc(sizeof(TokenSt));
  node->token->type = DOLLAR;
  node->token->string[0] = '$';
  node->token->string[1] = '\0';
  Stack[1][Sptr[1]] = node;   
  Sptr[1] = Sptr[1] + 1;

  while (final == 0) { 
    token = nextToken(fp);
    node = (Node *)malloc(sizeof(Node));
    node->token = token;
    node->left = NULL;
    node->right = NULL;

    if (node->token->type == LCURLY || node->token->type == RCURLY || node->token->type == EQUAL2 ||node->token->type == GREATER ||node->token->type == LESS || node->token->type ==  SEMICOLON || node->token->type == COMMA) {

      ungetToken();
      tmp = node->token->type;
      node->token->type = DOLLAR;
      } 

    if (node->token->type == RSQUARE){
      
      tmp = node->token->type;
      node->token->type = DOLLAR;
    }

    if (node->token->type == INTEGER || node->token->type == IDENT) {
    
      node2 = (Node *)malloc(sizeof(Node));
      node2->token = (TokenSt *)malloc(sizeof(TokenSt));

        node2->token = nextToken(fp);
        
        if(node2-> token->type == LSQUARE){

        	node = Array(node,fp);
        }else{
            ungetToken();
        }
      push(0, node);
      } else { 
      final = Check(node);
    }

    
  }
if (node->token->type == DOLLAR && tmp != RSQUARE){
    
    node->token->type = tmp;
 }
 temp=pop(0);
  printTree(temp);
  return temp;
}

void printTree(Node *node){
   if (node->left != NULL || node->right != NULL){
       printTree(node->left);
       printTree(node->right);
   }
   printf("%s ",node->token->string);
 }


void freeTree(Node *node) {
  if (node->left != NULL || node->right != NULL) {
    freeTree(node->left);
    freeTree(node->right);
  }
  free(node->token);
  free(node);
}

Node* Array(Node *node,FILE *fp){

	 TokenSt *token2,*token3; 
   token2 = nextToken(fp);
   if(token2->type == INTEGER || token2->type == IDENT ){
   	    strcat(node->token->string,"[");
   	    strcat(node->token->string,token2->string);
        token3 = nextToken(fp);
        if(token3->type == LSQUARE){

       	  node = Array(node,fp);
       	  token3 = nextToken(fp);
       	  if(token3->type == RSQUARE){
       	    strcat(node->token->string,"]");	
       	  }
       	token3 = nextToken(fp);
       	if(token3->type == LSQUARE){
       		node = Array(node,fp);

       	}else{
       		ungetToken();
       	}
        }else{

        if(token3->type == RSQUARE){
           
        strcat(node->token->string,"]");
        }

        token3 = nextToken(fp);
        if(token3->type == LSQUARE){
       	node = Array(node,fp);
        }else{
       	ungetToken();
       }
   }

  }
    return node;        
}
