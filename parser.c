#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "define.h"
#include "gen.c"

TokenSt *nextToken(FILE *fp);
void ungetToken(void);
Node *Oparser(FILE *fp);

static void parse_define(FILE *fp);
static void parse_define_statement(FILE *fp);
static void parse_define_array(FILE *fp);
static void parse_define_funcs(FILE *fp);
static void parse_statements(FILE *fp);
static void parse_statement(FILE *fp);
static void parse_assign_array(FILE *fp);
static void parse_assign_value(FILE *fp, char *to_be_assigned_val);
static void parse_value(FILE *fp);
static void parse_argument(FILE *fp);
static void parse_func(FILE *fp);
static void parse_define_func(FILE *fp);
static void parse_while(FILE *fp);
static void parse_if(FILE *fp);
static void parse_compare(FILE *fp);
static void parse_comp_symbol(int comp_symbol);
static void parse_array(FILE *fp);
static void parse_error(char *error_func_name, char *error_message);
static void print_data();
static void print_nop();
static void init_print();
void gen_code_operation(Node *node);

char *error_func_name;
// エラーが起こった理由. 各関数内で定義. 
// nullで渡しても一応大丈夫にはなってるので順次追加する感じで
char *error_message;

TokenSt *token;

struct symbol_table symbol[100];
int add = 0;

// プログラム全体の解析
// <プログラム> :== <変数宣言部><文集合>
void parse_program(FILE *fp) {
  init_print(); 
  // printf("プログラム全体の解析の始まり\n");
  // printf("変数宣言部の解析の始まり\n");
  // 識別子名を格納
  // printf(".data\n"); // 変数領域の確保開始
  parse_define(fp);
  // printf("変数宣言部の解析のおわり\n");
  token = nextToken(fp);
  if(token->type == FUNC) {
    ungetToken();
    parse_define_funcs(fp);
  }
  ungetToken();
  parse_statements(fp);
  // printf("プログラム全体の始まり\n");
  printf("jr $ra\n");
  print_data();
}

// 変数宣言部の解析
// <変数宣言部> ::= <宣言文> <変数宣言部> | <宣言文>
void parse_define(FILE *fp) {
  error_func_name = "parse_define";
  token = nextToken(fp);
  if(token->type == DEFINE) {
    ungetToken();
    parse_define_statement(fp);
    // 再帰して再び変数宣言部の解析
    parse_define(fp);
  }
  ungetToken();
}

// 宣言文の解析
// <宣言文> ::= define <識別子>; | define <配列宣言>;
void parse_define_statement(FILE *fp) {
  error_func_name = "parse_define_statement";
  // printf("宣言文の解析の始まり\n");
  token = nextToken(fp);
  if(token->type == DEFINE) {
    token = nextToken(fp);
    if(token->type == IDENT) {
      struct symbol_table *table =  &symbol[add++]; // 記号表の現在位置を取得
      strcpy(table->id, token->string); // 記号表に変数名を登録
      token = nextToken(fp);
      if(token->type == LSQUARE) {
        // 配列の宣言
        ungetToken();
        parse_define_array(fp); /* 後戻り: <識別子>スキップ */
      }
      // '['じゃなかったら、変数の宣言がここで完了したことになる
    }else{
      // DEFINEのあとが識別子じゃなかったらエラー
      error_message = "assign identifier after 'define'";
      parse_error(error_func_name, error_message);
    }
  }else {
    // DEFINEじゃなかったらエラー
    error_message = "not 'define'";
    parse_error(error_func_name, error_message);
  }

  if(token->type != SEMICOLON) {
    // 宣言文が';'で終わってなかったらエラー
    error_message = "define not ends with ';'";
    parse_error(error_func_name, error_message);
  }
  // printf("宣言文の解析のおわり\n");
}

