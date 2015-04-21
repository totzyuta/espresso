/*文字タイプ*/
#define TOKENMAX 100	/* トークンの最大文字数 */
#define MaxStack 128

/*extern int flag;*/
/*--< 文字のタイプ >--*/
typedef enum {
  delim,	/* 0: 区切り記号(空白,TAB,改行)		*/
  number,	/* 1: 数字 0～9				*/
  alpha,        /* 英字 a～z, A～Z                       */
  arithem,      /* 算術演算子 +-                          */
  relation,     /* 比較　 < >                           */
  mark,         /* 記号 , ; ( ) { } [ ]                  */
  equal,        /* =                                    */
  error		/* 未定義文字				*/
} CharType;

/*--< 状態のタイプ >--*/
typedef enum {
  Init,           /*初期状態*/
  Int,            /*数状態*/
  Identifer,      /*識別子状態*/
  Arithmetic,     /*算術演算子状態*/
  Relational,     /*比較演算子*/
  Mark,           /*記号状態*/
  Equal,          /*等号状態*/
  Final           /*終了状態*/
} StateType;

/*--<トークンのタイプ >--*/
typedef enum {
  INTEGER,	/* 整数		00 	 */
  IDENT,	/* 識別子	01	 */
  ADD,		/* 加算演算子 +	02	 */
  SUB,          /* 減算演算子 -    3       */
  MULTI,	/* 乗算演算子 *    4       */
  DIV,          /* 除算演算子 /    5       */
  EQUAL,        /* 等号      =    6       */
  EQUAL2,       /* 等号2     ==   7       */
  GREATER,      /* 大なり    >     8       */
  EQUGRET,      /*          >=    9       */
  LESS,         /* 小なり    <     10      */
  EQULESS,      /*          <=    11      */
  COMMA,        /*コンマ     ,     12      */
  SEMICOLON,    /*セミコロン  ;     13      */
  LAREN,        /*          (     14      */
  RPAREN,       /*          )     15      */
  LCURLY,       /*          {     16      */
  RCURLY,       /*          }     17      */
  LSQUARE,      /*          [     18      */
  RSQUARE,      /*          ]     19      */
  VOID,         /*          void     20   */
  DEFINE,       /*          define   21   */
  ARRAY,        /* 格納子    array    22   */
  WHILE,	/* 予約語(while)22    23   */
  IF,           /* 予約語(if)         24   */
  ELSE,          /* 予約後(else)      25   */ 
  ERROR,         /*      26  */ 
  DOLLAR
} TokenType;

/*----演算子の型を列挙----*/
typedef enum {
  ot_PlusMinus, /* ＋，－ */
  ot_MultDiv,   /* ＊，／ */
  ot_LPar,      /*  （    */
  ot_RPar,       /*   )   */
  ot_Lsqu,      /*  [  */
  ot_Rsqu,       /*   ]   */
  ot_dollar
} OpeType;

/*--< トークンを格納する構造体 >--*/
typedef struct {
  char 		string[TOKENMAX];	/* トークンの文字列 */
  TokenType	type;			/* トークンのタイプをすべて保持してるdefine.h:31 */
} TokenSt;

typedef struct node {
  TokenSt *token;              
  struct node *left;
  struct node *right;
}Node;

