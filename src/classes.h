#include <vector>
using namespace std;



class Value{
	public:
		virtual ~Value(){}
		virtual void Print(ostream &dst) const=0;
		virtual void freetree(){}
};


class exprs: public Value{
	public:
	    exprs(){};
	    int moper;
	    vector<class expression *>es;
	void Add(expression* e1, int oper,expression* e2)
	{
		es.push_back(e1);
		moper = oper;
		es.push_back(e2);
	}
	virtual void Print(ostream &dst) const
	{
	}
	virtual void freetree(){
		es.clear();
	}
};

class expression: public Value{
	public:
	    expression(){};
	    int type;
	    union{
		int a;
		double b;
		char *d;
		char *ds;
		exprs *e;
	    };
	    class El* el;
		int op;
	virtual void Print(ostream &dst) const
	{
	}
	virtual void freetree(){
		delete(e);
	}
};

class El: public Value{
	public:
		El(){};
		vector<expression*>eds;
		
	virtual void Print(ostream &dst) const
	{
	}
	virtual void freetree(){
	}
};

class Blstmt: public Value{
	public:
		Blstmt(){};
		int type;
		expression* expr;
		expression* e2;
		class mixde* m1;
		class block_l *bl;
		class block_l *bl2;
	virtual void Print(ostream &dst) const
	{
	}
	virtual void freetree(){
	}

}; 


class stmt: public Value{
	public:
	    stmt(){};
	    int type;
	    char* iden;
	    expression* mexpr;
		Blstmt *blstmt;

	virtual void Print(ostream &dst) const
	{
	}
	virtual void freetree(){
		delete(mexpr);
	}
};

class Stmt_l: public Value{
	public:
	    Stmt_l(){};
	    vector<stmt*>ss;
	void Add(stmt *value)
	{
		ss.push_back(value);
	}
	virtual void Print(ostream &dst) const
	{
	}
	virtual void freetree(){
		ss.clear();
	}
};


class Tp_I : public Value{
	public:
		Tp_I(int typen, char *d):identype(typen), iden(d){};
		int identype;
		char *iden;
		virtual void Print(ostream &dst) const
		{
		}
		virtual void freetree(){
		}
};

class Tp_IE : public Value{
	public:
		Tp_IE(int typen, char *d, expression *expr):identype(typen), iden(d), mexpr(expr){};
		int identype;
		char *iden;
		expression *mexpr;
		virtual void Print(ostream &dst) const
		{
		}
		virtual void freetree(){
			delete(mexpr);
		}
};

class Tf : public Value{
	public:
		Tf(int typen, char* it, class Dcl_l *dcl):identype(typen), iden(it), dcll(dcl){};
		int identype;
		char* iden;
		class Dcl_l *dcll;
		virtual void Print(ostream &dst) const
		{
		}
		virtual void freetree(){
		}

};


class Dcl : public Value{
	public:
		Dcl(){};
		int type;
		Tp_I* tpi;
		Tp_IE* tpie;
		Tf* tf;
		void Addtpi(Tp_I *value)
		{
			type = 1;
			tpi=value;
		}
		void Addtpie(Tp_IE *value)
		{
			type = 2;
			tpie=value;
		}
		void Addtf(Tf *value)
		{
			type = 3;
			tf=value;
		}
		
		virtual void Print(ostream &dst) const
		{
		}
		virtual void freetree(){
			delete(tpi);
			delete(tpie);
			delete(tf);
		}
};

class mixde: public Value{
	public:
		int type;
		Dcl* dcl;
		expression* expr;
		virtual void Print(ostream &dst) const
		{
		}
		virtual void freetree(){
		}
};


class Dcl_l: public Value{
	public:
		Dcl_l(){};
		int type;
		int containarg;
		vector<Dcl*>dcls;
		void Add(Dcl *value)
		{
			dcls.push_back(value);
		}
		virtual void Print(ostream &dst) const
		{
		}
		virtual void freetree(){
			dcls.clear();
		}

};

class block: public Value{
	public:
		block(){};
		int type;
		Dcl *dcll;
		stmt *stmtl;
		virtual void Print(ostream &dst) const
		{
		}
		virtual void freetree(){
			delete(dcll);
			delete(stmtl);
		}

		
};

class block_l: public Value{
	public:
		block_l(){};
		vector<block*> bs;
		void Add(block* value)
		{
			bs.push_back(value);
		}
		virtual void Print(ostream &dst) const
		{
		}
		virtual void freetree(){
			bs.clear();
		}

		
};



class f_d: public Value{
	public:
		f_d(){};
		int function_type;
		char* iden;
		Dcl_l *mdcll;
		block_l *mblock_l;
		void Add(int value1, char *it, Dcl_l *dcll, block_l *value2)
		{
			function_type = value1;
			iden = it;
			mdcll = dcll;
			mblock_l = value2;
		}
		virtual void Print(ostream &dst) const
		{
		}
		virtual void freetree(){
			delete(mdcll);
			delete(mblock_l);
		}

};

class Program_unit: public Value{
	public:
		Program_unit(){};
		int type;
		f_d *fds;
		Dcl *dss;
		char *PIs;
		virtual void Print(ostream &dst) const
		{
		dst<<"yo i know"<<endl;
		}
		virtual void freetree(){
			delete(fds);
			delete(dss);
		}
};

class Program: public Value{
	public:
		Program(){};
		vector<Program_unit*> pus;
		void Add(Program_unit *value)
		{
			pus.push_back(value);
		}
		virtual void Print(ostream &dst) const
		{
		dst<<"yo i have " << pus.size() << " program units!" <<endl;
		}
		virtual void freetree(){
			pus.clear();
		}
};

class ST{
	public:
		ST(){};
		int type;
		char* id;
		int identype;
		int fpv;
		Dcl_l* dcll;

};


class Register{
	public:
		Register(){};
		int regn;
		int avail;
		int timestamp;
};
