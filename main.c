#include <stdio.h>
#include <stdlib.h>
#include "define.h"

/* 使い方表示関数のプロトタイプ宣言                */
/* このファイルでしか使わないので staticにしている */
static void usage(char *s);
Node *parse_program(FILE *fp);


int main(int argc, char *argv[]){
  FILE		*fp;

  // ファイル名を指定しないとエラー 
  if (argc != 2 ) usage(argv[0]);

  // ファイルをオープンする
  if ((fp = fopen(argv[1],"r")) == NULL){
    fprintf(stderr,"No such file (%s)\n",argv[1]);
    exit(1);
  }
  
  // プログラムを解析してアセンブリプログラムを出力する
  parse_program(fp);
}


/*--< このプログラムの説明を表示 >--*/
static void usage(char *s){
  fprintf(stderr, "Usage: %s < filename >\n",s);
  exit(1);
}
