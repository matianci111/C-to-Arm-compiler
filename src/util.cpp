

#include "util.h"
vector<ST*> sym;
vector<Register*> Rs;
vector<char*> slv;
vector<int>fdsp;
int cs_index;
int currenttime;
int current_type;
int cfpv;
int regstack;
int fbranchsec;
int sbranchsec;
int functionarguments;
int maxfa;
int txtindex;

compiler_error debuge(expression *e1, Register *&reg){
	compiler_error err=compiler_Success;
	int fpv;
	int functiont;
	if(e1->type!=3){
		Register* localr= new Register();
		Register* localr2 = new Register();
		switch(e1->type){
			case 0:
				if(!err){err = type_promotion(617);}
				reqreg(localr);
				cout << "\tMov r" << localr->regn << ", #" <<  e1->a << endl;
				reg=localr;
				//constant
				break;
			case 1: cout << "oops I accessed something not implemented yet, due to a lack of time ofc <3 " << endl;
				if(!err){err = type_promotion(618);}
				break;
			case 2:
					if(!err){err = checkexp(e1->d, fpv);}
					reqreg(localr);
					cout << "\tLdr r" << localr->regn << ", [fp, #-" << fpv <<"]" << endl;
					reg=localr;
				//variable
				break;
			case 4: for(int i =0;i<slv.size();i++){
						if(slv[i]==e1->ds){
							reqreg(localr);
							cout << "\tLdr r" << localr->regn << ", .Lts+" << i*4<<endl;
							reg=localr;
							break;
						}
					}
					slv.push_back(e1->ds);
					reqreg(localr);
					cout << "\tLdr r" << localr->regn << ", .Lts+" << (slv.size()-1)*4<<endl;
					reg=localr;
				break;
			case 5:	functiont=0;
				 if(!err){err = checkfun(e1->d, functiont, e1->el);}
				 if(!err){err = debugel(e1->el);}
				if(!err){reqreg(localr);
				cout << "\tB " << e1->d << endl << "\tMov r" << localr->regn << ", r4" << endl;
				reg = localr;
				}
				//function call
				break;
			case 6:
					if(!err){err = checkexp(e1->d, fpv);}
					reqreg(localr);
					cout << "\tLdr r" << localr->regn << ", [fp, #-" << fpv <<"]" << endl;
				   switch(e1->op){
				   case 3:
						cout << "\tAdd r" <<localr->regn << ", r" << localr->regn << ", #1" << endl;
						cout << "\tstr r" << localr->regn << ", [fp, #-" << fpv <<"]" << endl;
				   break;
				   case 4:
						cout << "\tSub r" <<localr->regn << ", r" << localr->regn << ", #1" << endl;
						cout << "\tstr r" << localr->regn << ", [fp, #-" << fpv <<"]" << endl;
				   break;
				  
				  }
					reg=localr;
				  switch(e1->op){
				   case 1:
						reqreg(localr2);
						cout << "\tAdd r" <<localr2->regn << ", r" << localr->regn << ", #1" << endl;
						cout << "\tstr r" << localr2->regn << ", [fp, #-" << fpv <<"]" << endl;
						setfree(localr2);
				   break;
				   case 2:
						reqreg(localr2);
						cout << "\tSub r" <<localr2->regn << ", r" << localr->regn << ", #1" << endl;
						cout << "\tstr r" << localr2->regn << ", [fp, #-" << fpv <<"]" << endl;
						setfree(localr2);
				   break;
				  
				  }
		}
		return err;
	}
	exprs *e2 = e1->e;
	Register* localr1;
	Register* localr2;
	Register* localr3;

		if(!err){err=debuge(e2->es[0], localr1);}

	if(!err){err=debuge(e2->es[1], localr2);}

		regconsist(localr1);

	switch(e2->moper){
		case '=':
			int fpv1;
			if(!err){err = checkexp(e2->es[0]->d, fpv1);
			localr3 = new Register();
			cout << "\tstr r"<<localr2->regn <<", [fp,#-"<<fpv1<<"]" <<endl ;
			reqreg(localr3);
			cout << "\tmov r" <<localr3->regn << ", r" << localr2->regn << endl;
			current_type = 0;
			}
			//assign to variable
			break;
		case '+': 
					localr3 = new Register();
					reqreg(localr3);
					cout << "\tAdd r" << localr3->regn << ", r" << localr1->regn << ", r" <<localr2->regn<<endl;
			break;
		case '*':   		localr3 = new Register();
					reqreg(localr3);
					cout << "\tMul r" << localr3->regn << ", r" << localr1->regn << ", r" <<localr2->regn<<endl;
			break;
		case '-':   		localr3 = new Register();
					reqreg(localr3);
					cout << "\tSub r" << localr3->regn << ", r" << localr1->regn << ", r" <<localr2->regn<<endl;
			break;
		case '/':   		localr3 = new Register();
					reqreg(localr3);
					cout << "\tDiv r" << localr3->regn << ", r" << localr1->regn << ", r" <<localr2->regn<<endl;
			break;
		case EQ:   		localr3 = new Register();
					reqreg(localr3);
					cout << "\tcmp r" << localr1->regn << ", r" <<localr2->regn<<endl;
					cout << "\tMoveq r" << localr3->regn << ", #1"<< endl;
					cout << "\tMovne r" << localr3->regn << ", #0"<< endl;
			break;
		case NE: localr3 = new Register();
					reqreg(localr3);
					cout << "\tcmp r" << localr1->regn << ", r" <<localr2->regn<<endl;
					cout << "\tMoveq r" << localr3->regn << ", #0"<< endl;
					cout << "\tMovne r" << localr3->regn << ", #1"<< endl;
			break;
		case GE: 	localr3 = new Register();
					reqreg(localr3);
					cout << "\tcmp r" << localr1->regn << ", r" <<localr2->regn<<endl;
					cout << "\tMovge r" << localr3->regn << ", #1"<< endl;
					cout << "\tMovlt r" << localr3->regn << ", #0"<< endl;
			break;
		case LE: 	localr3 = new Register();
					reqreg(localr3);
					cout << "\tcmp r" << localr1->regn << ", r" <<localr2->regn<<endl;
					cout << "\tMovle r" << localr3->regn << ", #1"<< endl;
					cout << "\tMovgt r" << localr3->regn << ", #0"<< endl;
			break;
		case '>':   localr3 = new Register();
					reqreg(localr3);
					cout << "\tcmp r" << localr1->regn << ", r" <<localr2->regn<<endl;
					cout << "\tMovgt r" << localr3->regn << ", #1"<< endl;
					cout << "\tMovle r" << localr3->regn << ", #0"<< endl;
			break;
		case '<':   localr3 = new Register();
					reqreg(localr3);
					cout << "\tcmp r" << localr1->regn << ", r" <<localr2->regn<<endl;
					cout << "\tMovlt r" << localr3->regn << ", #1"<< endl;
					cout << "\tMovge r" << localr3->regn << ", #0"<< endl;
			break;
		
	}
	setfree(localr1);
	setfree(localr2);
	reg = localr3;
	return err;
}

