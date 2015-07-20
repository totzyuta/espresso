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
  // DEBUG
  printf("Oparser called!\n");

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

    if (node->token->type == LCURLY || node->token->type == RCURLY || node->token->type == EQUAL2 ||node->token->type == GREATER ||node->token->type == LESS || node->token->type == EQGREATER || node->token->type == EQLESS || node->token->type == NEQUAL || node->token->type ==  SEMICOLON || node->token->type == COMMA) {

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
        if(node->token->type == INTEGER){
          printf("error\n");
          exit(1);
        }
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
  return temp;
}

void printTree(Node *node){
  if (node->left != NULL && node->right != NULL){
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
      if(token2->type == INTEGER){
        printf("配列の前 整数\n");
        exit(1);
      }

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



/*
 * ここ以降、算術式のコード生成の処理
 */

int first_flag=0; // 最初かどうか(v0に格納するかどうか)を判断するフラッグ。どの深さかカウントもできる.0のときトップノードなのでv0に格納する
void enable_t(char *tn);
int available_t();
int used_t[7];
char *Tn; // 一時的に値(文字列)を格納しとく場所
void print_arithmetic(char *str0, char *str1, char *str2, int type/*, FILE *wfp*/);

void print_oparser(Node *node/*, FILE *wfp*/){ // `wfp` is for writing assembly program
  char *N1, *N2, *INPUT = NULL;

  if (node->left != NULL && node->right != NULL){
    first_flag++; 
    print_oparser(node->left/*, wfp*/);
    N1 = (char *)malloc(sizeof(char));
    strcpy(N1, Tn); // ひとつめのoperandをN1に代入
    print_oparser(node->right/*, wfp*/);
    N2 = (char *)malloc(sizeof(char));
    strcpy(N2, Tn); // ふたつめのoperandをN2に代入
    first_flag--;
    if(first_flag == 0) {
      Tn = "v0"; // 最後だったらv0に計算結果を入れる
      enable_t(N1);
    }else {
      Tn = N1; // 計算結果はN1に入れて上書きする
    }
    // print_nop(wfp);
    print_arithmetic(Tn, N1, N2, node->token->type/*, wfp*/);
  }else {
    INPUT = (char *)malloc(sizeof(char));
    if (first_flag != 0) {
      sprintf(INPUT, "t%d", available_t()); // 使えるtレジスタを検索して値を返す
    }else {
      INPUT = "v0"; // first_flagが0ならv0
    }
    Tn = INPUT;
    if(node->token->type == IDENT) {
      printf("la $t7, _%s\n", node->token->string); // load addressのとき$t7に。.dataでラベル付けして参照できるようにする
      printf("lw $%s, 0($t7)\n", INPUT);
    }else if(node->token->type == INTEGER){
      printf("ori $%s, $zero, %s\n", INPUT, node->token->string); // 直接入れる処理がないのでoriで代用　
    }
  }
  Tn = INPUT;
}

void init_used_t() {
  for(int i=0; i<7; i++) {
    used_t[i] = 0;
  }
}

int available_t() {
  int i;
  for(i=0; i<7; i++) {
    if(used_t[i] == 0) {
      used_t[i] = 1;
      return i;
    }
  }
  return -1;
  exit(1); /* ERROR */
}

void enable_t(char *tn) {
  // if(dbg2 == 1) {
  //   printf("used_t %d\n", used_t[5]);
  //   printf("tn : %s\nn : %d\n", tn, tn[1]-'0');
  // }
  if(strcmp(tn, "v0") != 0) { // tnが"v0"でないとき
    used_t[tn[1] - '0'] = 0;
  }
  // if(dbg2 == 1) {
  //   printf("used_t %d\n", used_t[5]);
  // }
}

void print_arithmetic(char *arg1, char *arg2, char *arg3, int token_type) {
  printf(">>> print_arithmetic called!!!");
  /*
  switch (token_type) {
    case 9:
      printf("add $%s, $%s, $%s\n", arg1, arg2, arg3);
      break;
    case 10:
      printf("sub $%s, $%s, $%s\n", arg1, arg2, arg3);
      break;
    case 11:
      // printf("mul $%s, $%s, $%s\n", arg1, arg2, arg3);
      printf("mult $%s, $%s\n" ,arg2, arg3);
      printf("mflo $%s\n",arg1);
      break;
    case 12:
      // printf("div $%s, $%s, $%s\n", arg1, arg2, arg3);
      printf("div $%s, $%s\n",arg2, arg3);
      printf("mflo $%s\n",arg1);
      break;
    default:
      break;
  }*/
  if(token_type == ADD) {
    printf("add $%s, $%s, $%s\n", arg1, arg2, arg3);
  }
}
