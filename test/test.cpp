int tianci(const char * str, ...);
int main(){
	int b;
	int c;
	int d =tianci("tianci",4);
	b =2;
	int a=4+((b+c)*3+4+(3+(5+d)))+2;
	for(int i =0;i<5;i++){
		if(b==0){
			b= b + b++ + b;
		}
		else{
		  b=3;
		}
	}
	
	if(b==0){
	
	   b = 3;
	}
	tianci("hahahahha2", 5);
	return 0;

}

int tianci(const char * str, ...){
	return 1;

}