compiler_error debugel(El* el){
	compiler_error err = compiler_Success;
	for(int i=0;i<el->eds.size();i++){
		Register* localr1;
		if(!err){err=debuge(el->eds[i], localr1);}
		
		cout << "\tstr r" << localr1->regn << ", [sp, #" <<functionarguments*4 << "]"<<endl;
		functionarguments++;
		setfree(localr1);
	}
	if(functionarguments>maxfa){
		maxfa=functionarguments;
	}
	functionarguments=0;
	return err;

}

compiler_error debugs(stmt *s){
	compiler_error err=compiler_Success;
	Register* localr1;
	switch(s->type){
		case 0:
			break;
		case 1: err = debuge(s->mexpr,  localr1) ;
				setfree(localr1);
			//just expression
			break;
		case 3: cout << "\tb .L"<< fbranchsec << endl;
			//return
			break;
		case 4:
			err = debuge(s->mexpr,  localr1);
			 cout << "\tMov r4, r"<< localr1->regn  << endl;
				setfree(localr1);
			 cout << "\tb .L"<< fbranchsec << endl;
			current_type = 0;
			cout << endl;
			//return expression;
			break;
		case 7: debugbs(s->blstmt);
			break;
	}
	return err;
}

compiler_error debugbs(Blstmt *blstmt){
	compiler_error err = compiler_Success;
	int enterd=sym.size();
	cs_index = sym.size();
	Register* localr1;
	int tmpbranch, tmpbranch2;
	switch(blstmt->type){
		case 1: 
			err = debuge(blstmt->expr,  localr1);
			cout << "\tcmp r" << localr1->regn << ", #0"<<endl;
			cout << "\tbne .Ls" << sbranchsec << endl;
			setfree(localr1);
			tmpbranch=sbranchsec;
			sbranchsec++;
			if(!err){err = debugbl(blstmt->bl);

			cout << ".Ls"<<tmpbranch<<":"<<endl;
			current_type=0;}
			break;
		case 2: cout << "\tb.Ls" <<sbranchsec << endl;
			tmpbranch=sbranchsec;
			sbranchsec++;
			tmpbranch2=sbranchsec;
			sbranchsec++;
			cout << ".Ls" << tmpbranch2<<endl;
			err = debugbl(blstmt->bl);
			cout << ".Ls"<<tmpbranch<<":"<<endl;
			if(!err){debuge(blstmt->expr, localr1);
			setfree(localr1);
			cout << "\tcmp r3, #0" << endl;
			cout << "\tbne .Ls"<<tmpbranch2 << endl;
			current_type = 0;}
			break;
		case 3:
			err= debuge(blstmt->expr,  localr1);
			tmpbranch=sbranchsec;
			sbranchsec++;
			tmpbranch2 = sbranchsec;
			sbranchsec++;
			cout << "\tcmp r" << localr1->regn << ", #0"<<endl;
			cout << "\tbeq .Ls" << tmpbranch << endl;
			setfree(localr1);
			if(!err){err = debugbl(blstmt->bl);
			cout << "\tb .Ls" << tmpbranch2 << endl;

			cout << ".Ls"<<tmpbranch<<":"<<endl;}
			if(!err){err= debugbl(blstmt->bl2);
			cout <<".Ls" <<tmpbranch2<<":" <<endl;
			
			current_type=0;}
			break;
		case 4:
		//init
			err = debugm(blstmt->m1);
			tmpbranch = sbranchsec;
			tmpbranch2 = ++sbranchsec;
			sbranchsec++;
			cout << "\tb .Ls" << tmpbranch << endl << ".Ls" << tmpbranch2 << ":" << endl;
			if(!err){err = debugbl(blstmt->bl);}
			if(!err){err = debuge(blstmt->expr, localr1);}
			setfree(localr1);
			cout << ".Ls" << tmpbranch << ":" << endl;
			if(!err){
				err=debuge(blstmt->e2, localr1);
				cout << "\tcmp r" << localr1->regn << ", #0" << endl;
				cout << "\tbne .Ls" << tmpbranch2 << endl;
				setfree(localr1);
			}
			
	}
	popback(enterd);
	cs_index = sym.size();
	return err;
}