// 配列宣言の解析
// <配列宣言> :== <識別子>[<整数>] | <識別子>[<整数>][<整数>]
// 後戻り: <識別子>スキップ 
void parse_define_array(FILE *fp) {
  error_func_name = "parse_define_array";
  // printf("配列宣言の解析の始まり\n");
  token = nextToken(fp);
  if(token->type == LSQUARE) {
    token = nextToken(fp);
    if(token->type != INTEGER) {
      // [ ] の中が整数じゃなかったらエラー
      error_message = "define arrays not by integer   ex. array[5]";
      parse_error(error_func_name, error_message);
    }
    token = nextToken(fp);
    if(token->type != RSQUARE) {
      // ] でおわっていなければエラー
      error_message = "not ends with ']' to define array";
      parse_error(error_func_name, error_message);
    }
    // [ がでてこなくなるまで '[ INT ]'の解析を再帰
    // (配列宣言の最後は';'になるはず)
    // parse_define_array(fp);
    //変更箇所
    token = nextToken(fp);
    if(token->type == LSQUARE){
      token = nextToken(fp);
      if(token->type != INTEGER) {
        // [ ] の中が整数じゃなかったらエラー
        error_message = "define arrays not by integer   ex. array[5]";
        parse_error(error_func_name, error_message);
      }
      token = nextToken(fp);
      if(token->type != RSQUARE) {
        // ] でおわっていなければエラー
        error_message = "not ends with ']' to define array";
        parse_error(error_func_name, error_message);
      }
      token = nextToken(fp);
    }else{
      // ungetToken(); //先読みした文戻す
    }
  }
  // printf("配列宣言の解析のおわり\n");
}


// 関数宣言部の解析
// <関数宣言部> ::= <関数宣言文> <関数宣言部> | <関数宣言文>
void parse_define_funcs(FILE *fp) {
  // printf("関数宣言部の解析のはじまり\n");
  parse_define_func(fp);
  token = nextToken(fp);
  // 文の先頭が`func`であれば再帰して解析
  if(token->type == FUNC) {
    parse_define_funcs(fp);
  }
  ungetToken();
  // printf("関数宣言部の解析のおわり\n");
}

// DEBUG: 文集合の解析関数が何回呼ばれたかカウントして対応する終わりを確認するため
int number_statements = 0;

// 文集合の解析
// <文集合> ::= <文> <文集合> | <文>
// ここでは関数の最初にnextToken()を呼んでいないので
// 呼び出し時にtokenがすでに読み込むべき最初のトークンになっている必要がある
void parse_statements(FILE *fp) {
  // DEBUG
  number_statements++;
  error_func_name = "parse_statement";
  error_message = "";
  // printf("文集合の解析の始まり (%d)\n", number_statements);
  parse_statement(fp);

  // 最後がかっこかreturnじゃなかったら再帰
  if(token != NULL) {
    if (strcmp(token->string, "}")!=0 && token->type != RETURN) {
      parse_statements(fp);
    }
  }

  // printf("文集合の解析のおわり (%d)\n", number_statements);
  // DEBUG
  number_statements--;
}

// 文の解析
// <文> ::= <代入文> | <ループ文> | <条件分岐文> | <関数宣言文> | <関数>;| break;
// 後戻り <識別子>飛ばして `=` から
void parse_statement(FILE *fp) {
  error_func_name = "parse_statement";
  // printf("文の解析の始まり\n");
  //  ungetToken();
  token = nextToken(fp);
  if(token->type == IDENT){
    char *identifier = token->string;
    token = nextToken(fp);
    if(token->type == EQUAL){
      ungetToken();
      parse_assign_value(fp, identifier); /*後戻り　識別子*/
    }else if(token->type == LSQUARE) {
      ungetToken();
      parse_assign_array(fp); /*後戻り　識別子*/
      // }else if(token->type == LPAREN){
      // ungetToken();
      // parse_func(fp); /*後戻り　識別子*/
      // token = nextToken(fp);
      // if(token->type != SEMICOLON) {
      //    parse_error(error_func_name, error_message);
      // }
    }else {
      error_message = "`=` or `[` should follow identifier. Or maybe you forgot `call` for using function.";
      parse_error(error_func_name, error_message);
    }
  }else if(token->type == WHILE){
    ungetToken();
    parse_while(fp);
  }else if(token->type == IF){
    ungetToken();
    parse_if(fp);
  }else if(token ->type == CALL){
    // ungetToken();
    parse_func(fp);
    token = nextToken(fp);
    if(token->type != SEMICOLON) {
      error_message = "no semicolon after calling function.";
      parse_error(error_func_name, error_message);
    }
    token = nextToken(fp);
  }else if(token->type == RETURN){ //return が文集合の中に入ってしまっているため　省き　文集合を終了させる
    ungetToken();
  }
  // printf("文の解析の終わり\n");
}

