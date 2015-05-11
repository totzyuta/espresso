
/*
 * 演算子順位行列の作成
 */

// enum型で演算子の順位関係にナンバーを振っておく
typedef enum {
  ot_PlusMinus, // +, - 
  ot_MultDiv,   // *, / 
  ot_LPar,      // (  
  ot_RPar,      // )  
  ot_Dollar     // $  
} OpeType;


/* 演算子順位行列 */
// >:1  <:-1  =:0  error:9  end:5
// OpeTypeが縦軸、横軸にそれぞれ対応する
// TODO: Make table by comments
// TODO: ASK why the first number is empty
static int orderMatrix[][5] {
  {1,-1,-1,1,-1,1,1},
  {1,1,-1,1,-1,1,1},
  {-1,-1,-1,0,-1,9,9},
  {1,1,9,1,1,1,1},
  {-1,-1,-1,9,-1,0,9},
  {1,1,9,1,9,1,1},
  {-1,-1,-1,9,-1,9,5}
};

// TokenTypeとOpeTypeがちょっと違うのでそこの変換をする関数
// この関数もoparser.cの中でしか使わないのでstatic
// TODO: Fix for my define.h
static OneType typeToOneType(TokenType Type)
{
	if((Type == SUM)||(Type == SUB)) return ot_PlusMinus;
  	else if((Type == MUL)||(Type == DIV)) return ot_MultiDiv;
  	else if(Type == LPAREN) return ot_LPar;
  	else if(Type == RPAREN) return ot_RPar;
  	else if((Type == ENN) return ot_Enn;
  	else return error;
};


/*
 * 構文木のデータ構造を定義
 */

// TODO: define.sで重複して定義されているのでどちらかにまとめる

/*--< トークンを格納する構造体 >--*/
typedef struct {
  char 		string[TOKENMAX];	/* トークンの文字列 */
  TokenType	type;			/* トークンのタイプをすべて保持してる
                         define.h:31 で定義 */
} TokenSt;

typedef struct node {
  TokenSt *token;              
  struct node *left;
  struct node *right;
} Node;