compiler_error debugm(mixde *m){
	compiler_error err = compiler_Success;
	int fpv =0;
	Register* localr1;
	switch(m->type){
		case 1:
			err = debuge(m->expr, localr1);
			setfree(localr1);
			break;
		case 2:
			err = debugd(m->dcl);
			break;
		case 3:
		err = debugd(m->dcl);
			break;

	}
	return err;


}


compiler_error debugd(Dcl *d){
	compiler_error err = compiler_Success;
	int enterd;
	int fpv=0;
	Register* localr1;
	switch(d->type){
		case 1:
				err = checkdcl(d->tpi->iden, d->tpi->identype);
				
			break;
		case 2: 
				
				err=checkdcl(d->tpie->iden, d->tpie->identype);
				if(!err){err=checkexp(d->tpie->iden, fpv);}
				if(!err){err = debuge(d->tpie->mexpr, localr1);		
					setfree(localr1);
					cout << "\tstr r"<<localr1->regn <<", [fp,#-"<<fpv<<"]" <<endl ;		
				}
				current_type = 0;
			cout << endl;
			break;
		case 3:	
			err=checkfundcl(d->tf->iden, d->tf->identype, d->tf->dcll);
			enterd = sym.size();
			cs_index = sym.size();			
			if(!err){err = debugdl(d->tf->dcll);}
			popback(enterd);
			cs_index = sym.size();
			break;
	}
	return err;
}


compiler_error debugdl(Dcl_l *dl){
	compiler_error err=compiler_Success;
	int fpv =0;
	int argindex=0;
	for(int i =0;i<(dl->dcls.size());i++){
		if(!err){err=debugd(dl->dcls[i]);}
		if(dl->type ==1){
			checkexp(dl->dcls[i]->tpi->iden,fpv);
			cout <<"\t ldr r3, [fp, #" << (argindex*4)+20 << "]" << endl;
			cout <<"\t str r3, [fp, #-" << fpv << "]" << endl;
			argindex++;
		}
	}
	
	return err;
}

