#ifndef DICTIONARY_H
#define DICTIONARY_H

#include "Elm.hpp"
#include "types.hpp"

class Dictionary
{
	private:
		Elm invalitElements;
		Elm twoHierarchyElements;
		Elm baseElements;
		Elm initElemets;
		int maxRawDatSize;
		int actualPosRawData;
		int count3element;
		byte* rawData = NULL;	// for storage items	
		byte* mDataStorage = NULL; // for local storage output data
		byte* pData1024B  = NULL;
		
		void hierarchyIndex();
		bool checkIndex( int pos , int &index1 , int &index2 );
		void showElements();
		void change( int index );
		void action();
	public:
		Dictionary( int maxRawDataSize );
		~Dictionary();
		void setInitDat( byte data1024B[] );
		int compress( byte storage[] , int sizeStorage , byte tagAccess );
};

#endif



