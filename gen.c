// Code Generator

#include <stdio.h>
#include "define.h"

Node *Oparser(FILE *fp);
void printTree(Node *node);

static int top = 0;

// 算術式のCode Generator
static void OparserCode(Node *node) {
  // stackを生成する
  printf(".space 400"); // 400バイト分のデータを確保する
  // レジスタを一つスタック専用ポインタにする
  // スタック領域を確保する
  // スタックに対するPOP, PUSHの動作をアセンブリで実現する
}
