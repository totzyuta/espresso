#include <stdio.h>
#include <stdlib.h>	/* free() を使う時に必要 */
#include "define.h"

/* 字句解析関数のプロトタイプ宣言                                       */
/* この関数は,字句を切り出して,TokenSt構造体に入れて,そのポインタを返す */
/* これ以上字句がない時は NULLを返す                                    */
/* なお，構造体を保持するメモリは,内部で mallocしている                 */
// lex.cで定義
TokenSt *nextToken(FILE *fp);


/* 使い方表示関数のプロトタイプ宣言                */
/* このファイルでしか使わないので staticにしている */
static void usage(char *s);


int main(int argc, char *argv[]){
  FILE		*fp;
  TokenSt	*token;

  /*--< ファイル名を指定しないとエラー >--*/
  if (argc != 2 ) usage(argv[0]);
  
  /*--< ファイルをオープンする >--*/
  if ((fp = fopen(argv[1],"r")) == NULL){
    fprintf(stderr,"No such file (%s)\n",argv[1]);
    exit(1);
  }

  int i=0; // for debugging
  /*--< 字句を順番に取り出す )--*/
  // while ( (token = nextToken(fp)) != NULL ){
  while ( (token = nextToken(fp)) != NULL ){
  // while (i<3){
    // debug
    i++;
    printf("----------------\n", i);
    printf("   %d times     \n", i);
    printf("----------------\n", i);
    if (token != NULL) 
      printf("Token is not NULL.\n");

    token = nextToken(fp);
    /*--< 字句の情報を表示する >--*/
    printf("Type:%02d  String:[%s]\n",token->type, token->string);
    /*--< 不必要になったら,ちゃんと解放する >--*/
    free(token); 
  }
  
  /*--< 後始末 >--*/
  fclose(fp);

  printf("Program Ends! Bye :)");

  return 0;
}

/*--< このプログラムの説明を表示 >--*/
static void usage(char *s){
  fprintf(stderr, "Usage: %s < filename >\n",s);
  exit(1);
}

