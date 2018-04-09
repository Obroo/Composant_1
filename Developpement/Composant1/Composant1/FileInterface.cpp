#include<string>
#include <iostream>
#include <fstream>
#include <stdio.h>

#include "FileInterface.h"
#include "Composant5.h"

using namespace std;

// Constructeur : initialisation la classe et lit le fichier json passé en paramètre
FileInterface::FileInterface(string file_path) {
	fichier = file_path;
	ifstream ifs(fichier);
	if (ifs.fail()) {
		cout << "la lecture du fichier : " << file_path <<" a echoue" << endl;
		throw new string("la lecture du fichier a echoue");
	}
	else {
		cout << "lecture du fichier : " << fichier << " a reussi." << endl;
	}
}

void FileInterface::insert(Bloc bloc) {
	CComposant5 c5;
	bool result = c5.verify_bloc(bloc, bloc.hash, bloc.nonce);
	if (result == false) {
		throw std::invalid_argument("The bloc did not respect the specifications.");
	}
	ifstream ifs(fichier); // lit le fichier
	json j = json::parse(ifs); // transforme en json
	auto jsonObjects = json::array();
	for (const auto& it : j) {
		jsonObjects.push_back(it);
	}
	string s = toString(true, bloc.hash);
	json js;
	js["hash"] = bloc.hash;
	js["nonce"] = to_string(bloc.nonce);
	js["previous_hash"] = string(bloc.previous_hash);
	js["num"] = to_string(bloc.num);
	js["TX"]["TXI"] = json::array();
	for (unsigned int i = 0; i < bloc.tx1.TXIs.size(); i++) {
		js["TX"]["TXI"][i]["nBloc"] = to_string(bloc.tx1.TXIs[i].nBloc);
		js["TX"]["TXI"][i]["nUTXO"] = to_string(bloc.tx1.TXIs[i].nUtxo);
		js["TX"]["TXI"][i]["nTx"] = to_string(bloc.tx1.TXIs[i].nTx);
		js["TX"]["TXI"][i]["signature"] = string(bloc.tx1.TXIs[i].signature, bloc.tx1.TXIs[i].signature + sizeof bloc.tx1.TXIs[i].signature / sizeof bloc.tx1.TXIs[i].signature[0]);
	}
	js["TX"]["UTXO"] = json::array();
	for (unsigned int i = 0; i < bloc.tx1.UTXOs.size(); i++) {
		js["TX"]["UTXO"][i]["nBloc"] = to_string(bloc.tx1.UTXOs[i].nBloc);
		js["TX"]["UTXO"][i]["nUTXO"] = to_string(bloc.tx1.UTXOs[i].nUTX0);
		js["TX"]["UTXO"][i]["nTx"] = to_string(bloc.tx1.TXIs[i].nTx);
		js["TX"]["UTXO"][i]["montant"] = to_string(bloc.tx1.UTXOs[i].montant);
		js["TX"]["UTXO"][i]["cle_publique"] = string(bloc.tx1.UTXOs[i].dest, bloc.tx1.UTXOs[i].dest + sizeof bloc.tx1.UTXOs[i].dest / sizeof bloc.tx1.UTXOs[i].dest[0]);
		js["TX"]["UTXO"][i]["hash"] = string(bloc.tx1.UTXOs[i].hash, bloc.tx1.UTXOs[i].hash + sizeof bloc.tx1.UTXOs[i].hash / sizeof bloc.tx1.UTXOs[i].hash[0]);
	} 
	js["TXM"]["nBloc"] = to_string(bloc.tx0.utxo[0].nBloc);
	js["TXM"]["nUTXO"] = to_string(bloc.tx0.utxo[0].nUTX0);
	js["TXM"]["montant"] = to_string(bloc.tx0.utxo[0].montant);
	js["TXM"]["cle_publique"] = string(bloc.tx0.utxo[0].dest, bloc.tx0.utxo[0].dest + sizeof bloc.tx0.utxo[0].dest / sizeof bloc.tx0.utxo[0].dest[0]);
	js["TXM"]["nBloc"] = string(bloc.tx0.utxo[0].hash, bloc.tx0.utxo[0].hash + sizeof bloc.tx0.utxo[0].hash / bloc.tx0.utxo[0].hash[0]);

	jsonObjects.emplace_back(js);
	std::ofstream o("output.json");
	o << std::setw(4) << jsonObjects << std::endl;
}

string FileInterface::toString(bool allInfos, string hash) {
	Bloc bloc = findByHash(hash);
	return bloc.ToString();
}

