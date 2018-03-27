#include<string>
#include <iostream>
#include <fstream>
#include <stdio.h>

#include "FileInterfaceAPI.h"

using namespace std;

// Constructeur : initialisation la classe et lit le fichier json passé en paramètre
FileInterface::FileInterface(string file_path) {
	fichier = file_path;
	ifstream ifs(fichier);
	if (ifs.fail()) {
		cout << "la lecture du fichier a echoue" << endl;
		throw new string("la lecture du fichier a echoue");
	}
	else {
		cout << "lecture du fichier : " << fichier << " a reussi." << endl;
	}
}

void FileInterface::insert(Bloc bloc)
{
	//On utilise le composant_5 pour vérifier si le bloc respecte les spécifications
	//Faut-il vérifier les transactions ou pas?
	/*CComposant5 c5;
	bool ans = c5.verify_bloc(bloc);
	if (ans == false) {
	throw std::invalid_argument("The bloc did not respect the specifications.");
	}*/
	ifstream ifs(fichier); // lit le fichier
	json j = json::parse(ifs); // transforme en json
	auto jsonObjects = json::array();
	for (const auto& it : j) {
		jsonObjects.push_back(it);
	}
	bloc.num = 5;
	string s = toString(true, bloc.hash);
	//json js = s;
	std::time_t rawtime;
	std::tm* timeinfo;
	char buffer[80];

	std::time(&rawtime);
	timeinfo = std::localtime(&rawtime);

	std::strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", timeinfo);
	string version = "0x2000";
	string ID_init = "1";
	string ID_recev = "2";
	json js;
	js["Identifiant"] = to_string(bloc.num);
	js["NombreDeTransaction"] = to_string(1);
	js["Hashes"]["Hash"] = bloc.hash;
	js["Hashes"]["BlocPrecedent"] = string(bloc.previous_hash);
	js["DateDeCreation"] = buffer;
	js["Version"] = version;
	js["Transaction"] = json::array();
	for (unsigned int i = 0; i < bloc.tx1.UTXOs.size(); i++) {
		js["Transaction"][i]["Id"] = to_string(bloc.tx1.UTXOs[i].nUTX0);
		js["Transaction"][i]["date"] = buffer;
		js["Transaction"][i]["Bloc"] = to_string(bloc.tx1.UTXOs[i].nBloc);
		js["Transaction"][i]["ID_init"] = ID_init;
		js["Transaction"][i]["ID_recev"] = ID_recev;
		js["Transaction"][i]["montant"] = to_string(bloc.tx1.UTXOs[i].montant);
	}
	jsonObjects.emplace_back(js);
	std::ofstream o("output.json");
	o << std::setw(4) << jsonObjects << std::endl;
}

string FileInterface::toString(bool allInfos, string hash) {
	std::time_t rawtime;
	std::tm* timeinfo;
	char buffer[80];

	std::time(&rawtime);
	timeinfo = std::localtime(&rawtime);

	std::strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", timeinfo);
	string version = "0x2000";
	string ID_init = "1";
	string ID_recev = "2";
	Bloc b = findByHash(hash);
	string str;
	str = "{ \n\t\"identifiant\": \"" + to_string(b.num);
	str += "\",\n\t\"NombreDeTransaction\": \"1\",\n\t\"Hashes\": { \n\t\t";
	if (allInfos == true) {
		str += "\"Hash\": \"" + string(b.hash) + ",\"";
	}
	str += "\"BlocPrecedent\": \"" + string(b.previous_hash);
	str += "\"},\n\t\"DateDeCreation\": \"" + string(buffer);
	str += "\",\"Version\": \"" + version;
	str += "\",\"Transactions\": [";
	for (unsigned int i = 0; i < b.tx1.UTXOs.size(); i++) {
		str += "\"Id\": \"" + b.tx1.UTXOs[i].nUTX0;
		str += "\",\"date\": \"" + string(buffer);
		str += "\",\"Bloc\": \"" + b.tx1.UTXOs[i].nBloc;
		str += "\",\"ID_init\": \"" + ID_init;
		str += "\",\"ID_recev\": \"" + ID_recev;
		str += "\",\"montant\": \"" + to_string(b.tx1.UTXOs[i].montant);
		str += "\"}";
		if (i != (b.tx1.UTXOs.size() - 1)) {
			str += ",";
		}
	}

	str += "]";
	return str;
}

