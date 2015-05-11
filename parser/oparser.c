
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
static OpeType typeToOpeType(TokenType Type)
{
	if((Type == SUM)||(Type == SUB)) return ot_PlusMinus;
  else if((Type == MUL)||(Type == DIV)) return ot_MultiDiv;
  else if(Type == LPAREN) return ot_LPar;
  else if(Type == RPAREN) return ot_RPar;
  else if(Type == DOLLAR) return ot_Dollar;
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


/*
 * push,pop関数を実装
 */

// 0:演算子用, 1:数値・識別子用
static Node *Stack[2][MaxStack];
static int Sptr[2];

void stackError(n) {
  if(n==0)
    fprintf(stderr, "Stack Overflow\n");
  if(n==1)
    fprintf(stderr, "Stack Underflow\n");
  exit(1);
}

// Node型の変数をStack Sに積む
void push(int S, Node *n) {
  if (Sptr[S]>=MaxStack) stackError(0); // Error Handling
  else {
    Stack[S][Sptr[S]] = n;
    Sptr[S] = Sptr[S] + 1; 
    // debug
    printf("Pushed '%s' to stack[%d]", n->token->string, S);
  }
}

// Stack Sをpopする
Node *pop(int S) {
  if (Sptr[S]>=MaxStack) stackError(1); // Error Handling
  else {
    Sptr[S] = Sptr[S] - 1;
    // debug
    printf("Popped from stack[%d]", S);
    return Stack[S][Sptr[S]];
}

/*
 * Top,Check関数を実装
 */

// 演算子スタックの一番上のデータの優先順位を返す
// @return [pointer] Nodeのポインタ
// TODO: ASK ここのアスタリスクについて
// Node *Top() {
Node* Top() {
  if (Sptr[S]>=MaxStack) stackError(1); // Error Handling
  else return Stack[1][Sptr[1]-1]; 
}

// TODO: 意味がよくわからん
// 返り値は算術式の解析が終了したかどうかを返す
// @return [int] 0:未終了,1:終了 
int Check(Node *Operator){
  Node *N;
  Node *topNode;
  OpeType order;

  topNode = Top();
  order = orderMatrix[ typeToOpeType(topNode->token->type) ]
                     [ typeToOpeType(Operator->token->type) ];

  // $$の組み合わせ=endなら終了 
  if (order == 5) return 1; /* 算術式解析の終了 */

  // errorの組み合わせなら終了
  if (order == 9){
    // その字は読まなかったことに
    return 1; /* 算術式解析の終了 */
  }

  if (Sptr[1]==0 || order == -1){
    push(1,Operator);
    return 0; /* 未終了 */
  } else  if (typeToOpeType(topNode->token->type)= ot_LPar ) {
    pop(1)
    return 0; /* 未終了 */
  } else {
    // operatorの優先順位が>のとき
    N=pop(1);
    N->left=pop(0);
    N->right=pop(0);
    push(0,N);
    return check(Operator); // 再帰的処理で再び行列を参照して調べる
  }
}

// 算術式の解析を行う関数
Node *Oparser(FILE *fp){
  TokenSt *token;
  Node *node;
  int final;  // 終了判定用
  final = 0; // 未終了状態
  // スタックポインタの初期化
  Sptr[0] = Sptr[1] = 0;
  // 最初に$をでっちあげてstack[1]に突っ込む
  dollarPoint = '$';
  push(1,*dollarPoint);  

  // 終了状態になるまで繰り返す
  while (final == 0){

    // 入力 := 字句入力; {１字句入力し，NodePointer型の値にして返す．}
    token = nextToken(fp);
    node->token = token;

    // 算術式に含まれていないセミコロンなどの文字であったとき
    if (typeToOpeType(token->type)==error) {
      // < 今の字句は読まなかったことにする > 
      ungetToken();
      // < その代わりに＄を読んだことにする >
      push(1,*dollarPoint);  
    }

    // メインのpushのとこ
    // その字句が数値や識別子であったとき
    if (token->type==INTEGER || token->type==IDENT) {
      push(0, token->string);
    } else {
      final = Check(node);
    }

  }
  return Stack[0][0];
}