string Bloc::ToString() {
	string str = "{";
	str += "\n\t\"hash\": \"" + string(hash);
	str += "\",\n\t\"nonce\": \"" + to_string(nonce) + "\",";
	str += "\n\t\"previous_hash\": \"" + string(previous_hash);
	str += "\",\n\t\"num\": \"" + to_string(num);
	str += "\",\n\t\"TX\": { \n\t\t";
	str += "\"TXI\": [{";

	for (unsigned int i = 0; i < tx1.TXIs.size(); i++) {
		str += "\"nBloc\": \"" + to_string(tx1.TXIs[i].nBloc);
		str += "\",\"nUTXO\": \"" + to_string(tx1.TXIs[i].nUtxo);
		str += "\",\"nTx\": \"" + to_string(tx1.TXIs[i].nTx);
		str += "\",\"signature\": \"" + string(tx1.TXIs[i].signature, tx1.TXIs[i].signature + sizeof tx1.TXIs[i].signature / sizeof tx1.TXIs[i].signature[0]);
		str += "\"}";
		if (i != (tx1.TXIs.size() - 1)) {
			str += ",";
		}
	}
	str += "],\n";
	str += "\"UTXO\": [{";
	for (unsigned int i = 0; i < tx1.UTXOs.size(); i++) {
		str += "\"nBloc\": \"" + to_string(tx1.UTXOs[i].nBloc);
		str += "\",\"nUTXO\": \"" + to_string(tx1.UTXOs[i].nUTX0);
		str += "\",\"nTx\": \"" + to_string(tx1.UTXOs[i].nTx);
		str += "\",\"montant\": \"" + to_string(tx1.UTXOs[i].montant);
		str += "\",\"cle_publique\": \"" + string(tx1.UTXOs[i].dest, tx1.UTXOs[i].dest + sizeof tx1.UTXOs[i].dest / sizeof tx1.UTXOs[i].dest[0]);
		str += "\",\"hash\": \"" + string(tx1.UTXOs[i].hash, tx1.UTXOs[i].hash + sizeof tx1.UTXOs[i].hash / sizeof tx1.UTXOs[i].hash[0]);
		str += "\"}";
		if (i != (tx1.UTXOs.size() - 1)) {
			str += ",";
		}
	}

	str += "]\n},";
	str += "\",\n\t\"TXM\": {";
	str += "\n\t\t\"nBloc\": \"" + to_string(tx0.utxo[0].nBloc);
	str += "\",\n\t\"nUTXO\": \"" + to_string(tx0.utxo[0].nUTX0) + "\",";
	str += "\",\n\t\"montant\": \"" + to_string(tx0.utxo[0].montant) + "\",";
	str += "\",\n\t\"cle_publique\": \"" + string(tx0.utxo[0].dest, tx0.utxo[0].dest + sizeof tx0.utxo[0].dest / sizeof tx0.utxo[0].dest[0]) + "\",";
	str += "\",\n\t\"hash\": \"" + string(tx0.utxo[0].hash, tx0.utxo[0].hash + sizeof tx0.utxo[0].hash / sizeof tx0.utxo[0].hash[0]) + "\"\n\t}\n}";
	return str;
}

