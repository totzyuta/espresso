#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "define.h"

TokenSt *nextToken(FILE *fp);
void ungetToken(void);
Node *Oparser(FILE *fp);

static void parse_define(FILE *fp);
static void parse_define_statement(FILE *fp);
static void parse_define_array(FILE *fp);
static void parse_statements(FILE *fp);
static void parse_statement(FILE *fp);
static void parse_assign_array(FILE *fp);
static void parse_assign_value(FILE *fp);
static void parse_value(FILE *fp);
static void parse_argument(FILE *fp);
static void parse_func(FILE *fp);
static void parse_define_func(FILE *fp);
static void parse_while(FILE *fp);
static void parse_if(FILE *fp);
static void parse_compare(FILE *fp);
static void parse_relational(FILE *fp);
static void parse_array(FILE *fp);
static void parse_error(char *error_func_name);

char *error_func_name;

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
    ungetToken();
    parse_define_statement(fp);
    // 再帰して再び変数宣言部の解析
    parse_define(fp);
  }else {
    // このエラー処理いる？
    parse_error(error_func_name);
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
      parse_error(error_func_name);
    }
  }else {
    // DEFINEじゃなかったらエラー
    parse_error(error_func_name);
  }
  token = nextToken(fp);
  // 宣言文が';'で終わってなかったらエラー
  if(token->type != SEMICOLON) {
    parse_error(error_func_name);
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
      parse_error(error_func_name);
    }
    token = nextToken(fp);
    if(token->type != RSQUARE) {
      // ] でおわっていなければエラー
      parse_error(error_func_name);
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
// <文> ::= <代入文> | <ループ文> | <条件分岐文> | <関数宣言文> | <関数>;| break;
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
      token = nextToken(fp);
      if(token->type != SEMICOLON) {
          parse_error(error_func_name);
      }
    }else {
      parse_error(error_func_name);
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
    parse_error(error_func_name);
  }
  printf("文の解析の終わり\n");
}

// 配列代入文の解析
void parse_assign_array(FILE *fp){
  printf("配列代入文の解析のはじまり\n");
  error_func_name = "parse_assign_array";
  parse_array(fp);
  token = nextToken(fp);
  if(token->type == EQUAL){
    token = nextToken(fp);
    if(token->type == CALL){
      token = nextToken(fp);
      if(token->type == IDENT){
	ungetToken();
	parse_func(fp);
	token = nextToken(fp);
	if(token->type == SEMICOLON){
	}else{
	  parse_error(error_func_name);
	}
      }else{
	parse_error(error_func_name);
      }
    }else{
      ungetToken();
      Oparser(fp);
      token = nextToken(fp);
      if(token->type == SEMICOLON){
      }else{
	parse_error(error_func_name);
      }
    }
  }else{
    parse_error(error_func_name);
  }
  printf("配列代入文の解析の終わり\n");

}

// 代入文の解析
void parse_assign_value(FILE *fp) {
  printf("parse_assign_value 後戻りで呼び出された\n");
  error_func_name = "parse_assign_value";
  printf("代入文の解析のはじまり\n");
  token = nextToken(fp);
  if(token->type == EQUAL){
    token = nextToken(fp);
    if(token ->type == CALL){
      ungetToken();
      parse_func(fp);
      token = nextToken(fp);
      if(token->type != SEMICOLON){
        parse_error(error_func_name);
      }
    }
    else{
      Oparser(fp);   // call Oparser !!
      token = nextToken(fp);
      if(token->type != SEMICOLON){
        parse_error(error_func_name);
      }
    }
  }else{
    parse_error(error_func_name);
  }
  printf("代入文の解析の終わり\n");
}

// 変数の解析
void parse_value(FILE *fp) {
  printf("変数の解析のはじまり\n");
  error_func_name = "parse_value";
  token = nextToken(fp);
  if(token->type == IDENT){
    token = nextToken(fp);
    if(token->type == LSQUARE){
      ungetToken();   //atomodori   [
      parse_array(fp);
    }
  }else if (token->type == INTEGER){
  }else{
    parse_error(error_func_name);
  }
  printf("変数の解析の終わり\n");
}

// 引数の解析
void parse_argument(FILE *fp) {
  printf("引数の解析のはじまり\n");
  error_func_name = "parse_argument";
  token = nextToken(fp);
  if(token->type == INTEGER || token->type == IDENT ){
    ungetToken();
    parse_value(fp);
  }else{
    parse_error(error_func_name);
  }
  token = nextToken(fp);
  if(token->type == COMMA){
    parse_argument(fp);
  }else{
    ungetToken();
  }
  printf("引数の解析の終わり\n");
}

// 関数の解析
void parse_func(FILE *fp) {
  printf("parse_func 後戻りで呼ばれた\n");
  printf("関数の解析の始まり\n");
  error_func_name = "parse_func";
  token = nextToken(fp);
  if(token->type == LPAREN){
    token = nextToken(fp);
    if(token->type == RPAREN){
    }else{
      ungetToken();
      parse_argument(fp);
    }
  }else{
    parse_error(error_func_name);
  }
  printf("関数の解析の終わり \n");
}

