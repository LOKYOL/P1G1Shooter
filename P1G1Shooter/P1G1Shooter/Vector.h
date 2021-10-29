// � Tristan, R�mi et Cl�ment
// Tous droits r�serv�
// En utilisant ce code, vous nous autorisez � :
//		Prendre vos donn�es.
//		Enregistrer vos donn�es.
//		Vendre vos donn�es.
//		Utiliser vos logiciels
//		Partager publiquement vos acc�s bancaires et vos mots de passes
//
// Si vous �tes un �tudiant chez Objectif3D, vous �tes l�galement oblig� de faire notre travail.

#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <crtdbg.h>

#define ADDED_CAPACITY 10

#define Vector_Deallocate(vector) \
	Vd_Deallocate(&vector)

#define Vector_Resize(vector) \
	Vd_Resize(&vector)

#define Vector_Clear(vector) \
	Vd_Clear(&vector)

#define Vector_Insert(vector, value, index) \
	Vd_Insert(&vector, &value, index)

#define Vector_Erase(vector, index) \
	Vd_Erase(&vector, index)

#define Vector_Push_back(vector, value) \
	Vd_Push_Back(&vector, &value)

#define Vector_Back(vector, type) \
	*(type *)Vd_Back(vector)

#define Vector_Data(vector, index, type) \
	*(type *)Vd_Data(vector, index)

#define Vector_Pop_back(vector, type) \
	*(type *)Vd_Pop_Back(vector)

#define Vector_Create_Vector(type) \
	Vd_InitVectorD(sizeof(type))
 
///<summary>
///	Un vecteur dynamique continent un double pointeur de type void qui fait r�f�rence � l'�l�ment en t�te.
/// La capacit� correspond � la taille en octets du tableau.
/// La longueur du tableau dynamique
/// La taille d'un �l�ment du tableau
///</summary>
typedef struct VectorD {
	void* _array;
	int _capacity;
	int _length;
	int _itemSize;
	
}VectorD;

/// <summary>
/// Initialise un vecteur
/// </summary>
/// <param name="typeSize">Taille du type des donn�es stock�es</param>
/// <returns></returns>
VectorD Vd_InitVectorD(int typeSize);

/// <summary>
/// R�cup�re le dernier �l�ment d'un tableau
/// </summary>
/// <param name="vector">Vecteur contenant le tableau</param>
/// <returns>Dernier �l�ment du tableau</returns>
void* Vd_Back(VectorD vector);

/// <summary>
/// Ajoute un �l�ment en fin de tableau
/// </summary>
/// <param name="vector">Vecteur contenant le tableau</param>
/// <param name="element">L'�l�ment � rajouter � la fin du tableau</param>
void Vd_Push_Back(VectorD* vector, void* element);

/// <summary>
/// R�cup�re et supprime le dernier �l�ment d'un tableau
/// </summary>
/// <param name="vector">Vecteur contenant le tableau</param>
void* Vd_Pop_Back(VectorD* vector);

/// <summary>
/// Obtiens l'�l�ment du tableau � l'index donn�
/// </summary>
/// <param name="vector">Vecteur contenant le tableau</param>
/// <param name="index">L'index � laquele on souhaite r�cup�rer un pointeur</param>
/// <returns>Retourne un pointeur de l'�lement qui se trouve � l'index pass� en param�tre, sinon retuourne NULL</returns>
void* Vd_Data(VectorD vector, int index);

/// <summary>
/// Insert un �l�ment � un index donn� du tableau
/// </summary>
/// <param name="vector">Vecteur contenant le tableau</param>
/// <param name="element">�l�ment qu'on souhaite ajouter</param>
/// <param name="index">L'index o� l'on souhaite l'ajouter</param>
void Vd_Insert(VectorD* vector, void* element, int index);

/// <summary>
/// Supprime un �l�ment � un index donn� du tableau
/// </summary>
/// <param name="vector">Vecteur contenant le tableau</param>
/// <param name="index">L'index de l'�l�ment recherch�</param>
void Vd_Erase(VectorD* vector, int index);

/// <summary>
/// R�cup�re la taille allou�e d'un tableau dynamique
/// </summary>
/// <param name="vector">Vecteur contenant le tableau</param>
/// <returns>Retourne la taille allou�e au tableau</returns>
int Vector_Capacity(VectorD vector);

/// <summary>
/// R�cup�re la taille actuelle d'un tableau dynamique
/// </summary>
/// <param name="vector">Vecteur contenant le tableau</param>
/// <returns>Retourne le nombre d'�l�ments dans le tableau</returns>
int Vector_Length(VectorD vector);

/// <summary>
/// Efface un tableau
/// </summary>
/// <param name="vector">Vecteur contenant le tableau</param>
void Vd_Clear(VectorD* vector);

/// <summary>
/// Retaille un tableau pour prendre le plus petit espace m�moire possible en fonction du nombre d'�l�ment que le tableau contient
/// </summary>
/// <param name="vector">Vecteur contenant le tableau</param>
void Vd_Resize(VectorD* vector);

/// <summary>
/// R�ajuste la taille allou�e du tableau pour pouvoir contenir plus d'�l�ments
/// </summary>
/// <param name="vector">Vecteur contenant le tableau</param>
void Vd_Reserve(VectorD* vector);

/// <summary>
/// Supprime un vecteur
/// </summary>
/// <param name="vector">Vecteur contenant le tableau</param>
void Vd_Deallocate(VectorD* vector);

/// <summary>
/// V�rifie si un vecteur est �gale � sa capacit� maximale ou la d�passe
/// </summary>
/// <param name="vector">Vecteur contenant le tableau</param>
int Vd_ExceedOrEqualMaxCapacity(VectorD vector);

/// <summary>
/// V�rifie si un vecteur d�passe sa capacit� minimale
/// </summary>
/// <param name="vector">Vecteur contenant le tableau</param>
int Vd_ExceedMinCapacity(VectorD vector);
