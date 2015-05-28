static Node *Stack[2][MaxStack];
static int Sptr[2];

/*	演算子順位行列	*/
static int orderMatrix[][7]P{
{1,-1,-1,1,-1,1,1},// >:1	<:-1	=:0	エラー:9	終了:5
{1,1,-1,1,-1,1,1},
{-1,-1,-1,0,-1,9,9},
{1,1,9,1,1,1,1},
{-1,-1,-1,9,-1,0,9},
{1,1,9,1,9,1,1},
{-1,-1,-1,9,-1,9,5}
};


static OneType typeToOneType(TokenType Type)
{
	if((Type == SUM)||(Type == SUB)) return ot_PlusMinus;
  	else if((Type == MUL)||(Type == DIV)) return ot_MultiDiv;
  	else if(Type == LPAREN) return ot_LPar;
  	else if(Type == RPAREN) return ot_RPar;
  	else if((Type == ENN) return ot_Enn;
  	else return error;
}

void push(int S, Node *n){ /* Node 型の変数をスタックS に積む*/
  if (Sptr[S]>=MaxStack)// エラー処理
	else{
		Stack[S][Sptr[S]] = n;
		Sptr[S] = Sptr[S] + 1;
	}
}


Node* pop(int S){ /* スタックS をpop する*/
  if (Sptr[S]<=0)// エラー処理
	else{
		Sptr[S] = Sptr[S] - 1;
		return Stack[S][Sptr[S]];
	}
}


NODE *Top(){ /* 演算子スタックの最上部のデータの優先順位を返す*/
  if (Sprt[1]<=0) //エラー処理
	else return Stack[1][Sptr[1]-1];
}



int Check(Node *Operator){
  Node *N;
  Node *topNode;
  OneType     order;

  topNode = Top();
  order = orderMatrix[ typeToOneType(topNode->token->type) ]
                     [ typeToOneType(Operator->token->type) ];

  if (order == 5) return 1; /* 算術式解析の終了 */

  if (order == 9){



    
    return 1; /* 算術式解析の終了 */

  }

  if ( Sptr[1]==0 || order == (-1)){
    push(1,Operator);
    return 0; /* まだ終了していない */

  } else  if (typeToOneType(topNode->token->type)= ot_LPar ) {
    pop(1)
    return 0; /* まだ終了していない */

  } else {
    N=pop(1);
    N->left=pop(0);
    N->right=pop(0);
    push(0,N);
    return check(Operator); //< 再帰的処理．その結果を受け取り，戻り値とする>
  }
}

/*


Node* Oparser(){
	Sptr[0] = 0; Sptr[1] = 0; // スタックポインタを初期化する
	//入力= 字句入力  字句(token) をNode* 型で
	TokenSt       *token;
	Node          *node;

	token = nextToken(fp);
	node = (Node *)malloc(sizeof(Node));
	node->token = token;
	node->left = NULL;
	node->right= NULL;

	while(node->token != NULL){
		if (入力のタイプが識別子または数値){
			push(0, 入力); //数値と識別子はオペランド用のスタックにプッシュ
		}else if{
			Check(入力); // 演算子の場合の処理
		}
		入力= 次の字句;
	}
	return Stack[0][0];
}

*/


Node *Oparser(FILE *fp){
  TokenSt       *token;
  Node          *node;
  int           final;  /* 終了判定用 */

  /*--< 終了状態ではない（＝０） >--*/
  final = 0;

  /*--< スタックポインタを初期化する >--*/
  Sptr[0] = Sptr[1] = 0;

  < ＄をでっちあげる >
      push(1,$);    //< それを Stack[1]に積む >

  /* 終了状態になるまで繰り返す */
  while (final == 0){

    入力 := 字句入力; {１字句入力し，NodePointer型の値にして返す．}

    if ( 算術式に含まれない文字（セミコロンなど）であった ) {

      < 今の字句は読まなかったことにする > 
      < その代わりに＄を読んだことにする >

    }

    if ( その字句が数値や識別子であった ) {
      push(0, 入力);
    } else {
      final = Check(入力);
    }
  }
  return Stack[0][0];
}