// 関数宣言文の解析
void parse_define_func(FILE *fp) {
  int miss = 1;
  error_func_name = "parse_define";
  printf("ループ文の解析の始まり");
  token = nextToken(fp);
  if(token->type == FUNC){
    token = nextToken(fp);
    if(token->type == IDENT){
      token = nextToken(fp);
      if(token->type == LPAREN){
	token = nextToken(fp);
	if(token->type != RPAREN){  /*空白の場合もあとで記述 */
	  ungetToken();
	  parse_argument(fp);
	  if(token->type == RPAREN){
	    token = nextToken(fp);
	    if(token->type == LCURLY){
	      parse_statements(fp);
	      token = nextToken(fp);
	      if(token->type == RCURLY){
		miss = 0;
		token = nextToken(fp);
		if(token->type == ELSE){
		  miss = 1;
		  token = nextToken(fp);
		  if(token->type == LCURLY){
		    parse_statements(fp);
		    token = nextToken(fp);
		    if(token->type == RCURLY){
		      miss = 0;
		    }
		  }
		}
	      }
	    }
	  }
	}else{
	  token = nextToken(fp);
	  if(token->type == LCURLY){
	    parse_statements(fp);
	    token = nextToken(fp);
	    if(token->type == RCURLY){
	      miss = 0;
	      token = nextToken(fp);
	      if(token->type == ELSE){
		miss = 1;
		token = nextToken(fp);
		if(token->type == LCURLY){
		  parse_statements(fp);
		  token = nextToken(fp);
		  if(token->type == RCURLY){
		    miss = 0;
		  }
		}
	      }
	    }
	  }
	}
      }
    }
  }
  if(miss == 1){
    parse_error(error_func_name);
  }
  printf("関数宣言文の解析の終わり");
}

// ループ文の解析
void parse_while(FILE *fp) {
  int miss = 1;
  error_func_name = "parse_while";
  printf("ループ文の解析の始まり\n");
  token = nextToken(fp);
  if(token->type == WHILE){
    token = nextToken(fp);
    if(token->type == LPAREN){
      parse_compare(fp);
      token = nextToken(fp);
      if(token->type == RPAREN){
	token = nextToken(fp);
	if(token->type == LCURLY){
	  parse_statements(fp);
	  token = nextToken(fp);
	  if(token->type == RCURLY){
	    miss = 0;
	  }
	}
      }
    }
  }
  if(miss == 1){
    parse_error(error_func_name);
  }
  printf("ループ文の解析の終わり\n");
}

// 条件分岐分の解析
void parse_if(FILE *fp) {
  int miss=1;
  error_func_name = "parse_if";
  printf("条件分岐式の解析の始まり\n");
  token = nextToken(fp);
  if(token->type == IF){
    token = nextToken(fp);
    if(token->type == LPAREN){
      parse_compare(fp);
      token = nextToken(fp);
      if(token->type == RPAREN){
        token = nextToken(fp);
        if(token->type == LCURLY){
          parse_statements(fp);
          token = nextToken(fp);
          if(token->type == RCURLY){
            miss = 0;
            token =nextToken(fp);
            if(token->type == ELSE){
              miss = 1;
              token = nextToken(fp);
              if(token->type == LPAREN){
                parse_statements(fp);
                token = nextToken(fp);
                if(token->type == RPAREN){
                  miss = 0;
                  token = nextToken(fp); /*else~~~が付く場合の token の位置の調整 */
                }
              }
            }
          ungetToken();
          } 
        }
      }
    }
  }
  if(miss==1){
    parse_error(error_func_name);
  }
  printf("条件分岐式の解析の終わり\n");
}

// 条件式の解析
void parse_compare(FILE *fp) {
  printf("条件式の解析の始まり\n");
  error_func_name = "parse_compare";
  Oparser(fp);
  token = nextToken(fp);
  if(token->type == GREATER || token->type == EQGREATER ||
     token->type == LESS || token->type == EQLESS || 
     token->type == NEQUAL || token->type == EQUAL2){
    ungetToken();
    parse_relational(fp);
  }else {
    parse_error(error_func_name);
  }
  Oparser(fp);
  printf("条件式の解析の終わり\n");
}

// 比較演算子の解析
void parse_relational(FILE *fp) {
  error_func_name = "parse_relational";
  printf("比較演算子の解析の始まり\n");
  token = nextToken(fp);
  if(token->type != EQUAL2){
    if(token->type != LESS){
      if(token->type != GREATER){
        if(token->type != EQGREATER){
          if(token->type != EQLESS){
            if(token->type != NEQUAL){
              parse_error(error_func_name);
            }
          }
        }
      }
    }
  }
  printf("比較演算子の解析の終わり\n");
}

// 配列の解析
void parse_array(FILE *fp) {
  /*LSQUARが次のtokenになる*/
  int miss =1;
  error_func_name = "parse_array";
  printf("配列の解析の始まり");
  token = nextToken(fp);
  if(token->type == IDENT){
    token = nextToken(fp);
    while(token->type == LSQUARE){
      miss = 1;
      parse_value(fp);
      token = nextToken(fp);
      if(token->type ==RSQUARE){
	miss = 0;
	token = nextToken(fp);
      }
    }
  }
  ungetToken(); /*Tokenの位置の調整*/
  if(miss == 1){
    parse_error(error_func_name);
  }
  printf("配列の解析の終わり");
}

// エラー処理
void parse_error(char *error_func_name) {
  printf("error : %s\n", error_func_name);
}
