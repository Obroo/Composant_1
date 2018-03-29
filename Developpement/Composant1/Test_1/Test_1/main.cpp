#include <iostream>

#include "FileInterface.h"

using namespace std;

int main(int argc, char* argv[]) {
	cout << "TEST COMPOSANT 1" << endl;
	FileInterface f("..\\..\\Composant1\\livreDeCompte.json"); // attention au path du projet
	f.verification();
	Bloc response = f.findByIndex(2);
	f.insert(response);
	cout<<"Resultat toString : " <<f.toString(true,2)<<endl;
	return 0;
}