string FileInterface::toString(bool allInfos, int index) {
	Bloc b = findByIndex(index);
	string str ="{";
	if (allInfos == true) {
		str += "\n\t\"hash\": \"" + string(b.hash);
		str += "\",\n\t\"nonce\": \"" + to_string(b.nonce) + "\",";
	}
	str += "\n\t\"previous_hash\": \"" + string(b.previous_hash);
	str += "\",\n\t\"num\": \"" + to_string(b.num);
	str += "\",\n\t\"TX\": { \n\t\t";
	str += "\"TXI\": [{";

	for (unsigned int i = 0; i < b.tx1.TXIs.size(); i++) {
		str += "\"nBloc\": \"" + to_string(b.tx1.TXIs[i].nBloc);
		str += "\",\"nUTXO\": \"" + to_string(b.tx1.TXIs[i].nUtxo);
		str += "\",\"nTx\": \"" + to_string(b.tx1.TXIs[i].nTx);
		str += "\",\"signature\": \"" + string(b.tx1.TXIs[i].signature, b.tx1.TXIs[i].signature + sizeof b.tx1.TXIs[i].signature / sizeof b.tx1.TXIs[i].signature[0]);
		str += "\"}";
		if (i != (b.tx1.TXIs.size() - 1)) {
			str += ",";
		}
	}
	str += "],\n";
	str += "\"UTXO\": [{";
	for (unsigned int i = 0; i < b.tx1.UTXOs.size(); i++) {
		str += "\"nBloc\": \"" + to_string(b.tx1.UTXOs[i].nBloc);
		str += "\",\"nUTXO\": \"" + to_string(b.tx1.UTXOs[i].nUTX0);
		str += "\",\"nTx\": \"" + to_string(b.tx1.UTXOs[i].nTx);
		str += "\",\"montant\": \"" + to_string(b.tx1.UTXOs[i].montant);
		str += "\",\"cle_publique\": \"" + string(b.tx1.UTXOs[i].dest, b.tx1.UTXOs[i].dest + sizeof b.tx1.UTXOs[i].dest / sizeof b.tx1.UTXOs[i].dest[0]);
		str += "\",\"hash\": \"" + string(b.tx1.UTXOs[i].hash, b.tx1.UTXOs[i].hash + sizeof b.tx1.UTXOs[i].hash / sizeof b.tx1.UTXOs[i].hash[0]);
		str += "\"}";
		if (i != (b.tx1.UTXOs.size() - 1)) {
			str += ",";
		}
	}

	str += "]\n},";
	str += "\",\n\t\"TXM\": {";
	str += "\n\t\t\"nBloc\": \"" + to_string(b.tx0.utxo[0].nBloc);
	str += "\",\n\t\"nUTXO\": \"" + to_string(b.tx0.utxo[0].nUTX0) + "\",";
	str += "\",\n\t\"montant\": \"" + to_string(b.tx0.utxo[0].montant) + "\",";
	str += "\",\n\t\"cle_publique\": \"" + string(b.tx0.utxo[0].dest, b.tx0.utxo[0].dest + sizeof b.tx0.utxo[0].dest / sizeof b.tx0.utxo[0].dest[0]) + "\",";
	str += "\",\n\t\"hash\": \"" + string(b.tx0.utxo[0].hash, b.tx0.utxo[0].hash + sizeof b.tx0.utxo[0].hash / b.tx0.utxo[0].hash[0]) + "\"\n\t}\n}";

	return str;
}


// Parcours tous les blocs afin de savoir s'ils sont conformes - génére une exception si un n'est pas conforme
void FileInterface::verification() {
	//Composant5 c5;
	vector<Bloc> liste_blocs = readAll(); // transforme le fichier en un vecteur de Bloc
	for (vector<Bloc>::iterator it = liste_blocs.begin(); it != liste_blocs.end(); ++it) {
		Bloc bloc = *it;
		cout << " verification du Bloc : " << bloc.num << endl;
		// on parcours la liste et on appelle la methode verification bloc du composant5
		/*if (!c5.verify_bloc(bloc, bloc.hash, bloc.nonce)) { // tester car methode verify_bloc attend un Bloc*
			throw new string("erreur lors de la verification d'un bloc");
		}*/
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
			//recuperation transactions
			json transactions = it["TX"];
			UTXO utxo;
			TXI txi;
			//recuperation txi et utxo
			for (const auto& i : transactions.at("TXI")) {
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
			copy(cle.begin(), cle.end(), b.tx0.utxo[0].dest);
			b.tx0.utxo[0].dest[cle.length()] = 0;
			string hash = txm.at("hash");
			copy(hash.begin(), hash.end(), b.tx0.utxo[0].hash);
			b.tx0.utxo[0].hash[hash.length()] = 0;
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
	for (vector<Bloc>::iterator i = ensembleBlocsBlockchain.begin(); i != ensembleBlocsBlockchain.end(); i++)	{
		Bloc bloc = *i;
		if (bloc.hash == hash){
			cout << "FindByHash : Bloc " << bloc.num << " found." << endl;	//Renvoyer le bloc en question à partir de la liste de tous les block
			return bloc;
		}
	}
	throw new string(" Erreur : Bloc not found");
}

Bloc FileInterface::findByIndex(int index) {
	vector<Bloc> ensembleBlocsBlockchain = readAll();		//Vecteur de tous les blocs de la blockchain 
	for (vector<Bloc>::iterator i = ensembleBlocsBlockchain.begin(); i != ensembleBlocsBlockchain.end(); i++) {
		Bloc bloc = *i;
		if (bloc.num == index) {
			cout << "FindByIndex : Bloc " << bloc.num <<" found."<< endl;		//Renvoyer le bloc en question à partir de la liste de tous les block
			return bloc;
		}
	}
	throw new string(" Erreur : Bloc not found");
}