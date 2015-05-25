#include <stdio.h>
#include <stdlib.h>	/* free() を使う時に必要 */
#include "define.h"

TokenSt *nextToken(FILE *fp);
// Node *node(FILE *fp);
Node *Oparser(FILE *fp);


/* 使い方表示関数のプロトタイプ宣言                */
/* このファイルでしか使わないので staticにしている */
static void usage(char *s);

int main(int argc, char *argv[]){
  FILE		*fp;
  TokenSt	*token;
  Node *node;

  /*--< ファイル名を指定しないとエラー >--*/
  if (argc != 2 ) usage(argv[0]);
  
  /*--< ファイルをオープンする >--*/
  if ((fp = fopen(argv[1],"r")) == NULL){
    fprintf(stderr,"No such file (%s)\n",argv[1]);
    exit(1);
  }

  // Parsing operators and operands
  node = Oparser(fp);  
  printTree(node);
  printf("\n");
  freeTree(node);

  fclose(fp);

  // printf("Program Ends! Bye :)");

  return 0;
}

/*--< このプログラムの説明を表示 >--*/
static void usage(char *s){
  fprintf(stderr, "Usage: %s < filename >\n",s);
  exit(1);
}
