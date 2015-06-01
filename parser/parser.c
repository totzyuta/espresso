#include <stdio.h>
#include "define.h"

TokenSt *nextToken(FILE *fp);
void ungetToken(void);

static void parse_program(FILE *fp);
static void parse_define(FILE *fp);
static void parse_define_statement(FILE *fp);
static void parse_define_array(FILE *fp);
static void parse_statements(FILE *fp);
static void parse_statement(FILE *fp);
static void parse_assign_array(FILE *fp);
static void parse_assign_value(FILE *fp);
static void parse_terms(FILE *fp);
static void parse_factor(FILE *fp);
static void parse_value(FILE *fp);
static void parse_argument(FILE *fp);
static void parse_func(FILE *fp);
static void parse_define_func(FILE *fp);
static void parse_while(FILE *fp);
static void parse_if(FILE *fp);
static void parse_compare(FILE *fp);
static void parse_relational(FILE *fp);
static void parse_array(FILE *fp);
static void ERROR(char error_func_name);

char error_func_name;

TokenSt *token;

// プログラム全体の解析
// <プログラム> :== <変数宣言部><文集合>
void parse_program(FILE *fp) {
  printf("プログラム全体の解析の始まり\n");
  parse_define(fp);
  parse_statements(fp);
  printf("プログラム全体の始まり\n");
}

// 変数宣言部の解析
// <変数宣言部> ::= <宣言文> <変数宣言部> | <宣言文>
void parse_define(FILE *fp) {
  error_func_name = "parse_define";
  printf("変数宣言部の解析の始まり\n");
  token = nextToken(fp);
  if(token->type == DEFINE) {
    ungetToken(fp);
    parse_define_statement(fp);
    // 再帰して再び変数宣言部の解析
    parse_define(fp);
  }else {
    // このエラー処理いる？
    ERROR(error_func_name);
  }
  printf("変数宣言部の解析のおわり\n");
}

// 宣言文の解析
// <宣言文> ::= <define <識別子>; | define <配列宣言>:
void parse_define_statement(FILE *fp) {
  error_func_name = "parse_define_statement";
  printf("宣言部の解析の始まり\n");
  token = nextToken(fp);
  if(token->type == DEFINE) {
    token = nextToken(fp);
    if(token->type == IDENT) {
      token = nextToken(fp);
      if(token->type == LSQUARE) {
        // 配列の宣言
        ungetToken();
        parse_define_array(fp); /* 後戻り: <識別子>スキップ */
      }
      // '['じゃなかったら、変数の宣言がここで完了したことになる
    }else{
      // DEFINEのあとが識別子じゃなかったらエラー
      ERROR(error_func_name);
    }
  }else {
    // DEFINEじゃなかったらエラー
    ERROR(error_func_name);
  }
  token = nextToken(fp);
  // 宣言文が';'で終わってなかったらエラー
  if(token->type != SEMICOLON) {
    ERROR(error_func_name);
  }
  printf("宣言部の解析の始まり\n");
}

// 配列宣言の解析
// <配列宣言> :== <識別子>[<整数>] | <配列宣言>[<整数>]
// 後戻り: <識別子>スキップ 
void parse_define_array(FILE *fp) {
  error_func_name = "parse_define_array";
  printf("配列宣言の解析の始まり\n");
  token = nextToken(fp);
  if(token->type == LSQUARE) {
    token = nextToken(fp);
    if(token->type != INTEGER) {
      // [ ] の中が整数じゃなかったらエラー
      ERROR(error_func_name);
    }
    token = nextToken(fp);
    if(token->type != RSQUARE) {
      // ] でおわっていなければエラー
      ERROR(error_func_name);
    }
    // [ がでてこなくなるまで '[ INT ]'の解析を再帰
    // (配列宣言の最後は';'になるはず)
    parse_define_array(fp);
  }
  printf("配列宣言の解析の始まり\n");
}

// 文集合の解析
// <文集合> ::= <文> <文集合> | <文>
void parse_statements(FILE *fp) {
  error_func_name = "parse_statement";
  printf("文集合の解析の始まり\n");
  parse_statement(fp);
  parse_statements(fp);
  printf("文集合の解析の始まり\n");
}

// 文の解析
// <文> ::= <代入文> | <配列代入文> | <ループ文> | <条件分岐文> | <関数宣言文> | <関数>;
void parse_statement(FILE *fp) {
  error_func_name = "parse_statement";
  printf("文の解析の始まり\n");
  token = nextToken(fp);
  if(token->type == IDENT){ 
    token = nextToken(fp);
    if(token->type == EQUAL){
      ungetToken();
      parse_assign_value(fp); /*後戻り　識別子*/
    }else if(token->type == LSQUARE) {
      ungetToken();
      parse_assign_array(fp); /*後戻り　識別子*/
    }else if(token->type == LPAREN){
      ungetToken();
      parse_func(fp); /*後戻り　識別子*/
      token = nextToken();
      if(token->type != SEMICOLON) {
          ERROR(error_func_name);
      }
    }else {
      ERROR(error_func_name);
    }
  }else if(token->type == WHILE){
    ungetToken();
    parse_while(fp);
  }else if(token->type == IF){
    ungetToken();
    parse_if(fp);
  }else if(token->type == FUNC) {
    ungetToken();
    parse_define_func(fp);
  }else {
    ERROR(error_func_name);
  }
  printf("文の解析の終わり\n");
}

// 配列代入文の解析
void parse_assign_array(FILE *fp) {
}

// 代入文の解析
void parse_assign_value(FILE *fp) {
}

// 算術式の解析
void parse_operation(FILE *fp) {
}

// 項の解析
void parse_terms(FILE *fp) {
}

// 因子の解析
void parse_factor(FILE *fp) {
}

// 変数の解析
void parse_value(FILE *fp) {
}

// 引数の解析
void parse_argument(FILE *fp) {
}

// 関数の解析
void parse_func(FILE *fp) {
}

// 関数宣言文の解析
void parse_define_func(FILE *fp) {
}

// ループ文の解析
void parse_while(FILE *fp) {
}

// 条件分岐分の解析
void parse_if(FILE *fp) {
}

// 条件式の解析
void parse_compare(FILE *fp) {
}

// 比較演算子の解析
void parse_relational(FILE *fp) {
}

// 配列の解析
void parse_array(FILE *fp) {
}

// エラー処理
void ERROR(char error_func_name) {
}
