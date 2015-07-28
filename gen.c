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
void gen_operation(char *str0, char *str1, char *str2, int type);

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
    gen_operation(Tn, N1, N2, node->token->type);
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

void gen_operation(char *arg1, char *arg2, char *arg3, int token_type) {
  printf("nop\t\t\t\t# (delay slot)\n");
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


// variables for generating in parsing 
extern struct symbol_table symbol[100];
extern int add;

// generate initialize process 
void gen_code_initializer() {
  printf("\tINITIAL_GP = 0x10008000\n");
  printf("\tINITIAL_SP = 0x7ffffffc\n");
  printf("\t# system call service number\n\n");
  printf("\tstop_service = 99\n");
  printf("\t.text\n\t# 初期化ルーチン\n");
  printf("init:\n");
  printf("\t# initialize $gp (global pointer) and $sp (stack pointer)\n");
  printf("\tla\t$gp, INITIAL_GP\t\t# $gp <- 0x10008000 (INITIAL_GP)\n");
  printf("\tla\t$sp, INITIAL_SP\t\t# $sp <- 0x7ffffffc (INITIAL_SP)\n");
  printf("\tjal\tmain\t\t\t# jump to `main'\n");
  gen_code_nop();
  printf("\tli\t$v0, stop_service\t# $v0 <- 99 (stop_service)\n");
  printf("\tsyscall\t\t\t\t# stop\n");
  gen_code_nop();
  printf("\t# not reach here\n");
  printf("stop:\t\t\t\t\t# if syscall return\n");
  printf("\tj stop\t\t\t\t# infinite loop...\n");
  gen_code_nop();
  printf("\n\t.text\t0x00001000\n");
  printf("main:\n");
}

// data segment for variables
void gen_code_data_segment() {
  struct symbol_table *table = symbol;
  printf("\t.data 0x10004000\n");
  int i;
  for (i=0; i<add; i++) {
    printf("_%s:\t.word 0x0000\n", table->id);
    table++;
  }
}

// I don wanna do anything
void gen_code_nop(){
  printf("nop\t\t\t\t# (delay slot)\n");
}
