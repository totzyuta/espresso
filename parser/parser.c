#include <stdio.h>
#include "define.h"

TokenSt *nextToken(FILE *fp);
void ungetToken(void);

static parse_program(FILE *fp);
static parse_define(FILE *fp);
static parse_define_statement(FILE *fp);
static parse_define_array(FILE *fp);
static parse_statements(FILE *fp);
static parse_statement(FILE *fp);
static parse_assign_array(FILE *fp);
static parse_assign_value(FILE *fp);
static parse_terms(FILE *fp);
static parse_factor(FILE *fp);
static parse_value(FILE *fp);
static parse_argument(FILE *fp);
static parse_func(FILE *fp);
static parse_define_func(FILE *fp);
static parse_while(FILE *fp);
static parse_if(FILE *fp);
static parse_compare(FILE *fp);
static parse_relational(FILE *fp);
static parse_array(FILE *fp);

char error_func_name;

// プログラム全体の解析
void parse_program(FILE *fp) {
}

// 変数宣言部の解析
void parse_define(FILE *fp) {
}

// 宣言文の解析
void parse_define_statement(FILE *fp) {
}

// 配列宣言の解析
void parse_define_array(FILE *fp) {
}

// 文集合の解析
void parse_statements(FILE *fp) {
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
