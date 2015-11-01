	#include <iostream>
#include <fstream>
#include <sstream>
#include <string> 
#include <vector>
using namespace std;


int main(){
	int index=0;
	ifstream myfile;
	myfile.open("vector.txt");
	string h;
	vector<string> sp;

	while(myfile>>h){
	  sp.push_back(h);
	}
	myfile.close();


    myfile.open("output.txt");
    string line;
    string wordEntered("undecided"); // Get it from the command line


    while( getline(myfile, line) ) {

        stringstream ls( line );
        string word;

        while(ls >> word)
        {
            if (word != wordEntered)
            {
                 cout << word << " ";
            }
			else{
				cout << sp[index];
				index++;
			}
        }
        cout << endl;
    }

	return 0;
}