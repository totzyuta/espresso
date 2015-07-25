// Code Generator

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * 算術式のコード生成の処理
 */
int first_flag=0; // 最初かどうか(v0に格納するかどうか)を判断するフラッグ。どの深さかカウントもできる.0のときトップノードなのでv0に格納する
void enable_t(char *tn);
int available_t();
int used_t[7];
char *Tn; // 一時的に値(文字列)を格納しとく場所
void print_arithmetic(char *str0, char *str1, char *str2, int type);

void gen_code_operation(Node *node){ 
  // char *N1, *N2, *INPUT = NULL;
  char INPUT[100], N1[100], N2[100];

  if (node->left != NULL && node->right != NULL){
    first_flag++; 
    gen_code_operation(node->left);
    // N1 = (char *)malloc(sizeof(char));
    strcpy(N1, Tn); // ひとつめのoperandをN1に代入
    gen_code_operation(node->right);
    // N2 = (char *)malloc(sizeof(char));
    strcpy(N2, Tn); // ふたつめのoperandをN2に代入
    first_flag--;
    if(first_flag == 0) {
      Tn = "v0"; // 最後だったらv0に計算結果を入れる
      enable_t(N1);
    }else {
      Tn = N1; // 計算結果はN1に入れて上書きする
    }
    print_arithmetic(Tn, N1, N2, node->token->type);
  }else {
    // INPUT = (char *)malloc(sizeof(char));
    if (first_flag != 0) {
      sprintf(INPUT, "t%d", available_t()); // 使えるtレジスタを検索して値を返す
    }else {
      // INPUT = "v0"; // first_flagが0ならv0
      sprintf(INPUT, "v0");
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
  exit(1); // ERROR
}

void enable_t(char *tn) {
  if(strcmp(tn, "v0") != 0) { // tnが"v0"でないとき
    used_t[tn[1] - '0'] = 0;
  }
}

void print_arithmetic(char *arg1, char *arg2, char *arg3, int token_type) {
  switch (token_type) {
    case 9:
      printf("add $%s, $%s, $%s\n", arg1, arg2, arg3);
      break;
    case 10:
      printf("sub $%s, $%s, $%s\n", arg1, arg2, arg3);
      break;
    case 11:
      printf("mult $%s, $%s\n" ,arg2, arg3);
      printf("mflo $%s\n",arg1);
      break;
    case 12:
      printf("div $%s, $%s\n",arg2, arg3);
      printf("mflo $%s\n",arg1);
      break;
    default:
      break;
  }
}