// 配列代入文の解析
void parse_assign_array(FILE *fp){
  // printf("配列代入文の解析のはじまり\n");
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
          // nothing
        }else{
          parse_error(error_func_name, error_message);
        }
      }else{
        parse_error(error_func_name, error_message);
      }
    }else{
      ungetToken();
      gen_code_operation(Oparser(fp));
      token = nextToken(fp);
      if(token->type == SEMICOLON){
        // nothing
      }else{
        parse_error(error_func_name, error_message);
      }
    }
  }else{
    parse_error(error_func_name, error_message);
  }
  // printf("配列代入文の解析の終わり\n");
}

// 代入文の解析
// 後戻り <識別子>はスキップして `=` から
void parse_assign_value(FILE *fp, char *to_be_assigned_val) {
  error_func_name = "parse_assign_value";
  // printf("代入文の解析のはじまり\n");
  // varに変数名を格納
  char *var;
  var = token->string;
  token = nextToken(fp); // unget token: これでtoken-stringは`=`になってるはず　 
  if(token->type == EQUAL){
    token = nextToken(fp);
    if(token ->type == CALL){
      parse_func(fp);
      // generating code
      printf("li $t7, _%s\n", to_be_assigned_val);
      printf("sw $v0, 0($t7)\n"); // 関数の返り値は$v0に入ってるとする
      token = nextToken(fp);
      if(token->type != SEMICOLON){
        error_message = "Not ends with `;` when to assign value";
        parse_error(error_func_name, error_message);
      }
    }else{
      ungetToken();
      gen_code_operation(Oparser(fp));   // call Oparser !!
      // generating code
      printf("li $t7, _%s\n", to_be_assigned_val);
      printf("sw $v0, 0($t7)\n"); // 算術式の結果は$v0に入っているとする
      token = nextToken(fp); // unget token
      if(token->type != SEMICOLON){
        error_message = "Not ends with `;` when to assign value";
        parse_error(error_func_name, error_message);
      }
    }
  }else{
    parse_error(error_func_name, error_message);
  }
  // printf("代入文の解析の終わり\n");
}

// 変数の解析
void parse_value(FILE *fp) {
  // printf("変数の解析のはじまり\n");
  error_func_name = "parse_value";
  error_message = "";
  token = nextToken(fp);
  if(token->type == IDENT){
    token = nextToken(fp);
    if(token->type == LSQUARE){
      ungetToken();   //atomodori   [
      parse_array(fp);
    }
  }else if (token->type == INTEGER){
    token = nextToken(fp);
  }else{
    parse_error(error_func_name, error_message);
  }
  // printf("変数の解析の終わり\n");
}

// 引数の解析
void parse_argument(FILE *fp) {
  // printf("引数の解析のはじまり\n");
  error_func_name = "parse_argument";
  token = nextToken(fp);
  if(token->type == INTEGER || token->type == IDENT ){
    ungetToken();
    parse_value(fp);
  }else{
    parse_error(error_func_name, error_message);
  }
//  token = nextToken(fp);
  if(token->type == COMMA){
    parse_argument(fp);
  }else{
    ungetToken();
  }
  // printf("引数の解析の終わり\n");
}

