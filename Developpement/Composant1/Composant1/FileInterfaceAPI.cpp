#include<string>
#include <iostream>
#include <fstream>
#include <stdio.h>

#include "FileInterfaceAPI.h"

using namespace std;

// Constructeur : initialisation la classe et lit le fichier json pass� en param�tre
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

// Parcourt tous les blocs afin de savoir s'ils sont conformes - g�n�re une exception si un n'est pas conforme
void FileInterface::verification() {
	vector<Bloc> liste_blocs = parse(fichier); // transforme le fichier en un vecteur de Bloc
	for (vector<Bloc>::iterator it = liste_blocs.begin(); it != liste_blocs.end(); ++it) {
		Bloc bloc = *it;
		cout << "Bloc : " << bloc.num << endl;
		/*	// on parcours la liste et on appelle la methode verification bloc du composant5
			if (!verify_bloc(bloc)) { // tester car methode verify_bloc attend un Bloc*
				throw new string("erreur lors de la verification d'un bloc");
			}*/
	}
	cout << "verification successful." << endl;
}

vector<Bloc> FileInterface::parse(string blocsJson) {
	vector<Bloc> liste_blocs;
	ifstream ifs(blocsJson); // lit le fichier
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
				// recuperation compte destinataire (cl� publique) 
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
				//index of transaction pr�c�dente (a verifier)
				txi.nUtxo = ++nUTXI;
				//TO - DO : unsigned char signature[64];  signature de la transaction avec la cl� priv�e

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
