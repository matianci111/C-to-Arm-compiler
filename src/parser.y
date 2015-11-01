%code requires{
     #include <iostream>
     #include <cstdlib>
	 #include <vector>
	 #include "util.h"
	 using namespace std;
     void yyerror(char *s);
     int yylex(void);
	 
}

%union{
	int  cvalue;
	double dvalue;
	char *id;
	Dcl  *dclu;
	Program_unit *pu;
	Program *p;
	expression *expr;
	f_d *fd;
	block_l *bl;
	Dcl_l *dcll;
	block *b;
	stmt *mstmt;
	Stmt_l *stmtl;
	El *el;
	Blstmt *blstmt;
	mixde *m;
	
}
%token <id> P_INC
%token <id> STRING_LITERAL
%token DOUBLE
%token INT
%token VOID
%token CONST
%token CHAR
%token RETURN
%token ARG_LIST
%token FOR
%token ELSE
%token IF
%token WHILE
%token INCREMENT
%token DECREMENT
%token <cvalue> iCONSTANT
%token <dvalue> dCONSTANT
%token <id> IDENTIFIER
%left '='
%left EQ NE '>' GE '<' LE
%left '+' '-'
%left '*' '/'


%type <el> expr_list
%type <expr> expr
%type <expr> function_call
%type <expr> assign_expr
%type <cvalue> type
%type <blstmt> block_statement
%type <blstmt> for_statement 
%type <blstmt> if_statement
%type <mstmt> statement
%type <dclu> declaration
%type <m> mixde
%type <dcll> declaration_list
%type <b> block
%type <bl> block_list
%type <fd> function_definition
%type <pu> program_unit
%type <p> program
%start startpoint
%%
startpoint: program {compiler_error err = compiler_Success; err=debugp($1); cout << "Ended with err " << err << endl;}
			;

program:	program_unit {$$=new Program(); $$->Add($1);}
		|program program_unit {$1->Add($2);$$=$1;}
		;

program_unit:	
		function_definition {$$=new Program_unit();$$->type=1;$$->fds=$1; }
		|declaration';'{$$=new Program_unit();$$->type = 2;$$->dss = $1; }
		|P_INC {$$=new Program_unit();$$->type=3;$$->PIs=$1;}
		;


function_definition:
		type IDENTIFIER '(' ')' '{' '}' {$$=new f_d(); $$->Add($1, $2, new Dcl_l(), new block_l()); }
		|type IDENTIFIER '(' declaration_list ')' '{' '}' {$$=new f_d(); $$->Add($1, $2, $4, new block_l());}
		|type IDENTIFIER '('  ')' '{' block_list '}' {$$=new f_d(); $$->Add($1, $2, new Dcl_l(), $6); }
		|type IDENTIFIER '(' declaration_list ')' '{' block_list '}' {$$=new f_d(); $$->Add($1, $2, $4, $7);$4->type=1;}
		|type IDENTIFIER '(' declaration_list ',' ARG_LIST ')' '{' block_list '}' {$$=new f_d(); $$->Add($1, $2, $4, $9);$4->type=1;}
		;

declaration	:type IDENTIFIER{$$=new Dcl(); Tp_I *tp_i=new Tp_I($1,$2); $$->Addtpi(tp_i); }
		|type IDENTIFIER '=' expr  {$$=new Dcl();Tp_IE *tpie = new Tp_IE($1,$2,$4); $$->Addtpie(tpie);}
		|type IDENTIFIER '(' ')' {$$=new Dcl();Dcl_l *dcll = new Dcl_l(); dcll->containarg=0; Tf *tf = new Tf($1, $2, dcll); $$->Addtf(tf);}
		|type IDENTIFIER '(' declaration_list ')' {$$=new Dcl();$4->containarg=0; Tf *tf = new Tf($1, $2, $4); $$->Addtf(tf);$4->type =2;}
		|type IDENTIFIER '(' declaration_list ',' ARG_LIST ')' {cout <<"ha" <<endl;$$=new Dcl();$4->containarg=1; Tf *tf = new Tf($1, $2, $4);  $$->Addtf(tf);$4->type =2;}
		;


declaration_list:
		declaration {$$=new Dcl_l(); $$->Add($1); }
		|declaration_list ',' declaration {$1->Add($3);$$=$1;}
		;//done

type:		INT {$$=617; }
		|VOID {$$=618;}
		|CONST CHAR '*' {$$=619;}
		;//done
		
		
block_list:	
		block {$$=new block_l();$$->Add($1);}
		|block_list block {$1->Add($2);$$=$1;}
		;

block:		declaration ';'{$$=new block(); $$->type=1; $$->dcll=$1;}
		|statement {$$=new block(); $$->type=2; $$->stmtl=$1;}
		;	


statement:	';' {$$=new stmt(); $$->type =0;}
		|expr ';' {$$=new stmt(); $$->type=1;$$->mexpr=$1;}
		|RETURN ';' {$$=new stmt(); $$->type =3;}
		|RETURN expr ';' {$$=new stmt(); $$->type=4; $$->mexpr=$2;}
		|block_statement  {$$=new stmt(); $$->type =7; $$->blstmt = $1;}
		;//done
		