// 関数の解析
void parse_func(FILE *fp) {
  // printf("関数の解析の始まり\n");
  error_func_name = "parse_func";
  token = nextToken(fp); 
  if(token->type == IDENT) {
    token = nextToken(fp);
    if(token->type == LPAREN){
      token = nextToken(fp);
      if(token->type == RPAREN){ // 引数がない場合
        // nothing
      }else { // 引数がある場合
        ungetToken();
        parse_argument(fp);
        token = nextToken(fp);
        if(token->type != RPAREN){ 
          error_message = "Ends calling function with `)`";
          parse_error(error_func_name, error_message);
        }
      }
    }else{
      error_message = "Wrong way to call function. May check `(` is after identifier.";
      parse_error(error_func_name, error_message);
    }
  }else {
    error_message = "Not calling function with correct identifier.";
    parse_error(error_func_name, error_message);
  }
  // printf("関数の解析の終わり \n");
}

// 関数宣言文の解析
void parse_define_func(FILE *fp){
  // printf("関数宣言文の解析のはじまり\n");
  error_func_name = "parse_define_func";
  token = nextToken(fp);
  if (token->type != FUNC) {
    error_message = "token type not FUNC";
    parse_error(error_func_name,error_message);
  }
  token = nextToken(fp);
  if(token->type != IDENT) {
    error_message = "token type not IDENT .........func <here>";
    parse_error(error_func_name,error_message);
  }
  token = nextToken(fp);
  if(token->type != LPAREN){
    error_message = "token type not LPAREN ";
    parse_error(error_func_name,error_message);
  }
  token = nextToken(fp);
  if (token->type == IDENT || token->type == INTEGER) {
    ungetToken(); // IDENT or INTEGER を先読みしたため
    parse_argument(fp);
  }else if(token->type == RPAREN){
    goto after_RPAREN;
  }else{
    error_message = "func <....>( here is not IDENT or INTEGER or void)";
    parse_error(error_func_name,error_message);
  }
  token = nextToken(fp);
  if(token->type != RPAREN){
    error_message = "token type not RPAREN";
    parse_error(error_func_name,error_message);
  }
  after_RPAREN:
  token = nextToken(fp);
  if(token->type != LCURLY){
    error_message = "token type not LCURLY";
    parse_error(error_func_name,error_message);
  }
  parse_statements(fp);
  token = nextToken(fp);
  if(token->type == RETURN){    // return exist
    // parse_value(fp);
    token = nextToken(fp);
    if(token->type != IDENT && token->type != INTEGER) {
      error_message = "token type not IDENT or INTEGER return <here>";
      parse_error(error_func_name,error_message);
    }
    ungetToken();   // ↑の文で確かめたために必要　確かめない場合は不要
    parse_value(fp);
    // EDITED
    // token = nextToken(fp);
    if(token->type != SEMICOLON){
      error_message = " token type not SEMICOLON return <...> ? here";
      parse_error(error_func_name,error_message);
    }
    token = nextToken(fp);
    if(token->type != RCURLY){
      error_message = "token type not RCURLY .....return <...> ; ?";
      parse_error(error_func_name,error_message);
      goto AFTER_RCURLY;
    }
  }else if(token->type == RCURLY){
    goto AFTER_RCURLY; 
  }else{
   error_message = "not return or not void";
   parse_error(error_func_name,error_message);
  }
  AFTER_RCURLY:
  return ;
}


int LABEL = 1; // ラベルをL1から始める
int LABEL2;

