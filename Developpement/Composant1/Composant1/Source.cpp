#include <iostream>

#include "../../../Interface Fichier/FileInterfaceAPI.h"

using namespace std;

int main(int argc, char* argv[]) {
	cout << "test" << endl;
	FileInterface f("livreDeCompte.json"); // attention au path du projet
	f.verification();
	Bloc response = f.findByIndex(0);
	f.insert(response);
	cout<<"hash : " <<response.hash<<endl;
	return 0;
}