string FileInterface::toString(bool allInfos, int index) {
	std::time_t rawtime;
	std::tm* timeinfo;
	char buffer[80];

	std::time(&rawtime);
	timeinfo = std::localtime(&rawtime);

	std::strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", timeinfo);
	string version = "0x2000";
	string ID_init = "1";
	string ID_recev = "2";
	Bloc b = findByIndex(index);
	string str;
	str = "{ \n\t\"identifiant\": \"" + to_string(b.num);
	str += "\",\n\t\"NombreDeTransaction\": \"1\",\n\t\"Hashes\": { \n\t\t";
	if (allInfos == true) {
		str += "\"Hash\": \"" + string(b.hash) + ",\"";
	}
	str += "\"BlocPrecedent\": \"" + string(b.previous_hash);
	str += "\"},\n\t\"DateDeCreation\": \"" + string(buffer);
	str += "\",\"Version\": \"" + version;
	str += "\",\"Transactions\": [";
	for (unsigned int i = 0; i < b.tx1.UTXOs.size(); i++) {
		str += "\"Id\": \"" + b.tx1.UTXOs[i].nUTX0;
		str += "\",\"date\": \"" + string(buffer);
		str += "\",\"Bloc\": \"" + b.tx1.UTXOs[i].nBloc;
		str += "\",\"ID_init\": \"" + ID_init;
		str += "\",\"ID_recev\": \"" + ID_recev;
		str += "\",\"montant\": \"" + to_string(b.tx1.UTXOs[i].montant);
		str += "\"}";
		if (i != (b.tx1.UTXOs.size() - 1)) {
			str += ",";
		}
	}

	str += "]";

	return str;
}


// Parcours tous les blocs afin de savoir s'ils sont conformes - génére une exception si un n'est pas conforme
void FileInterface::verification() {
	cout << "test2" << endl;
	vector<Bloc> liste_blocs = readAll(); // transforme le fichier en un vecteur de Bloc
	for (vector<Bloc>::iterator it = liste_blocs.begin(); it != liste_blocs.end(); ++it) {
		Bloc bloc = *it;
		cout << "Bloc : " << bloc.num << endl;
		// on parcours la liste et on appelle la methode verification bloc du composant5
		//if (!verify_bloc(bloc)) { // tester car methode verify_bloc attend un Bloc*
			//throw new string("erreur lors de la verification d'un bloc");
		//}
	}
	cout << "verification successful." << endl;
}

