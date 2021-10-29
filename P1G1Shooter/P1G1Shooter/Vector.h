// © Tristan, Rémi et Clément
// Tous droits réservé
// En utilisant ce code, vous nous autorisez à :
//		Prendre vos données.
//		Enregistrer vos données.
//		Vendre vos données.
//		Utiliser vos logiciels
//		Partager publiquement vos accès bancaires et vos mots de passes
//
// Si vous êtes un étudiant chez Objectif3D, vous êtes légalement obligé de faire notre travail.

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
///	Un vecteur dynamique continent un double pointeur de type void qui fait référence à l'élément en tête.
/// La capacité correspond à la taille en octets du tableau.
/// La longueur du tableau dynamique
/// La taille d'un élément du tableau
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
/// <param name="typeSize">Taille du type des données stockées</param>
/// <returns></returns>
VectorD Vd_InitVectorD(int typeSize);

/// <summary>
/// Récupère le dernier élément d'un tableau
/// </summary>
/// <param name="vector">Vecteur contenant le tableau</param>
/// <returns>Dernier élément du tableau</returns>
void* Vd_Back(VectorD vector);

/// <summary>
/// Ajoute un élément en fin de tableau
/// </summary>
/// <param name="vector">Vecteur contenant le tableau</param>
/// <param name="element">L'élément à rajouter à la fin du tableau</param>
void Vd_Push_Back(VectorD* vector, void* element);

/// <summary>
/// Récupère et supprime le dernier élément d'un tableau
/// </summary>
/// <param name="vector">Vecteur contenant le tableau</param>
void* Vd_Pop_Back(VectorD* vector);

/// <summary>
/// Obtiens l'élément du tableau à l'index donné
/// </summary>
/// <param name="vector">Vecteur contenant le tableau</param>
/// <param name="index">L'index à laquele on souhaite récupérer un pointeur</param>
/// <returns>Retourne un pointeur de l'élement qui se trouve à l'index passé en paramètre, sinon retuourne NULL</returns>
void* Vd_Data(VectorD vector, int index);

/// <summary>
/// Insert un élément à un index donné du tableau
/// </summary>
/// <param name="vector">Vecteur contenant le tableau</param>
/// <param name="element">Élément qu'on souhaite ajouter</param>
/// <param name="index">L'index où l'on souhaite l'ajouter</param>
void Vd_Insert(VectorD* vector, void* element, int index);

/// <summary>
/// Supprime un élément à un index donné du tableau
/// </summary>
/// <param name="vector">Vecteur contenant le tableau</param>
/// <param name="index">L'index de l'élément recherché</param>
void Vd_Erase(VectorD* vector, int index);

/// <summary>
/// Récupère la taille allouée d'un tableau dynamique
/// </summary>
/// <param name="vector">Vecteur contenant le tableau</param>
/// <returns>Retourne la taille allouée au tableau</returns>
int Vector_Capacity(VectorD vector);

/// <summary>
/// Récupère la taille actuelle d'un tableau dynamique
/// </summary>
/// <param name="vector">Vecteur contenant le tableau</param>
/// <returns>Retourne le nombre d'éléments dans le tableau</returns>
int Vector_Length(VectorD vector);

/// <summary>
/// Efface un tableau
/// </summary>
/// <param name="vector">Vecteur contenant le tableau</param>
void Vd_Clear(VectorD* vector);

/// <summary>
/// Retaille un tableau pour prendre le plus petit espace mémoire possible en fonction du nombre d'élément que le tableau contient
/// </summary>
/// <param name="vector">Vecteur contenant le tableau</param>
void Vd_Resize(VectorD* vector);

/// <summary>
/// Réajuste la taille allouée du tableau pour pouvoir contenir plus d'éléments
/// </summary>
/// <param name="vector">Vecteur contenant le tableau</param>
void Vd_Reserve(VectorD* vector);

/// <summary>
/// Supprime un vecteur
/// </summary>
/// <param name="vector">Vecteur contenant le tableau</param>
void Vd_Deallocate(VectorD* vector);

/// <summary>
/// Vérifie si un vecteur est égale à sa capacité maximale ou la dépasse
/// </summary>
/// <param name="vector">Vecteur contenant le tableau</param>
int Vd_ExceedOrEqualMaxCapacity(VectorD vector);

/// <summary>
/// Vérifie si un vecteur dépasse sa capacité minimale
/// </summary>
/// <param name="vector">Vecteur contenant le tableau</param>
int Vd_ExceedMinCapacity(VectorD vector);