// jumpの後にnopがいるよ。
// L2は常に最後に抜ける場所として確保
// L1はL2に対応、L3はL4に対応という感じ
// L1: 
//     ...
// L3: 
//     ...
// L4: 
//     ...
// L2: 
//     ...
// ラベルの値をローカルに保存するための変数を用意する
// ジャンプ先のラベルはグローバルで上書きし続けるとか
// whileがきた時点でローカルにラベルを2つローカルでとる
// グローバル変数でラベルどこまで使ってるか保存しておく
// ループ文の解析
void parse_while(FILE *fp) {
  int miss = 1;
  error_func_name = "parse_while";
  // printf("ループ文の解析の始まり\n");
  // while文のラベルを生成
  int label1 = LABEL++;
  int label2 = LABEL++;
  LABEL2 = label2;
  printf("$L%d:\n", label1);
  token = nextToken(fp);
  if(token->type == WHILE){
    token = nextToken(fp);
    if(token->type == LPAREN){
      parse_compare(fp);
      token = nextToken(fp);
      if(token->type == RPAREN){
        token = nextToken(fp);
        if(token->type == LCURLY){
          parse_statements(fp); // whileの中で行う処理の文集合の解析
          if(token->type == RCURLY){
            miss = 0;
          }
        }
      }
    }
  }
  printf("j $L%d\n", label1);
  printf("nop\n");
  printf("$L%d:\n", label2);
  if(miss == 1){
    error_message = "wrong syntax in while loop";
    parse_error(error_func_name, error_message);
  }
  token = nextToken(fp);
  ungetToken();
  // printf("ループ文の解析の終わり\n");
}

void parse_if(FILE *fp){
  // printf("条件分岐文の解析の始まり\n");
  error_func_name = "parse_if";
  token = nextToken(fp);
  if(token->type != IF){
    error_message = "token type not IF";
    parse_error(error_func_name, error_message);
  }
  token = nextToken(fp);
  if(token->type != LPAREN){
    error_message = "token type not LPAREN";
    parse_error(error_func_name, error_message);
  }
  parse_compare(fp); //........if ( here
  token = nextToken(fp);
  if(token->type != RPAREN){
    error_message = "token type not RPAREN";
    parse_error(error_func_name, error_message);
  }
  token = nextToken(fp);
  if(token ->type != LCURLY){
    error_message = "token type not LCURLY";
    parse_error(error_func_name,error_message);
  }
  parse_statements(fp);
  if(token == NULL) {
    goto parse_if_end;
  }
  if (token->type== RCURLY) {
    // nothing to do here?
  }
  if (token->type != RCURLY) {
    error_message = "token type not RCURLY";
    parse_error(error_func_name,error_message);
  }
  token = nextToken(fp);
  if(token == NULL){
    goto parse_if_end;
  }
  if (token->type == ELSE) {
    token = nextToken(fp);
    if (token->type != LCURLY) {
      error_message = "token type not LCURLY if(...){...}else{  here!";
      parse_error(error_func_name,error_message);
    }
    parse_statements(fp);
    if(token->type != RCURLY){
      error_message = "token type not RCURLY";
      parse_error(error_func_name,error_message);
    }
  }else{
    ungetToken(); // else を余分に読み込んだため
  }
  parse_if_end:
  return;
  // printf("条件分岐文の解析の終わり\n");
}

// 条件式の解析
// 比較演算子の解析のところを参照 -> それぞれ生成するコードを分ける必要あり
void parse_compare(FILE *fp) {
  // printf("条件式の解析の始まり\n");
  error_func_name = "parse_compare";
  gen_code_operation(Oparser(fp));
  printf("add $t8, $v0, $zero\n"); // $t8つかわないと持ってかれる。$t8:条件式左辺
  token = nextToken(fp);
  int comp_symbol = token->type; // 比較演算子の両辺を解析してから分岐処理を生成する
  gen_code_operation(Oparser(fp)); // $v0に条件式の右辺入ってるよ
  parse_comp_symbol(comp_symbol); // 比較演算子の解析
  // printf("条件式の解析の終わり\n");
}

