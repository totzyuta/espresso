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

  /*----------------------------------------------------------------------*/
  // DEBUG
  printf("parsing IF...\n");
  token = nextToken(fp);                /* まず「if」を読み込む           */
  if (token->type != IF){               /* それが「if」でなければエラー   */
    fprintf(stderr,"Syntax Error\n");
    exit(1);
  }
  free(token);                          /* いらないので解放しておく       */
  /*----------------------------------------------------------------------*/
  // DEBUG
  printf("parsing LPAREN...\n");
  token = nextToken(fp);                /* 次に「(」を読み込む            */
  if (token->type != LPAREN){            /* それが左括弧でなければエラー   */
    fprintf(stderr,"Syntax Error\n");
    exit(1);
  }
  free(token);                          /* いらないので解放しておく       */
  /*----------------------------------------------------------------------*/
  // DEBUG
  printf("parsing Operation...\n");
  /* 次は算術式が入っているはず                                           */
  node = Oparser(fp);                   /* 算術式を読み込み，構造体を返す */
  //printTree(node);                      /* 構造体の中身を表示する         */
  //freeTree(node);                       /* 構造体のメモリを解放する       */
  printf("\n");
  /*----------------------------------------------------------------------*/
  // DEBUG
  printf("parsing '>'...\n");
  token = nextToken(fp);                /* 次に「＞」を読み込む           */
  if (token->type != GREATER){               /* それが「＞｝でなければエラー   */
    fprintf(stderr,"Syntax Error\n");
    exit(1);
  }
  free(token);                          /* いらないので解放しておく       */
  /*----------------------------------------------------------------------*/
  // DEBUG
  printf("parsing Operation2...\n");
  /* 次は算術式が入っているはず                                           */
  node = Oparser(fp);                   /* 算術式を読み込み，構造体を返す */
  //printTree(node);                      /* 構造体の中身を表示する         */
  //freeTree(node);                       /* 構造体のメモリを解放する       */
  printf("\n");
  /*----------------------------------------------------------------------*/
  // DEBUG
  printf("parsing RPAREN...\n");
  token = nextToken(fp);                /* 次に「)」を読み込む            */
  if (token->type != RPAREN){            /* それが右括弧でなければエラー   */
    fprintf(stderr,"Syntax Error\n");
    exit(1);
  }
  free(token);                          /* いらないので解放しておく       */
  /*----------------------------------------------------------------------*/
  token = nextToken(fp);                /* 代入される変数名を読み込む     */
  if (token->type != IDENT){            /* それが識別子でなければエラー   */
    fprintf(stderr,"Syntax Error\n");
    exit(1);
  }
  free(token);                          /* いらないので解放しておく       */
  /*----------------------------------------------------------------------*/
  token = nextToken(fp);                /* 次に代入記号「＝」を読み込む   */
  if (token->type != EQUAL){              /* それが代入記号でなければエラー */
    fprintf(stderr,"Syntax Error\n");
    exit(1);
  }
  free(token);                          /* いらないので解放しておく       */
  /*----------------------------------------------------------------------*/
  /* 次は算術式が入っているはず                                           */
  node = Oparser(fp);                   /* 算術式を読み込み，構造体を返す */
  //printTree(node);                      /* 構造体の中身を表示する         */
  //freeTree(node);                       /* 構造体のメモリを解放する       */
  printf("\n");
  /*----------------------------------------------------------------------*/
  token = nextToken(fp);                /* 最後のセミコロン「;」を読み込む*/
  if (token->type != SEMICOLON){        /* それが「;」でなければエラー    */
    fprintf(stderr,"Syntax Error\n");
    exit(1);
  }
  free(token);                          /* いらないので解放しておく       */
  /*----------------------------------------------------------------------*/

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
