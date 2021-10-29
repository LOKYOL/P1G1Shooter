#include "Vector.h"
#include <string.h>

VectorD Vd_InitVectorD(int typeSize)
 {
	VectorD vector;
	vector._capacity = typeSize * ADDED_CAPACITY;
	vector._length = 0;
	vector._itemSize = typeSize;
	vector._array = (char*) malloc(sizeof(char) * (typeSize * ADDED_CAPACITY));

	return vector;
}

void* Vd_Back(VectorD vector)
{
	return (char*)vector._array + vector._itemSize * (vector._length - 1);
}

void Vd_Push_Back(VectorD* vector, void* element)
{
	if (vector->_length * vector->_itemSize >= vector->_capacity)
		Vd_Reserve(vector);

	//double* test = (double*)element;
	void* adr = (char*)vector->_array + vector->_itemSize * vector->_length;
	memcpy(adr, element, vector->_itemSize);
	vector->_length++;
}

void* Vd_Pop_Back(VectorD* vector)
{
	void* memory = Vd_Back(*vector);
	vector->_length--;
	return memory;
}

void* Vd_Data(VectorD vector, int index)  // Updater avec cas NULL
{
	if (index >= 0 && index < vector._length)
	{
		return (char*)vector._array + index * vector._itemSize;
	}

	printf("ERREUR: Index %d hors-tableau\n", index);
	fprintf(stderr, "Index out-of-bound, exiting...\n");
	exit(EXIT_FAILURE);
}

void Vd_Insert(VectorD* vector, void* element, int index)
{
	if (index < 0 || index >= vector->_length)
		return;

	if (Vd_ExceedOrEqualMaxCapacity(*vector))
		Vd_Reserve(vector);

	int i = vector->_length;
	int difference = vector->_length - index;

	memmove((char*)vector->_array + vector->_itemSize * (index + 1), (char*)vector->_array + vector->_itemSize * index, vector->_itemSize * difference);

	//while(i > index){
	//	memcpy((char*)vector->_array + i * vector->_itemSize, (char*)vector->_array + (i - 1) * vector->_itemSize, vector->_itemSize);
	//	i--;
	//}

	void* adr = (char*)vector->_array + vector->_itemSize * index;
	memcpy(adr, element, vector->_itemSize);
	vector->_length++;
}

void Vd_Erase(VectorD* vector, int index)
{
	if (index < 0 || index >= vector->_length)
		return;

	int i = index;

	int difference = vector->_length - index;
	memmove((char*)vector->_array + vector->_itemSize * index, (char*)vector->_array + vector->_itemSize * (index + 1), vector->_itemSize * difference);

	//while(i < vector->_length - 1){	
	//	memcpy((char*)vector->_array + i * vector->_itemSize, (char*)vector->_array + (i + 1) * vector->_itemSize, vector->_itemSize);
	//	i++;
	//}

	vector->_length--;

	if(Vd_ExceedMinCapacity(*vector))
		Vd_Resize(&vector);
}

int Vector_Capacity(VectorD vector)
{
	return vector._capacity;
}

int Vector_Length(VectorD vector)
{
	return vector._length;
}

void Vd_Clear(VectorD* vector)
{
	vector->_length = 0;
}

void Vd_Resize(VectorD* vector)
{
	vector->_capacity = vector->_length * vector->_itemSize;
	void* temp = (void*)realloc(vector->_array, vector->_length * vector->_itemSize);
	vector->_array = temp;
}

void Vd_Reserve(VectorD* vector)
{
	vector->_capacity = (vector->_length + ADDED_CAPACITY) * vector->_itemSize;
	void* temp = (void*)realloc(vector->_array, vector->_capacity);
	vector->_array = temp;
}

void Vd_Deallocate(VectorD* vector)
{
	free(vector->_array);
}

int Vd_ExceedOrEqualMaxCapacity(VectorD vector){
	return vector._length * vector._itemSize >= vector._capacity ? 1 : 0;
}

int Vd_ExceedMinCapacity(VectorD vector){
	return vector._length * vector._itemSize < vector._capacity - (ADDED_CAPACITY * vector._itemSize) ? 1 : 0;
}
