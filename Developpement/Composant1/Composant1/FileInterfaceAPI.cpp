#include<string>
#include <iostream>
#include <fstream>
#include <stdio.h>

#include "../../../Interface Fichier/FileInterfaceAPI.h"

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

string FileInterface::toString(bool allInfos, string hash)
{
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

string FileInterface::toString(bool allInfos, int index)
{
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
	vector<Bloc> liste_blocs = readAll(); // transforme le fichier en un vecteur de Bloc
	for (vector<Bloc>::iterator it = liste_blocs.begin(); it != liste_blocs.end(); ++it) {
		Bloc bloc = *it;
		cout << "Bloc : " << bloc.num << endl;
		/*// on parcours la liste et on appelle la methode verification bloc du composant5
		if (!verify_bloc(bloc)) { // tester car methode verify_bloc attend un Bloc*
			throw new string("erreur lors de la verification d'un bloc");
		}*/
	}
	cout << "verification successful." << endl;
}

//Reçoit un file path et retourne un vector contenant tous les blocs
vector<Bloc> FileInterface::readAll() {
	vector<Bloc> liste_blocs;
	ifstream ifs(fichier); // lit le fichier
	json j = json::parse(ifs); // transforme en json
	// parcours le tableau json contenant les blocs
	for (const auto& it : j) {
		Bloc b;
		//recuperation transactions
		vector<json> transactions = it["Transactions"];
		TX transaction;
		UTXO utxo;
		TXI txi;
		int nUTXO = 0; // compte le nombre de utxo 
		int nUTXI = 0; // compte le nombre de txi 
		for (auto i : transactions) {
			// si l'utilisateur est l'emetteur de la transaction (TXO)
			if (i.at("ID_init") == it["Identifiant"]) {
				//recuperation des valeurs json en string
				string bloc = i.at("Bloc");
				string id = i.at("Id");
				string montant = i.at("montant");
				//cast des valeurs en int ou float
				utxo.nBloc = stoi(bloc);
				utxo.nTx = stoi(id);
				utxo.nUTX0 = ++nUTXO;
				utxo.montant = strtof(montant.c_str(), 0);
				// recuperation compte destinataire (clé publique) 
				for (const auto& temp : j) {
					if (temp["Identifiant"] == i.at("ID_recev")) {
						json hashes = it["Hashes"];
						string hash_temp = hashes.at("Hash");
						copy(hash_temp.begin(), hash_temp.end(), utxo.dest);
						utxo.dest[hash_temp.length()] = 0;
					}
				}
				//TO - DO : unsigned char hash[HASH_SIZE]; hash(nBloc,nTx,nUTXO,montant,destinataire) pour securisation de l'UTXO

				transaction.UTXOs.push_back(utxo);

			} // si l'utilisateur est le destinataire de la transaction (TXI)
			else if (i.at("ID_recev") == it["Identifiant"]) {
				cout << i.at("ID_recev") << endl;
				// block number and id of the source Utxo
				string bloc = i.at("Bloc");
				string id = i.at("Id");
				//cast des valeurs en int ou float
				txi.nBloc = stoi(bloc);
				txi.nTx = stoi(id);
				//index of transaction précédente (a verifier)
				txi.nUtxo = ++nUTXI;
				//TO - DO : unsigned char signature[64];  signature de la transaction avec la clé privée

				transaction.TXIs.push_back(txi);
			}
		}
		//ajouter les transactions TXI et UTXO au bloc
		b.tx1 = transaction;

		//recuperation hashes
		json hashes = it["Hashes"];
		cout << hashes.at("Hash") << endl;
		string hash = hashes.at("Hash");
		string hashBlocPrecedent = hashes.at("BlocPrecedent");
		strncpy_s(b.hash, HASH_SIZE, hash.c_str(), HASH_SIZE);
		strncpy_s(b.previous_hash, HASH_SIZE, hashBlocPrecedent.c_str(), HASH_SIZE);
		//numeroBloc
		string numbloc = it["Identifiant"];
		b.num = stoi(numbloc);
		//TO-DO: transactions_coin_base

		//on ajoute le bloc a liste_blocs 
		liste_blocs.push_back(b);
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