compiler_error debugb(block *b){
	compiler_error err=compiler_Success;
	switch(b->type){
		case 1:	err=debugd(b->dcll);
			break;
		case 2: err=debugs(b->stmtl);
			break;

	}
	return err;
}

compiler_error debugbl(block_l *bl){
	int enterd = sym.size();
	compiler_error err=compiler_Success;
	for(int i =0; i<(bl->bs.size());i++){
		if(!err){err=debugb(bl->bs[i]);}
	}
	popback(enterd);
	return err;
}

compiler_error debugfd(f_d *fd){
    cfpv = 1;
		int enterd=sym.size();
		cs_index = sym.size();
		compiler_error err=compiler_Success;
		cout << "\t.global " << fd->iden << endl;
		cout << fd->iden << ": " << endl;
		cout << "\tstmfd sp!, {r0-r3, fp, lr}" << endl << "\tadd fp, sp, #4" << endl << "\tsub sp, sp, undecided" << endl;

		err=debugdl(fd->mdcll);
		if(!err){err=debugbl(fd->mblock_l);}
		cout << ".L" <<fbranchsec << ":" << endl; 
		fbranchsec++;
		cout  << "\tsub sp, fp, #4" << endl << "\tldmfd sp!, {r0-r3, fp, pc}" << endl;
		fdsp.push_back(cfpv+maxfa);
		maxfa = 0;
	 cfpv = 1;	
	popback(enterd);
	cs_index = sym.size();
	return err;
}

compiler_error debugpu(Program_unit *pu){
	compiler_error err=compiler_Success;
	switch(pu->type){
		case 1:	err=debugfd(pu->fds);
			break;
		case 2: err=debugd(pu->dss);
			break;

	}
	return err;
}

compiler_error debugp(Program *p){
	currenttime = 0;
	for(int i =0;i<4;i++){
		currenttime++;
		Register* r = new Register();
		r->regn = i;
		r->avail = 1;
		r->timestamp = currenttime;
		Rs.push_back(r);	
	}
	regstack = 0;
	fbranchsec = 0;
	sbranchsec = 0;

	functionarguments=0;

	compiler_error err=compiler_Success;
	for(int i=0;i<(p->pus.size());i++){
		if(!err){err=debugpu(p->pus[i]);}
	}
	cout << ".Lts:" << endl;
	for(int i=0;i<slv.size();i++){
		cout << "\t" << ".word   .Lts" << i << endl;	
	}
	
	for(int i=0;i<slv.size();i++){
		cout << "\t" << ".align 2" << endl;
		cout << ".Lts" << i << ":" << endl;
		cout << "\t" << ".asciz " << slv[i]<<endl;
	}
	ofstream myfile;
	myfile.open("vector.txt");
	for(int i=0; i<fdsp.size();i++){
		myfile<<fdsp[i]*4<<endl;
	}
	myfile.close();
	
	return err;
}




compiler_error checkdcl(char* newid, int identype){
	compiler_error err = compiler_Success;
	int number = sym.size()-cs_index;
	for(int i =0;i<number;i++){
		if(strcmp(newid, sym[cs_index+i]->id)==0){
			cout << "declaration fails because identifier already exists" << endl;
			err = compiler_failed;
			return err;
		}
	}
	ST* st = new ST();
	st->type=1;
	st->id = newid;
	st->fpv = 0;
	st->identype=identype;
	sym.push_back(st);
	return err;
}

compiler_error checkfundcl(char* newid, int identype, Dcl_l* dcll){
	compiler_error err = compiler_Success;
	int number = sym.size()-cs_index;
	for(int i =0;i<number;i++){
		if(strcmp(newid, sym[cs_index+i]->id)==0){
			cout << "declaration fails because identifier already exists" << endl;
			err = compiler_failed;
			return err;
		}
	}
	ST* st = new ST();
	st->type=2;
	st->id = newid;
	st->identype=identype;
	st->dcll=dcll;
	sym.push_back(st);
	return err;
}


