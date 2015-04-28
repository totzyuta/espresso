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
  // arithem,  /* 算術演算子 +-                */
  eq,       /* 4: =                            */
  sign,     /* 5: + - * / { } [ ] ( ) ;     */
  excl,     /* 6: !                            */
  // relation, /* 比較　 < >                   */
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
  FUNC,         /* 関数宣言 func  04  */
  IDENT,	      /* 識別子	       	05  */
  ADD,		      /* 加算演算子 +	 	06  */
  SUB,          /* 減算演算子 -   07  */
  MUL,  	    /* 乗算演算子 *   08  */
  DIV,          /* 除算演算子 /   09  */
  EQUAL,        /* 等号   =       10  */
  GREATER,      /* 大なり >       11  */
  EQGREATER,    /*        >=      12  */
  LESS,         /* 小なり <       13  */
  EQLESS,       /*        <=      14  */
  NEQUAL,       /*        !=      15  */
  EQUAL2,       /* 比較  ==       16  */
  COMMA,        /*コンマ  ,       17  */
  SEMICOLON,    /*セミコロン ;    18  */
  LPAREN,       /*        (       19  */
  RPAREN,       /*        )       20  */
  LCURLY,       /*        {       21  */
  RCURLY,       /*        }       22  */
  LSQUARE,      /*        [       23  */
  RSQUARE,      /*        ]       24  */
  ERROR,        /*                25  */ 
  DOLLAR
} TokenType;

/*----演算子の型を列挙----*/
typedef enum {
  ot_PlusMinus, /*  +, -  */
  ot_MultDiv,   /*  *, /  */
  ot_LPar,      /*  (     */
  ot_RPar,      /*  )     */
  ot_Lsqu,      /*  [     */
  ot_Rsqu,      /*  ]     */
  ot_dollar
} OpeType;

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
