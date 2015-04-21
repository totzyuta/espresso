#include <stdio.h>
#include <stdlib.h>	/* malloc() を使う時に必要 */
#include <string.h>
#include "define.h"

/*--< 遷移表の定義 >--*/
// static StateType table[11][11] = {
  // Original Program
  /* delim , number , alpha , kagen , joujo , brackets , eq , comp , colon , semi_c , error */ 
  // { Final , Int , Ident , Kagen , Joujo , Brackets , Eq1 , Comp , Colon , Semi_c , Error }/* Init */
  // { Final , Int , Final , Final , Final , Final , Final , Final , Final, Final ,Final }/* Int */
  // { Final , Ident , Ident  , Final , Final , Final , Final , Final , Final , Final , Final }/* Ident */
  // { Final , Final , Final , Final , Final , Final , Final , Final , Final , Final , Final }/* Kagen */
  // { Final , Final , Final , Final , Final , Final , Final , Final , Final , Final , Final }/* Joujo */
  // { Final , Final , Final , Final , Final , Final , Final , Final , Final , Final , Final }/* Brackets */
  // { Final , Final , Final , Final , Final , Final , Eq2 , Final , Final , Final , Final }/* Eq1 */
  // { Final , Final , Final , Final , Final , Final , Final , Final , Final , Final , Final }/* Eq2 */
  // { Final , Final , Final , Final , Final , Final , Final , Final , Final , Final , Final }/* Comp */
  // { Final , Final , Final , Final , Final , Final , Final , Final , Final , Final , Final }/* Colon */
  // { Final , Final , Final , Final , Final , Final , Final , Final , Final , Final , Final }/* Semi_c */
// };

// New Minimum Lexical Analyzer
static StateType table[2][3] = {
  // New minimum program
  /* delim,   number,   error */
  {  Final,   Int,      Final },/* Init */
  {  Final ,  Int ,     Final }/* Int */
};

static CharType charToCharType(int c);
static TokenType whichTokenType(char *s, StateType state);

TokenSt *nextToken(FILE *fp){
  // debug
  printf("nextToken() Called!\n");

  // TODO: ASK!
  static char	FIFO[TOKENMAX];
  TokenSt	*token = NULL;
  // nstateは次のトークンの状態(nextstate)
  StateType	state, nstate;
  // 現在の状態がなければ初期状態とする
  // TODO: ASK! 
  if (state)
    state = Init;

  // debug
  printf("- Before -\n");
  printf("token->type: %d \n", token->type);
  printf("token->string: %s \n", token->string);

  // 1文字ずつ読み込んで状態遷移する
  char c = getc(fp) ;
  // debug
  printf("Input Character: %c \n", c);
  // TODO: ASK!
  nstate = table[state][charToCharType(c)];

  // 終了状態になった時
  // 読み込んだトークンを保存するための構造体のメモリを確保
  token = (TokenSt *)malloc(sizeof(TokenSt));
  // tokenに情報を格納
  strcpy(token->string, FIFO);
  token->type = whichTokenType(token->string, state);

  // token->string[0] = c;
  // debug
  printf("- After -\n");
  printf("token->string: %s \n", token->string);
  printf("token->type: %i \n", token->type);

  return token;
}


/*--< 文字を入力とし,文字の種類を対応する数字で返す関数 >--*/
static CharType charToCharType(int c){
  // debug
  printf("charToCharType() Called!\n");

  if ((c>='0')&&(c<='9')) return number;
  // if (((c>='a')&&(c<='z'))||((c>='A')&&(c<='Z'))) return alpha;
  // if ((c=='*')||(c=='/')) return joujo;
  // if ((c=='>')||(c=='<')) return comp;
  // if (c=='=') return eq;
  // if ((c=='+')||(c=='-') return kagen;
  // if (c==';') return semi_c;
  // if ((c=='(')||(c==')')||(c=='{')||(c=='}')||(c=='[')||(c==']')) return brackets;
  if((c==' ')||(c=='\n')||(c=='\t')) return delim;
  return error;
}

/*--< (トークンの)文字列と直前の状態を入力とし,トークンの種類を返す関数 >--*/
static TokenType whichTokenType(char *s, StateType state){
  // debug
  printf("whichTokenType() Called!\n");

  if (state == Int) return INTEGER;
  /*
  if (strcmp(s, "define") == 0) return DEFINE;
  if (strcmp(s, "null") == 0) return NIL;
  if (strcmp(s, "if") == 0) return IF;
  if (strcmp(s, "while") == 0) return WHILE;
  if (state == Ident) return IDENT;
  if (strcmp(s, "+") == 0) return ADD;
  if (strcmp(s, "-") == 0) return SUB;
  if (strcmp(s, "*") == 0) return MUL;
  if (strcmp(s, "/") == 0) return DIV;
  if (strcmp(s, "(") == 0) return L_S_BRACKETS;
  if (strcmp(s, ")") == 0) return R_S_BRACKETS;
  if (strcmp(s, "{") == 0) return L_M_BRACKETS;
  if (strcmp(s, "}") == 0) return R_M_BRACKETS;
  if (strcmp(s, "[") == 0) return L_L_BRACKETS;
  if (strcmp(s, "]") == 0) return R_L_BRACKETS;
  if (strcmp(s, "=") == 0) return EQUAL;
  if (strcmp(s, ">") == 0) return B_COMP;
  if (strcmp(s, "<") == 0) return L_COMP;
  if (strcmp(s, "==") == 0) return E_COMP;
  if (strcmp(s, ":") == 0) return COLON;
  if (strcmp(s, ";") == 0) return SEMI_C;
  */
}
