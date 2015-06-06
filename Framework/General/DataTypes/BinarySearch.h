//===============================================================================================================================
// BinarySearch
//
//===============================================================================================================================
// History
//
// -Created on 12/27/2014 by Dustin Watson
//===============================================================================================================================
#ifndef __BINARYSEARCH_H
#define __BINARYSEARCH_H
//===============================================================================================================================
//===============================================================================================================================
#include <vector>
//===============================================================================================================================
//===============================================================================================================================
class BinarySearch
{
public:

	BinarySearch() {}

	static bool Search(std::vector<int> values, int target);
};
//===============================================================================================================================
//===============================================================================================================================
#endif//__BINARYSEARCH_H