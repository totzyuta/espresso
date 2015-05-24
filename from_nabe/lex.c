#include <stdio.h>
#include <stdlib.h>	/* malloc() を使う時に必要 */
#include <string.h>
#include "define.h"

int flag=0;

// oparser.cで使用
// 1字句余計に読み込んでいるかどうかを覚えておくための関数
void ungetToken(void) {
  flag = 1;
}

static StateType table[8][8] = {
  /* delim,   number,   alpha,      brackets,   eq,         sign,       excl,     error */
  {  Final,   Int,      Identifer,  Relational, Relational, Sign,       Not,      Final },  /* Init */
  {  Final ,  Int ,     Final,      Final,      Final,      Final,      Final,    Final },  /* Int */
  {  Final ,  Identifer,Identifer,  Final,      Final,      Final,      Final,    Final },  /* Identifer */
  {  Final ,  Final,    Final,      Final,      Relational2,Final,      Final,    Final },  /* Relational */
  {  Final ,  Final,    Final,      Final,      Relational, Final,      Final,    Final },  /* Relational2 */
  {  Final ,  Final,    Final,      Final,      NotEq,      Final,      Final,    Final },  /* Not */
  {  Final ,  Final,    Final,      Final,      Final,      Final,      Final,    Final },  /* NotEq */
  {  Final ,  Final,    Final,      Final,      Final,      Final,      Final,    Final }   /* Sign */
};

static CharType charToCharType(int c);
static TokenType whichTokenType(char *s, StateType state);

// 1字句余計に読み込んでいるときに返すトークンを格納する変数
static TokenSt *previous; 

TokenSt *nextToken(FILE *fp){
  // DEBUG
  // printf("nextToken() Called!\n");

  // 余計に読み込んでいるときは前のトークンを返す
  if (flag == 1) {
    flag = 0;
    return previous; // L:94で代入
  }
  
  // FIFO配列全体でトークンになるように
  // 文字を一文字ずつ格納する
  static char	FIFO[TOKENMAX];
  TokenSt	*token = NULL;
  // nstateは次のトークンの状態(nextstate)
  StateType	state, nstate;
  // 現在の状態を初期状態にリセット
  state = Init;

  // 一文字ずつ読んでいって
  // 終了状態まで遷移を行う
  int i=0;
  while (1) {
    // ファイルから1文字読み込む
    char c = getc(fp);

    // トークンの先頭の文字が空白、改行、タブじゃなくなるまでスキップ！
    while (i==0 && (c==' ' || c=='\n' || c=='\t')) {
      c = getc(fp);
    }
    // 配列の最後に格納
    FIFO[i] = c; 

    // トークンの最初がEOFならNULLを返す
    if (c == EOF)
      return NULL;

    // DEBUG
    printf("%d文字目は%cです\n", i, c);
    
    // tableをもとに次の状態に遷移
    nstate = table[state][charToCharType(c)];
    // DEBUG
    // printf("nstate:%d\n", nstate);
    // 終了状態はStateTypeのenumで8
    if (nstate==8) {
      // 読み込み終了前処理
      // i++;
      FIFO[i]='\0';
      ungetc(c, fp);
      break;
    }
    // 状態を更新
    state = nstate;
    i++;
  }


  /*
   * ここから終了状態になった時の処理
   */

  // 読み込んだトークンを保存するための構造体のメモリを確保
  token = (TokenSt *)malloc(sizeof(TokenSt));
  // tokenに情報を格納
  strcpy(token->string, FIFO);
  token->type = whichTokenType(token->string, state);

  // 1字句余計に読み込んでいるときに返すトークンを格納する変数
  previous = token;

  return token;
}


/*--< 文字を入力とし,文字の種類を対応する数字で返す関数 >--*/
// Ex. cが3のときはCharTypeのうちnumberを返すので、数値だとnumberは1を表す
static CharType charToCharType(int c){
  // DEBUG
  // printf("charToCharType() Called! Argument: %d \n", c);

  if ( (c>='0') && (c<='9') ) 
    return number;

  if ( ((c>='a') && (c<='z')) || ((c>='A') && (c<='Z')) ) 
    return alpha;

  if ( (c=='>') || (c=='<') ) 
    return brackets;

  if (c=='=') 
    return eq;

  if ( (c=='+') || (c=='-') || (c=='*') || (c=='/') || (c==';') || (c=='(') || (c==')') || (c=='{') || (c=='}') || (c=='[') || (c==']') ) 
    return sign;

  if (c=='!') 
    return excl;

  if((c==' ')||(c=='\n')||(c=='\t')) 
    return delim;

  return error;
}

/*--< (トークンの)文字列と直前の状態を入力とし,トークンの種類を返す関数 >--*/
static TokenType whichTokenType(char *s, StateType state){
  // debug
  // printf("whichTokenType() Called!\n");

  if (state == Int) return INTEGER;
  if (strcmp(s, "define") == 0) return DEFINE;
  if (strcmp(s, "if") == 0) return IF;
  if (strcmp(s, "while") == 0) return WHILE;
  if (strcmp(s, "func") == 0) return FUNC;
  if (strcmp(s, "return") == 0) return RETURN;
  if (state == Identifer) return IDENT;
  if (strcmp(s, "+") == 0) return ADD;
  if (strcmp(s, "-") == 0) return SUB;
  if (strcmp(s, "*") == 0) return MUL;
  if (strcmp(s, "/") == 0) return DIV;
  if (strcmp(s, "(") == 0) return LPAREN;
  if (strcmp(s, ")") == 0) return RPAREN;
  if (strcmp(s, "{") == 0) return LCURLY;
  if (strcmp(s, "}") == 0) return RCURLY;
  if (strcmp(s, "[") == 0) return LSQUARE;
  if (strcmp(s, "]") == 0) return RSQUARE;
  if (strcmp(s, "=") == 0) return EQUAL;
  if (strcmp(s, ">") == 0) return GREATER;
  if (strcmp(s, "<") == 0) return LESS;
  if (strcmp(s, ">=") == 0) return EQGREATER;
  if (strcmp(s, "<=") == 0) return EQLESS;
  if (strcmp(s, "!=") == 0) return NEQUAL;
  if (strcmp(s, "==") == 0) return EQUAL2;
  if (strcmp(s, ";") == 0) return SEMICOLON;
}
