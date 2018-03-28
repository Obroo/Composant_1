#include <iostream>

#include "FileInterface.h"
using namespace std;

int main(int argc, char* argv[]) {
	cout << "test" << endl;
	//Composant5 c5;
	FileInterface f("livreDeCompte.json"); // attention au path du projet
	f.verification();
	Bloc response = f.findByIndex(2);
	f.insert(response);
	cout<<"hash : " <<f.toString(true,2)<<endl;
	return 0;
}