//Reçoit un file path et retourne un vector contenant tous les blocs
vector<Bloc> FileInterface::readAll() {
	vector<Bloc> liste_blocs;
	json j;
	try	{
		// parsing input with a syntax error
		ifstream ifs(fichier); // lit le fichier
		j = json::parse(ifs); // transforme en json
	
		// parcours le tableau json contenant les blocs
		for (const auto& it : j) {
			Bloc b;
			//recuperation transactions
			/*************************************************TODO******************************************************************/
			//recuperer json de it["TX]
			//lire dans un vector<json> chaque txi et txo
			// lire les transactions du mineures et ajouter dans utxo
			json transactions = it["TX"];
			UTXO utxo;
			TXI txi;
			//recuperation txi et utxo
			for (const auto& i : transactions.at("TXI")) {
				cout << i.at("nTx") << endl;
				// block number and id of the source Utxo
				string nTx = i.at("nTx");
				string bloc = i.at("nBloc");
				string nUTXO = i.at("nUTXO");
				string signature = i.at("signature");
				//cast des valeurs en int ou float
				txi.nBloc = stoi(bloc);
				txi.nTx = stoi(nTx);
				txi.nUtxo = stoi(nUTXO);
				copy(signature.begin(), signature.end(), txi.signature);
				txi.signature[signature.length()] = 0;
				//ajout d'un txi
				b.tx1.TXIs.push_back(txi);
			}
			for (const auto& i : transactions.at("UTXO")) {
				cout << i.at("nTx") << endl; 
				// block number and id of the source Utxo
				string nTx = i.at("nTx");
				string bloc = i.at("nBloc");
				string nUTXO = i.at("nUTXO");
				string montant = i.at("montant"); 
				utxo.nBloc = stoi(bloc);
				utxo.nTx = stoi(nTx);
				utxo.nUTX0 = stoi(nUTXO);
				utxo.montant = strtof(montant.c_str(), 0); 
				string dest = i.at("cle_publique");
				copy(dest.begin(), dest.end(), utxo.dest);
				utxo.dest[dest.length()] = 0;
				string hash = i.at("hash");
				copy(hash.begin(), hash.end(), utxo.hash);
				utxo.hash[hash.length()] = 0;
				//ajout d'un txo
				b.tx1.UTXOs.push_back(utxo);
			}
			//recuperation tx0
			json txm = it["TXM"];
			b.tx0.utxo[0].nTx = 0; // il n'y a qu'une seule transaction pour le mineur
			string nUTXO = txm.at("nUTXO");
			b.tx0.utxo[0].nUTX0 = stoi(nUTXO);
			string nBloc = txm.at("nBloc");
			b.tx0.utxo[0].nBloc = stoi(nBloc);
			string montant = txm.at("montant");
			b.tx0.utxo[0].montant = strtof(montant.c_str(), 0);
			string cle = txm.at("cle_publique");
			cout << "test3" << endl;
			copy(cle.begin(), cle.end(), b.tx0.utxo[0].dest);
			b.tx0.utxo[0].dest[cle.length()] = 0;
			string hash = txm.at("hash");
			copy(hash.begin(), hash.end(), b.tx0.utxo[0].hash);
			b.tx0.utxo[0].hash[hash.length()] = 0;

			//recuperation hash et hash du précedent
			string hashBloc = it["hash"];
			string hashBlocPrecedent = it["previous_hash"];
			strncpy_s(b.hash, HASH_SIZE, hashBloc.c_str(), HASH_SIZE);
			strncpy_s(b.previous_hash, HASH_SIZE, hashBlocPrecedent.c_str(), HASH_SIZE);
			//numeroBloc
			string numbloc = it["num"];
			b.num = stoi(numbloc);
			string nonce = it["nonce"];
			b.nonce = stoi(nonce);
			//on ajoute le bloc a liste_blocs 
			liste_blocs.push_back(b);
		}
	}
	catch (json::parse_error& e) {
		// output exception information
		cout << "message: " << e.what() << '\n'
			<< "exception id: " << e.id << '\n'
			<< "byte position of error: " << e.byte << endl;
	}
	return liste_blocs;
}

Bloc FileInterface::findByHash(string hash) {
	vector<Bloc> ensembleBlocsBlockchain = readAll();//Vecteur de tous les blocs de la blockchain 
	int i;	//Variable d'incrémentation
	for (vector<Bloc>::iterator i = ensembleBlocsBlockchain.begin(); i != ensembleBlocsBlockchain.end(); i++)	{
		Bloc bloc = *i;
		if (bloc.hash == hash){
			cout << "Bloc :" << bloc.num << endl;	//Renvoyer le bloc en question à partir de la liste de tous les block
			return bloc;
		}
	}
	throw new string(" Erreur : Bloc not found");
}

Bloc FileInterface::findByIndex(int index) {
	vector<Bloc> ensembleBlocsBlockchain = readAll();		//Vecteur de tous les blocs de la blockchain 
	int i;//Variable d'incrémentation
	for (vector<Bloc>::iterator i = ensembleBlocsBlockchain.begin(); i != ensembleBlocsBlockchain.end(); i++) {
		Bloc bloc = *i;
		if (bloc.num == index) {
			cout << "Bloc :" << bloc.num << endl;		//Renvoyer le bloc en question à partir de la liste de tous les block
			return bloc;
		}
	}
	throw new string(" Erreur : Bloc not found");
}