compiler_error checkexp(char* expid, int &fpv){
	compiler_error err = compiler_Success;
	for(int i =sym.size()-1;i>=0;i--){
		if(strcmp(expid, sym[i]->id)==0){
			err = type_promotion(sym[i]->identype);
			if(!err){
				if(sym[i]->fpv==0){
					cfpv++;
					sym[i]->fpv = cfpv*4;
				}
				fpv = sym[i]->fpv;
			}
			return err;
		}
	}
	cout << "\"declaration for identifier " << expid << " not found in this scope\"" << endl;
	err = compiler_failed;
	return err;
}


compiler_error checkfun(char* expid, int &returntype, El* el){
	compiler_error err = compiler_Success;
	for(int i =0;i<sym.size();i++){
		if(strcmp(expid, sym[i]->id)==0){
			returntype = 617; // sorry about this I can't implement what I thought I could implement
					if(!Samearguments(sym[i]->dcll,el)){
						cout << "function " << expid << " call fail" << endl;
						err = compiler_failed;
					}
			return err;
		}
	}
	cout << "\"declaration for function " << expid << " not found in this scope\"" << endl;
	err = compiler_failed;
	return err;
}

compiler_error type_promotion(int incomingtype){
	compiler_error err = compiler_Success;
	/*if(incomingtype!=current_type && (incomingtype == 619|incomingtype == 618)){
		cout << "Inconsistant type, can't convert" << endl;
		err = compiler_failed;
		return err;
	}

	if(incomingtype>current_type){
		current_type = incomingtype;
	}*/
	incomingtype = 617;
	return err;

}

string checktype(int i){
	string type;
	switch(i){
	case 617:type = "\"int\"";
		break;
	case 618:type = "\"void\"";
		break;
	case 619:type = "\"char*\"";
		break;
	}
	return type;
}

string converttotype(char a){
	string type;
	switch(a){
	case 'd':type = "\"int\"";
		break;
	case 'f':type = "\"double\"";
	}
	return type;
}

void popback(int n){
	int number = sym.size()-n;
	for(int i =0; i<number;i++){
		sym.pop_back();
	}
}

bool Samearguments(Dcl_l* dcll, El* el){
	bool same=true;
	if(dcll->containarg==1){
		return true;
	}
	if(el->eds.empty()||(dcll->dcls.empty()&&dcll->containarg!=1)){
		if(el->eds.empty()&&dcll->dcls.empty()){
			return true;		
		}
		else{
			cout << "not same amount of arguments in function call, fail!" << endl;
			return false;
		}
	}

	if(el->eds.size()!=dcll->dcls.size()){
		cout << "not same amount of arguments in function call, fail!" << endl;
		return false;
	}
	return same;


}

void reqreg(Register* &localr){
	for(int i = 0;i<4;i++){
		if(Rs[i]->avail==1){
			Rs[i]->timestamp = currenttime;
			currenttime++;
			Rs[i]->avail = 0;
			localr->timestamp = Rs[i]->timestamp;
			localr->avail = 0;
			localr->regn=Rs[i]->regn;
			return;
		}
	}
	Register* last = new Register();
	last->timestamp = currenttime;
	currenttime++;
	for(int i = 0;i<4;i++){
		if(Rs[i]->timestamp < last->timestamp){
			last->timestamp = Rs[i]->timestamp;
			last->regn = Rs[i]->regn;
			last->avail = Rs[i]->avail;

		}
	}
	for(int i = 0;i<4;i++){
		if(Rs[i]->regn == last->regn){
				Rs[i]->timestamp=currenttime;
				currenttime++;
				last->timestamp=Rs[i]->timestamp;
		}
	}
	
	regstack = regstack+4;
	cout << "\tstr r" << last->regn << " [sp, #-" << regstack <<"]!" << endl;
	localr=last;

}

void regconsist(Register* localr1){
	for(int i=0;i<4;i++){
		if(localr1->regn == Rs[i]->regn){
			if(localr1->timestamp!=Rs[i]->timestamp){
				reqreg(localr1);
				cout << "\tldr r" << localr1->regn <<", [sp, #-" << regstack <<"]!" << endl;
				regstack=regstack-4;
				return;
			}
		}
	
	}

}

void setfree(Register* tmp){
	for(int i =0;i<4;i++){
		if(tmp->regn == Rs[i]->regn){
			tmp->avail=1;
			Rs[i]->avail=1;
			break;
		}
		
	}

}
