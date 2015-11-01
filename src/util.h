#ifndef _XXX_
#define _XXX_
 
#include <iostream>
#include "classes.h"
#include <stdlib.h>
#include <fstream>
#include "error.h"
#include "y.tab.h"
#include <string>
#include <cstring>

using namespace std;

compiler_error debuge(expression *e, Register *&reg);
compiler_error debugel(El* el);
compiler_error debugd(Dcl *d);
compiler_error debugdl(Dcl_l *dl);
compiler_error debugs(stmt *s);
compiler_error debugb(block *b);
compiler_error debugbs(Blstmt *blstmt);
compiler_error debugbl(block_l *bl);
compiler_error debugfd(f_d *fd);
compiler_error debugpu(Program_unit *pu);
compiler_error debugp(Program *p);
compiler_error debugm(mixde *m);
compiler_error printline(stmt *s);


compiler_error checkdcl(char* newid, int identype);
compiler_error checkfundcl(char* newid, int identype, Dcl_l* dcll);
compiler_error checkexp(char* expid, int &fpv);
compiler_error checkfun(char* expid, int &returntype, El* el);
compiler_error type_promotion(int incomingtype);
void popback(int n);
string checktype(int i);
string converttotype(char a);
bool Samearguments(Dcl_l* dcll, El* el);
void reqreg(Register* &localr);
void regconsist(Register* localr1);
void setfree(Register* tmp);

#endif
