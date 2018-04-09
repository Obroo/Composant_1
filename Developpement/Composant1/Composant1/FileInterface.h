#pragma once
#ifndef COMPOSANT1_H
#define COMPOSANT1_H

#include <string>

#ifdef FILEINTERFACE_EXPORTS
#define FILEINTERFACE_INTERFACE __declspec(dllexport)
#else
#define FILEINTERFACE_INTERFACE __declspec(dllimport)
#endif

#include "json.hpp"
#include "Bloc.h"

using namespace std;
using json = nlohmann::json;

class FILEINTERFACE_INTERFACE FileInterface {
private:
	string fichier;
public:
	// Constructeur : initialisation la classe et lit le fichier json passé en paramètre
	FileInterface(string fichier);

	// Insert le bloc à la suite du fichier - génére une exception si le hash du bloc précédent n'est pas bon
	void insert(Bloc bloc);

	// Recherche un bloc en fonction de son hash - génére une exception si aucun bloc n'est trouvé
	Bloc findByHash(string hash);

	// Recherche un bloc en fonction de sa position dans la chaine - génére une exception si aucun bloc n'est trouvé
	Bloc findByIndex(int index);

	// Parcourt tous les blocs afin de savoir s'ils sont conformes - génére une exception si un n'est pas conforme
	void verification();

	// Retourne tous les blocs constituant la blockchain sous forme de vecteur
	vector<Bloc> readAll();

	// Retourne un bloc sous forme de chaine de caractère. Si hash == true, renvoie la chaine de caracère avec le hash et le nonce, sinon sans le hash et le nonce

	string toString(bool allInfos, string hash);
	string toString(bool allInfos,  int index);
};

#endif