block_statement:
		if_statement {$$=$1;}
		|WHILE '(' expr ')' '{' block_list '}'  {$$=new Blstmt(); $$->type = 2; $$->expr = $3; $$->bl = $6;}
		|WHILE '(' expr ')' '{'  '}'  {$$=new Blstmt(); $$->type = 2; $$->expr = $3; $$->bl = new block_l();}
		|if_statement ELSE '{' '}' {$$=$1;}
		|if_statement ELSE '{' block_list '}' {$1->bl2=$4; $$ =$1; $$->type = 3;}
		|for_statement {$$=$1;}
		
		;
if_statement:
		IF '(' expr ')' '{' block_list '}' {$$=new Blstmt(); $$->type = 1; $$->expr = $3; $$->bl = $6;}
		|IF '(' expr ')' '{' '}' {$$=new Blstmt(); $$->type = 1; $$->expr = $3; $$->bl = new block_l();}
		;
for_statement:
		FOR '(' mixde ';' expr ';' expr ')' '{' '}' {$$=new Blstmt(); $$->type = 4; $$->expr = $7; $$->bl = new block_l(); $$->m1=$3; $$->e2=$5;}
		| FOR '(' mixde ';' expr ';' expr ')' '{' block_list '}' {$$=new Blstmt(); $$->type = 4; $$->expr = $7; $$->bl = $10;$$->m1=$3; $$->e2=$5;}
		;

mixde:	expr	{$$=new mixde(); $$->type=1; $$->expr = $1;}
		|type IDENTIFIER {$$=new mixde(); $$->type=2;Dcl* dcl=new Dcl(); Tp_I *tp_i=new Tp_I($1,$2); dcl->Addtpi(tp_i);$$->dcl = dcl ;}
		|type IDENTIFIER '=' expr {$$=new mixde(); $$->type=3;Dcl* dcl=new Dcl(); Tp_IE *tp_ie=new Tp_IE($1,$2,$4); dcl->Addtpie(tp_ie);$$->dcl = dcl ;}
		;

expr_list:
		expr {$$=new El(); $$->eds.push_back($1);}
		|expr_list ',' expr  {$1->eds.push_back($3);$$=$1;}
		;

expr:		iCONSTANT{$$=new expression(); $$->type=0; $$->a=$1;} 
		|dCONSTANT{$$=new expression(); $$->type=1; $$->b=$1;} 
		|IDENTIFIER {$$=new expression(); $$->type=2; $$->d=$1;}
		|STRING_LITERAL {$$=new expression(); $$->type=4; $$->ds=$1;}
		|IDENTIFIER '=' expr{$$=new expression(); $$->type=3; expression* e1 = new expression(); e1->type=2; e1->d=$1; exprs *es = new exprs();es->Add(e1,('='),$3);$$->e=es;}
		|expr '+' expr {$$=new expression(); $$->type=3; exprs *es = new exprs();es->Add($1,('+'),$3);$$->e=es;}
		|expr '*' expr {$$=new expression(); $$->type=3; exprs *es = new exprs();es->Add($1,('*'),$3);$$->e=es;}
		|expr '-' expr {$$=new expression(); $$->type=3; exprs *es = new exprs();es->Add($1,('-'),$3);$$->e=es;}
		|expr '/' expr {$$=new expression(); $$->type=3; exprs *es = new exprs();es->Add($1,('/'),$3);$$->e=es;}
		|assign_expr {$$=$1;}
		| '(' expr ')' {$$=$2;}
		|function_call {$$ = $1;}
		;



function_call:
		IDENTIFIER '(' ')'  {$$=new expression(); $$->type = 5; $$->d=$1; $$->el=new El();}
		|IDENTIFIER '(' expr_list ')' {$$=new expression(); $$->type = 5; $$->d=$1; $$->el=$3;}
		|IDENTIFIER INCREMENT  {$$=new expression(); $$->type = 6; $$->d=$1; $$->op=1;}
		|INCREMENT IDENTIFIER {$$=new expression(); $$->type = 6; $$->d=$2; $$->op=3;}
		|IDENTIFIER DECREMENT {$$=new expression(); $$->type = 6; $$->d=$1; $$->op=2;}
		|DECREMENT IDENTIFIER {$$=new expression(); $$->type = 6; $$->d=$2; $$->op=4;}
		;//done

assign_expr:
		expr EQ expr {$$=new expression(); $$->type=3; exprs *es = new exprs();es->Add($1, EQ ,$3);$$->e=es;}
		|expr '>' expr {$$=new expression(); $$->type=3; exprs *es = new exprs();es->Add($1,('>'),$3);$$->e=es;}
		|expr GE expr {$$=new expression(); $$->type=3; exprs *es = new exprs();es->Add($1, GE ,$3);$$->e=es;}
		|expr LE expr {$$=new expression(); $$->type=3; exprs *es = new exprs();es->Add($1, LE ,$3);$$->e=es;}
		|expr '<' expr {$$=new expression(); $$->type=3; exprs *es = new exprs();es->Add($1,('<'),$3);$$->e=es;}
		|expr NE expr {$$=new expression(); $$->type=3; exprs *es = new exprs();es->Add($1, NE ,$3);$$->e=es;}
		
		;
		
%%

void yyerror(char *s){
	std::cout<< "error!" << s  << std::endl;
}

int main(void){
	yyparse();
	return 0;
}
