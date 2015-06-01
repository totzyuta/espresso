/*文字タイプ*/
#define TOKENMAX 100	/* トークンの最大文字数 */
#define MaxStack 128

/*extern int flag;*/
/*--< 文字のタイプ >--*/
typedef enum {
  delim,	  /* 0: 区切り記号(空白,TAB,改行)	*/
  number,	  /* 1: 数字 0～9				          */
  alpha,    /* 2: 英字 a～z, A～Z              */
  brackets, /* 3: < >                          */
  eq,       /* 4: =                            */
  sign,     /* 5: + - * / { } [ ] ( ) ;     */
  excl,     /* 6: !                            */
  error		  /* 7: 未定義文字				            */
} CharType;

/*--< 状態のタイプ >--*/
typedef enum {
  Init,           /*0:初期状態*/
  Int,            /*1:数状態*/
  Identifer,      /*2:識別子状態*/
  Relational,     /*3:比較演算子*/
  Relational2,    /*4:比較演算子(NotEqual)*/
  Not,            /*5:Not状態*/
  NotEq,          /*6:不等号状態*/
  Sign,           /*7: 記号状態*/
  Final           /*8: 終了状態*/
} StateType;

/*--<トークンのタイプ >--*/
typedef enum {
  INTEGER,	    /* 整数		        00  */
  DEFINE,       /*        define  01  */
  WHILE,	      /* 予約語 while   02  */
  IF,           /* 予約語 if      03  */
  ELSE,         /* 予約語 else    04  */
  FUNC,         /* 関数宣言 func  05  */
  CALL,         /* 関数宣言 call  06  */
  RETURN,       /* 返り血 return  07  */
  IDENT,	      /* 識別子	       	08  */
  ADD,		      /* 加算演算子 +	 	09  */
  SUB,          /* 減算演算子 -   10  */
  MUL,  	      /* 乗算演算子 *   11  */
  DIV,          /* 除算演算子 /   12  */
  EQUAL,        /* 等号   =       13  */
  GREATER,      /* 大なり >       14  */
  EQGREATER,    /*        >=      15  */
  LESS,         /* 小なり <       16  */
  EQLESS,       /*        <=      17  */
  NEQUAL,       /*        !=      18  */
  EQUAL2,       /* 比較  ==       19  */
  COMMA,        /*コンマ  ,       20  */
  SEMICOLON,    /*セミコロン ;    21  */
  LPAREN,       /*        (       22  */
  RPAREN,       /*        )       23  */
  LCURLY,       /*        {       24  */
  RCURLY,       /*        }       25  */
  LSQUARE,      /*        [       26  */
  RSQUARE,      /*        ]       27  */
  ERROR,        /*                28  */ 
  DOLLAR
} TokenType;

/*----演算子の型を列挙----*/
typedef enum {
  ot_PlusMinus, /*  +, -  */
  ot_MultiDiv,   /*  *, /  */
  ot_LPar,      /*  (     */
  ot_RPar,      /*  )     */
//  ot_Lsqu,      /*  [     */
//  ot_Rsqu,      /*  ]     */
  ot_Dollar,
  ot_error
} OpeType;

typedef enum {
  greater,     // >
  less,        // <
  equal,       // =
  uncorrect,   // ×
  end          // 終了
} OrderType;

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