// 比較演算子の解析
void parse_comp_symbol(int comp_symbol) {
  if(comp_symbol == GREATER) { // >
    printf("slt $t8, $v0, $t8\n");
    printf("beq $t8, $zero, $L%d\n",LABEL2); // LABEL2は現在の深さのwhileの飛ぶ先のラベル
  }else if(comp_symbol == EQGREATER) { // >=
    printf("slt $t8, $t8, $v0\n");
    printf("bne $t8, $zero, $L%d\n",LABEL2);
  }else if(comp_symbol == LESS) { // <
    printf("slt $t8, $t8, $v0\n");
    printf("beq $t8, $zero, $L%d\n",LABEL2);
  }else if(comp_symbol == EQLESS) { // <=
    printf("slt $t8, $v0, $t8\n");
    printf("bne $t8, $zero, $L%d\n",LABEL2);
  }else if(comp_symbol == NEQUAL) {
    printf("bne $t8, $v0, $L%d\n",LABEL2);
  }else if(comp_symbol == EQUAL2) {
    printf("beq $t8, $v0, $L%d\n",LABEL2);
  }else {
    error_message = "wrong relational sign.";
    parse_error(error_func_name, error_message);
  }
  print_nop();
}

// 配列の解析
void parse_array(FILE *fp) {
  /*LSQUAREが次のtokenになる*/
  // printf("後戻りで呼ばれた\n");
  // printf("配列の解析の始まり\n");
  error_func_name = "parse_array";
  token = nextToken(fp);
  if(token ->type == LSQUARE){
    token = nextToken(fp);
    if(token->type == IDENT|| token->type == INTEGER ){
      token = nextToken(fp);
      if(token ->type != RSQUARE){ /*一次元配列の終了へ*/
        error_message = "one-dimensional array not has RSQUARE";
        parse_error(error_func_name, error_message);
      }
      token = nextToken(fp); /*次にRSQUARが存在するか判断*/
      if(token->type == LSQUARE){
        token = nextToken(fp);
        if(token->type == IDENT || token->type == INTEGER){
          token = nextToken(fp);
          if(token->type == RSQUARE){
            /*二次元配列の終了*/
          }else{
            error_message = "two-dimensional array not has RSQUARE";
            parse_error(error_func_name, error_message);
          }
        }else{
          error_message = "two-dimensional array not has IDENT or INTEGER";
          parse_error(error_func_name, error_message);
        }
      }else{
        ungetToken();
        /*一次元配列の終了*/
      }
    }else{
      error_message = "one-dimensional array not has IDENT or INTEGER";
      parse_error(error_func_name, error_message);
    }
  }else{
    error_message = "one-dimensional array not has LSQUARE";
    parse_error(error_func_name, error_message);
  }
  // printf("配列の解析の終わり\n");
}


// エラー処理
void parse_error(char *error_func_name, char *error_message) {
  if (error_message == NULL) {
    error_message = "sorry, an unknow error occurs...";
  }
  // printf("error in %s: %s\n", error_func_name, error_message);
  exit(1);
}

void print_data() {
  struct symbol_table *table = symbol;
  printf("\t.data 0x10004000\n");
  for (int i=0; i<add; i++) {
    printf("_%s:\t.word 0x0000\n", table->id);
    table++;
  }
}

void print_nop(){
  printf("nop\t\t\t\t# (delay slot)\n");
}

void init_print() {
  printf("\tINITIAL_GP = 0x10008000\n");
  printf("\tINITIAL_SP = 0x7ffffffc\n");
  printf("\t# system call service number\n\n");
  printf("\tstop_service = 99\n");
  printf("\t.text\n\t# 初期化ルーチン\n");
  printf("init:\n");
  printf("\t# initialize $gp (global pointer) and $sp (stack pointer)\n");
  printf("\tla\t$gp, INITIAL_GP\t\t# $gp <- 0x10008000 (INITIAL_GP)\n");
  printf("\tla\t$sp, INITIAL_SP\t\t# $sp <- 0x7ffffffc (INITIAL_SP)\n");
  printf("\tjal\tmain\t\t\t# jump to `main'\n");
  print_nop();
  printf("\tli\t$v0, stop_service\t# $v0 <- 99 (stop_service)\n");
  printf("\tsyscall\t\t\t\t# stop\n");
  print_nop();
  printf("\t# not reach here\n");
  printf("stop:\t\t\t\t\t# if syscall return\n");
  printf("\tj stop\t\t\t\t# infinite loop...\n");
  print_nop();
  printf("\n\t.text\t0x00001000\n");
  printf("main:\n");
}
