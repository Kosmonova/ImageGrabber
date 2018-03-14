#include <iostream>
#include "Dictionary.hpp"

//#define SHOW_OUT

using namespace std;

Dictionary::Dictionary( int maxRawDataSize )
{
	count3element = 0;
	actualPosRawData = 0;
	maxRawDatSize = maxRawDataSize;
	rawData = new byte[ maxRawDatSize ];
	if( rawData == NULL )
		cout << "err allocate raw data dictionary" << endl;
}

Dictionary::~Dictionary()
{
	delete[] rawData;
	delete[] mDataStorage;
}

void Dictionary::change( int index )
{
	word Item = ((word*)pData1024B)[ index ];
	byte &lItem = ( ( byte* )&Item )[ 0 ];
	((word*)pData1024B)[ index ] = index / 2;

	byte lItemPrev = lItem;

	Item = ((word*)pData1024B)[ index + 1 ];

	((word*)pData1024B)[ index + 1 ] = 0;

	mDataStorage[ actualPosRawData++ ] = lItem;
	mDataStorage[ actualPosRawData++ ] = lItemPrev;
	mDataStorage[ actualPosRawData++ ] = index / 2;
	
	count3element++;
}

void Dictionary::showElements()
{
	cout << "invalid elm: " << endl;
	invalitElements.showElements();
	
	cout << "two hierarchy elm: " << endl;
	twoHierarchyElements.showElements();

	cout << "initElemets elm: " << endl;
	initElemets.showElements();
	
	cout << "baseElements elm: " << endl;
	baseElements.showElements();
}



bool Dictionary::checkIndex( int pos , int &index1 , int &index2 )
{
	word Var1x, Var2x;
	byte &Var1l = ( ( byte* )&Var1x )[ 0 ];
	byte &Var2l = ( ( byte* )&Var2x )[ 0 ];
	byte &Var1h = ( ( byte* )&Var1x )[ 1 ];
	byte &Var2h = ( ( byte* )&Var2x )[ 1 ];
	
	Var1x = ((word*)pData1024B)[ pos * 2 ];
	Var2x = ((word*)pData1024B)[ pos * 2 + 1 ];
	
	if(  ( Var1h == 1 ) ||  ( Var1h == 2 ) )
	{
		byte element1 = pData1024B[ Var1l * 4 + 1 ];
		
		if( ( ( Var1h == 1 ) && ( element1 == 0 ) ) || ( ( Var1h == 2 ) && ( element1 != 0 ) ) ) 
		{
			index1 = Var1l;
		}
		else
		{
#ifdef SHOW_OUT
			cout << "index: " << hex << pos << " var1h: " << hex << (int)Var1h;
			cout << " var1l: " << hex << (int)Var1l << " element1: " << hex << (int)element1 << endl;
#endif
			index1 = 0xFFF + ( Var1h * 0x100 + Var1l ) * 0x1000;
		}
		
		byte element2 = pData1024B[ Var2l * 4 + 1 ];
		
		if( ( ( Var2h == 1 ) && ( element2 == 0 ) ) || ( ( Var2h == 2 ) && ( element2 != 0 ) ) ) 
		{
			index2 = Var2l;
		}
		else
		{
#ifdef SHOW_OUT
			cout << "index: " << hex << pos << " var2h: " << hex << (int)Var2h;
			cout << " var2l: " << hex << (int)Var2l << " element2: " << hex << (int)element2 << endl;
#endif
			index2 = 0xFFF + ( Var2h * 0x100 + Var2l ) * 0x1000;
		}
		
		return true;
	}
	else
	{
		return false;
	}
}

void Dictionary::hierarchyIndex()
{
	for( int index = 0 ; index < 256 ; index++ )
	{
		int index1 , index2;
		bool status = checkIndex( index , index1 , index2 );
		
		if( !status )
		{
			initElemets.add( index );
		}
		else
		{
#ifdef SHOW_OUT
			cout << "index: " << hex << index;
			cout << "  index1: " << hex << index1 << "  index2: " << hex << index2 << endl;
#endif
			
			if( ( index1 > 0x500 ) || ( index2 > 0x500 ) )
				invalitElements.add( index );
			
			index1 &= 0xFF;
			if( pData1024B[ 4 * index1  + 1 ] != 0 )
				twoHierarchyElements.add( index1 );
				
			index2 &= 0xFF;
			if( pData1024B[ 4 * index2 + 1 ] != 0 )
				twoHierarchyElements.add( index2 );	
		}
	}

	int dest;
	int index = 0;
	while( invalitElements.get( index++ , dest ) )
		twoHierarchyElements.remove( dest );
	
		
	for( int pos = 0 ; pos < 256 ; pos++ )
		if( !twoHierarchyElements.isInner( pos ) && ( !initElemets.isInner( pos )) && ( !invalitElements.isInner( pos )) )
			baseElements.add( pos );
}

void Dictionary::action()
{	
	while( initElemets.size() < 256 )
	{
		invalitElements.clear();
		twoHierarchyElements.clear();
		baseElements.clear();
		initElemets.clear();
		hierarchyIndex();
		int index;
		
#ifdef SHOW_OUT
		showElements();	
#endif
		
		if( initElemets.size() != 0x100 && baseElements.size() == 0 )
		{
			cout << "err not possible code dictionary" << endl;
			break;
		}
	
		while( baseElements.getElment( index ) )
			change( index * 2 );	
	}
}

void Dictionary::setInitDat( byte data1024B[] )
{
	pData1024B = data1024B;
}

int Dictionary::compress( byte storage[] , int sizeStorage , byte tagAccess )
{
	mDataStorage = new byte[ sizeStorage ];
	if( mDataStorage == NULL )
	{
		cout << "err alloc data storage local" << endl;
		return -1;
	}
	
	actualPosRawData = 0;
	count3element = 0;	
	action();

	mDataStorage[ actualPosRawData++ ] = tagAccess;//0x0E;
	mDataStorage[ actualPosRawData++ ] = count3element;
	
	int compressDataSize = count3element * 3 + 2;
	for( int pos = 0 ; pos < compressDataSize ; pos++ )
		storage[ pos ] = mDataStorage[ compressDataSize - pos - 1 ]; 
	
	return compressDataSize;
}






