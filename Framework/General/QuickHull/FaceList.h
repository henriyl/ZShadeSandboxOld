//===============================================================================================================================
// FaceList
//
//===============================================================================================================================
// History
//
// -Created on 4/6/2015 by Dustin Watson
//===============================================================================================================================
#ifndef __FACELIST_H
#define __FACELIST_H
//===============================================================================================================================
//===============================================================================================================================
#include "HullFace.h"
//===============================================================================================================================
// Single-linked list of hull faces
namespace ZShadeSandboxMath {
class FaceList
{
	ZShadeSandboxMath::HullFace* head;
	ZShadeSandboxMath::HullFace* tail;

	ZShadeSandboxMath::HullFace* next;
	ZShadeSandboxMath::HullFace* prev;

public:
	
	FaceList();

	// Clears the list
	void clear();

	// Adds a item to the end of the list
	void add(ZShadeSandboxMath::HullFace* item);

	// Returns the first element in the list
	ZShadeSandboxMath::HullFace* first();

	// Returns true if the list is empty
	bool isEmpty();
};
}
//===============================================================================================================================
//===============================================================================================================================
#endif//__FACELIST_H