// Composant5.cpp : Defines the exported functions for the DLL application
#include <iostream>
#include "Composant5.h"

// This is the constructor of a class that has been exported.
// see Composant5.h for the class definition
CComposant5::CComposant5()
{
	std::cout << "constructeur" << std::endl;
}
//méthode permettant de la vérification d'un bloc.
//elle retourne true si le bloc répond aux caractéristique définies de false dans le cas contraire.
bool CComposant5::verify_bloc(Bloc b, char hash[HASH_SIZE], int nonce) {
	return true;
}

//méthode permettant de la vérification d'une transaction.
//elle retourne true si la transactio répond aux caractéristique définies de false dans le cas contraire.
bool CComposant5::verify_transaction(TXI tx) {
	return true;
}

