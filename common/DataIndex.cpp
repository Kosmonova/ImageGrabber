#include <iostream>
#include <cstring>
#include <iomanip> 

#include "DataIndex.hpp" 
 
//#define SHOW_INFO
#include <fstream>
 
using namespace std;
 
int DataIndex::BestItem::countActItem = 0;
 
DataIndex::DataIndex( byte data[] , int size ) 
{
	cpData = new byte[ size ];
	sizeData = size;
	memcpy( cpData , data , size );
	
	dataDict = NULL;
	dataDict = new byte[ size * 4 ];
	sizeFillDict = sizeof( int );
	countItemDist = ( int* )dataDict;
	(*countItemDist) = 0;
	tagAccess = -1;
	
	dataIndex = new byte[ size * 4 ];
	BestItem::countActItem = 0;
	
	memset( mRouteMap , 0 , 0x400 );
	for( dword index = 0 ; index < 0x100 ; index++ )
	{
		mDictElm[ index ] = ( dword )index;
	}
}

DataIndex::~DataIndex()
{
	delete[] cpData;
	delete[] dataDict;
	delete[] dataIndex;
}

void DataIndex::compress()
{
	createDict();
	createIndexis();
}

bool DataIndex::getDictionary( byte storage[] , int &maxSize , int &tagAcc )
{
	if( maxSize < sizeFillDict )
	{
		cout << "err not enought memory for dictionary data" << endl;
		return false;
	}
	else
	{
		memcpy( storage , dataDict , sizeFillDict );
		maxSize = sizeFillDict;
		tagAcc = tagAccess;
		return true;
	}
}

void DataIndex::getDictionary( byte storage[] , int &tagAcc )
{
	memcpy( storage , mDictElm , 0x400 );
	tagAcc = tagAccess;
}

bool DataIndex::getIndexis( byte storage[] , int &maxSize )
{
	if( maxSize < sizeFillDataIndexis )
	{
		cout << "err not enought memory for indexis data" << endl;
		return false;
	}
	else
	{
		memcpy( storage , dataIndex , sizeFillDataIndexis );
		maxSize = sizeFillDataIndexis;
		return true;
	}	
}

bool DataIndex::addIndex( int fallIndex[] , int remainSize , int &posDatIn )
{
	int countFallIndex = 0;
	int countNextNoRootItem = 0;
	
	while( !isRoot( cpData , posDatIn , contItemRoot ) )
	{
		dataIndex[ sizeFillDataIndexis++ ] = ( byte )tagAccess;
		dataIndex[ sizeFillDataIndexis++ ] = cpData[ posDatIn++ ]; 
		countNextNoRootItem++;
		
		if( posDatIn >= sizeData )
		{
			return true;
		}
	}

	do
	{
		int pos = sizeof( int );
		int maxSize = 0;
		byte index = 0;
		int status = 0;
		int bestIndex = 0;
		int routeBestIndex = 0;
		bool nextIndex = false;
	
		while( pos < sizeFillDict )
		{
			int sizeMemblock = *(int*)(&dataDict[ pos ]);
			pos += sizeof( int );
			nextIndex = false;
 	
			for( int pos1 = 0 ; pos1 < countFallIndex ; pos1++ )
			{
				if( fallIndex[ pos1 ] == index )
				{
					nextIndex = true;
				}
			}
			
			if( !nextIndex )
			{
				status = memcmp( cpData + posDatIn , dataDict + pos , sizeMemblock );
				if( status == 0 )
				{
					if( sizeMemblock > maxSize )
					{
						maxSize = sizeMemblock;
						bestIndex = index;
						routeBestIndex = ( byte )( mRouteMap[ bestIndex ] - mDictElm );
						//routeBestIndex = bestIndex;
					}
				}	
			}

			index++;
			pos += sizeMemblock;
		}
		
		if( maxSize == 0 )
		{
			return false;
		}
		else
		{
			posDatIn += maxSize;
			dataIndex[ sizeFillDataIndexis++ ] = routeBestIndex;	
			bool status1;
			
			if( posDatIn >= sizeData )
			{
				return true;
			}

			status1 = addIndex( fallIndex + countFallIndex , remainSize - countFallIndex , posDatIn );
			
			if( !status1 )
			{
				fallIndex[ countFallIndex++ ] = routeBestIndex;
				sizeFillDataIndexis--;
				posDatIn -= maxSize;
				posDatIn -= countNextNoRootItem;
				sizeFillDataIndexis -= countNextNoRootItem * 2;
			}
			else
			{
				return true;
			}
		}	
	}
	while( countFallIndex < ( *countItemDist ) );
		
	return false;
}

int DataIndex::createIndexis()
{
	sizeFillDataIndexis = 0;	
	int remainSize = 0x4000;
	int fallIndex[ remainSize ];
	int posDatIn = 0;
		
	bool status = addIndex( fallIndex , remainSize , posDatIn );
	
	if( !status )
	{
		cout << "err algoritmus find index data" << endl;
		cout << "         posDatIn " << posDatIn << endl;
		return -3;
	}

	return 0;
}

int DataIndex::rootItem( int count )
{
	typedef struct elementCount
	{
		int count;
		byte element;
	}
	ElementCount;
	
	ElementCount elmCount[ 0x100 ];
	
	for( int pos = 0 ; pos < 0x100 ; pos++ )
	{
		elmCount[ pos ].element = ( byte )pos;
		elmCount[ pos ].count = 0;
		byte* pNexPos = cpData + 2 *sizeof( int );
		byte* endPos = pNexPos + sizeData;
		pNexPos = ( byte* )memchr( pNexPos , ( byte )pos , ( int ) ( endPos - pNexPos ) );
		
		while( pNexPos != NULL )
		{
			elmCount[ pos ].count++;
			pNexPos = ( byte* )memchr( pNexPos , ( byte )pos , ( int ) ( endPos - ++pNexPos ) );
		}
	}
	
	for( int pos = 0; pos < 0x100 - 1; pos++)
	{
		for( int pos1 = 0; pos1 < 0x100 - pos - 1; pos1++) 
			if( elmCount[ pos1 ].count < elmCount[ pos1 + 1 ].count )
			{
				ElementCount temp = elmCount[ pos1 ];
				elmCount[ pos1 ] = elmCount[ pos1 + 1 ];
				elmCount[ pos1 + 1 ] = temp;
			}	
	}
	
	int pos = 0;
	while( elmCount[ pos ].count != 0 && pos < count )
	{
		*( int* )( dataDict + sizeFillDict ) = 1;
		sizeFillDict += sizeof( int );
		dataDict[ sizeFillDict++ ] = elmCount[ pos ].element;
		mDictElm[ elmCount[ pos ].element ] = ( dword )elmCount[ pos ].element;
		(*countItemDist)++;
#ifdef SHOW_INFO
		cout << "root Item index: " <<  (*countItemDist) - 1 << endl;
		cout << (int)elmCount[ pos ].element << endl;
#endif
		pos++;
	}
	
	if( pos == count )
	{
		*( int* )( dataDict + sizeFillDict ) = 0;
		sizeFillDict += sizeof( int );
		(*countItemDist)++;
		
		int freeIndex = 0;
		int pos1 = 0;
		while( pos1 < count )
		{
			if( freeIndex == elmCount[ pos1 ].element )
			{
				pos1 = 0;
				freeIndex++;
			}
			else
			{
				pos1++;
			}
		}
		tagAccess = freeIndex;
	}
	
	return pos;
}

int DataIndex::getCountItem( byte item[] , int sizeItem )
{	 
	int countNumItem = 0;
	int posInData = 0;
	
	while( posInData + sizeItem < sizeData )	
	{	
		int status = memcmp( cpData + posInData , item , sizeItem );
			
		if( status == 0 )
		{
			countNumItem++;
			posInData += sizeItem;
		}
		else
		{
			posInData++;
		}
	}
		
	return countNumItem;
}

void DataIndex::addItem( byte Item[] , int sizeItem , dword elmDict )
{
	*( int* )( dataDict + sizeFillDict ) = sizeItem;
	sizeFillDict += sizeof( int );
	memcpy( dataDict + sizeFillDict , Item , sizeItem );
	sizeFillDict += sizeItem;
	*mRouteMap[ (*countItemDist) ] = elmDict;
	(*countItemDist)++;
	
}

void hexPrint( byte item[] , int size )
{
	for( int pos1 = 0 ; pos1 < size ; pos1++ )
	{
		if( pos1 % 0x10 == 0 )
			cout << endl;
		
		byte firstChar = item[ pos1 ];
		cout << setw(2)  << hex << (int)firstChar << " ";	
	}
	cout << endl;
}

void showItem( byte dataUniversal[] , dword *mRouteMap[] , dword mDictElm[] )
{
	cout << endl << endl<< endl << setfill('0');
	int size = 0;
	int posMem = sizeof( int );
	int posMemBlock = sizeof( int );
	int &countMemBlock = *( int* )( dataUniversal );
	
	for( int pos = 0 ; pos < countMemBlock ; pos++ )
	{
		
		size = *( int *)( dataUniversal +  posMem );
		posMem += sizeof( int );
#ifdef SHOW_INFO		
		cout << hex << ( int )( mRouteMap[ pos ] - mDictElm ) << ". sizeMemBlock: " << size << endl;
#endif

		for( int pos1 = 0 ; pos1 < size ; pos1++ )
		{
			if( pos1 % 0x10 == 0 )
				cout << endl;
			
			byte firstChar = dataUniversal[ posMem + pos1 ];
			cout << setw(2)  << hex << (int)firstChar << " ";	
		}
		cout << endl;
		posMem += size;	
	}
	
	cout << endl << endl<< endl;
}

bool DataIndex::isRoot( byte arrayDat[] , int posDat , int contItemRoot )
{
	byte element = arrayDat[ posDat ];
	int posDict = sizeof( int );
	
	for( int pos = 0; pos < contItemRoot; pos++)
	{
		int sizeBlock = *( int* )( dataDict + posDict );
		posDict += sizeof( int );
		if( dataDict[ posDict ] == element )
		{
			return true;
		}
		posDict += sizeBlock;
	}
	
	return false;
}

bool DataIndex::isRoot( dword index )
{
	for( int pos = 0; pos < contItemRoot; pos++)
	{
		if( *mRouteMap[ pos ] == index )
			return true;
	}
	
	return false;
}

bool DataIndex::generateItem( byte item[] , int &sizeItem , dword &elmDict )
{
	
	int indexItem1 = numCombineItem % ( *countItemDist );
	int indexItem2 = numCombineItem / ( *countItemDist );

	if( indexItem2 >= ( *countItemDist ) )
	{
		return false;
	}
	
	byte index1Route = ( byte )( mRouteMap[ indexItem1 ] - mDictElm );
	byte index2Route = ( byte )( mRouteMap[ indexItem2 ] - mDictElm );	
	
	while( index1Route == ( byte )tagAccess || index2Route == ( byte )tagAccess )
	{
		index1Route = ( byte )( mRouteMap[ indexItem1 ] - mDictElm );
		
		if( index1Route == ( byte )tagAccess )
		{
			if( indexItem1 + 2 > ( *countItemDist ) ) 
			{
				indexItem2++;
				indexItem1 = indexItem2;
			}	
			else
			{
				indexItem1++;
			}	
		}

		index2Route = ( byte )( mRouteMap[ indexItem2 ] - mDictElm );
		
		if( index2Route == ( byte )tagAccess )
		{
			if( indexItem2 + 2 < ( *countItemDist ) ) 
			{
				indexItem2++;
			}
			else
			{
				return false;
			}	
		}

	}
#ifdef SHOW_INFO
	cout << "index1 "<< indexItem1 << endl;
	cout << "index2 "<< indexItem2 << endl;
#endif
	numCombineItem = indexItem2 * ( *countItemDist ) + indexItem1;
	
	byte *pIndex1 = NULL , *pIndex2 = NULL;
	int sizeItem1 , sizeItem2;
	int findIndex = 0;
	int posDict = sizeof( int );
	int sizeMemBlockDsc = 0;
	
	while( ( pIndex1 == NULL || pIndex2 == NULL  ) && posDict < sizeFillDict )
	{
		posDict += sizeMemBlockDsc;	
		sizeMemBlockDsc = *( int *)( dataDict +  posDict );
		posDict += sizeof( int );
		
		if( findIndex == indexItem1 )
		{
			pIndex1 =  dataDict +  posDict;
			sizeItem1 = sizeMemBlockDsc;
		}

		if( findIndex == indexItem2 )
		{
			pIndex2 =  dataDict +  posDict;
			sizeItem2 = sizeMemBlockDsc;
		}
		
		findIndex++;
	}
		
	if( pIndex1 == NULL || pIndex2 == NULL  )
	{
		cout << "err not found item" << endl;
		return false;
	}

	if( sizeItem < sizeItem1 + sizeItem2 )
	{
		cout << "err not enought buffer for combine item" << endl;
		return false;
	}
	
	byte var1 , var2;
	
	index1Route = ( byte )( mRouteMap[ indexItem1 ] - mDictElm );
	index2Route = ( byte )( mRouteMap[ indexItem2 ] - mDictElm );
	
	var1 = isRoot( index1Route ) ? 0x1 : 0x2;
	var2 = isRoot( index2Route ) ? 0x1 : 0x2;
	byte* pElmDictByte = ( byte* )&elmDict;
	
	if( mChangeOrderItem )
	{
		memcpy( item , pIndex2 , sizeItem2 );
		memcpy( item + sizeItem2 , pIndex1 , sizeItem1 );
		pElmDictByte[ 0 ] = index2Route;
		pElmDictByte[ 1 ] = var2;
		pElmDictByte[ 2 ] = index1Route;
		pElmDictByte[ 3 ] = var1;
		mChangeOrderItem = false;
	}	
	else	
	{
		memcpy( item , pIndex1 , sizeItem1 );
		memcpy( item + sizeItem1 , pIndex2 , sizeItem2 );
		pElmDictByte[ 0 ] = index1Route;
		pElmDictByte[ 1 ] = var1;
		pElmDictByte[ 2 ] = index2Route;
		pElmDictByte[ 3 ] = var2;
		
		if( ( indexItem1 + 1 ) < ( *countItemDist ) )
		{
			indexItem1++;
		}
		else
		{
			indexItem2++;
			indexItem1 = indexItem2;
		}
		mChangeOrderItem = true;
	}

	sizeItem = sizeItem1 + sizeItem2;
	numCombineItem = indexItem2 * ( *countItemDist ) + indexItem1;

	return true;
}

bool DataIndex::popBestItem( BestItem &item )
{
	if( BestItem::countActItem == 0 )
	{
		return false;
	}
	
	item = bestItems[ 0 ];
	BestItem::countActItem--;
	int countItem = BestItem::countActItem;
	memcpy( bestItems , bestItems + 1 , sizeof( BestItem ) * countItem );
	
	return true;
}

bool DataIndex::isInDict( byte Item[] , int size )
{
	int posDict = sizeof( int );
	int sizeMemBlockDsc = 0;
	
	while( posDict < sizeFillDict )
	{
		posDict += sizeMemBlockDsc;	
		sizeMemBlockDsc = *( int *)( dataDict +  posDict );
		posDict += sizeof( int );
		
		if( memcmp( dataDict + posDict , Item , size ) == 0 )
		{
			return true;
		}
			
	}	
	
	return false;
}
/*
bool DataIndex::isInDict( dword index )
{
	for( int pos = 0; pos < ( *countItemDist ); pos++)
	{
		if( mDictElm[ pos ] == index )
			return true;
	}
	
	return false;
}
*/
void DataIndex::reductBestItem( int minReduct )
{
	while( BestItem::countActItem > 0 )
	{
		if( bestItems[ BestItem::countActItem - 1 ].reduct > minReduct )
		{
			return;
		}
		BestItem::countActItem--;
	}
}

void DataIndex::initRoutTab()
{
	int posRouteMap = 0;
	int posDict = sizeof( int );
	int sizeMemBlockDics = 0;
	int countPrimarItems = contItemRoot;
	
	for( int index = 0 ; index < contItemRoot ; index++ )
	{
		posDict += sizeMemBlockDics;	
		sizeMemBlockDics = *( int *)( dataDict +  posDict );
		posDict += sizeof( int );
		
		if( posDict > sizeFillDict )
		{
			cout << "err not enought root item in dictionary" << endl;
			return;
		}
		
		if( sizeMemBlockDics != 1 )
		{
			{
				cout << "err item in dictionary is not root" << endl;
				return;	
			}
		}
		
		mRouteMap[ posRouteMap++ ] = &mDictElm[ dataDict[ posDict ] ];
#ifdef SHOW_INFO
		cout << "root index: " << (int)dataDict[ posDict ] << endl;
#endif
	}
	
	if( tagAccess != -1 )
	{
		mRouteMap[ posRouteMap++ ] = &mDictElm[ tagAccess ];
	}
	
	int freeIndex = 0;
	
	while( posRouteMap < 0x100 )	
	{
		for( int index = 0 ; index < posRouteMap ;  )
		{
			if( mRouteMap[ index++ ] == &mDictElm[ freeIndex ] )
			{
				freeIndex++;
				index = 0;
			}
		}
		
		if( freeIndex > 0xFF )
		{
			cout << "err not possible search free index" << endl;
			return;	
		}
#ifdef SHOW_INFO
		cout << "freeIndex " << freeIndex << endl;
#endif
		mRouteMap[ posRouteMap++ ] = &mDictElm[ freeIndex ];
	}
}

void DataIndex::pushBestItem( BestItem item , int countItems )
{

	for( int pos = 0 ; pos < BestItem::countActItem  ; pos++ )
	{
		if( bestItems[ pos ].sizeItem == item.sizeItem )
		{
			if( memcmp( bestItems[ pos ].Item , item.Item , item.sizeItem ) == 0 )
			{
				return;
			}	
		}
	}	
	
	for( int pos = BestItem::countActItem ; pos >= 0  ; pos-- )
	{ 
		if( pos == countItems )
		{
			if( item.reduct < bestItems[ pos - 1 ].reduct )
			{
				return;
			}
		}
		else if( pos == 0 )
		{
			bestItems[ 0 ] = item;
			if( BestItem::countActItem < countItems )
				BestItem::countActItem++;
			return;	
		}
		else
		{
			bestItems[ pos ] = bestItems[ pos - 1 ];
			
			if( bestItems[ pos ].reduct > item.reduct )
			{
				bestItems[ pos ] = item;
				if( BestItem::countActItem < countItems )
				{
					BestItem::countActItem++;
				}
				return;
			}
		}
	}
}

int DataIndex::createDict()
{
	
	memset( mRouteMap , 0 , 0x100 );
	
	contItemRoot = 0x29;
	contItemRoot = rootItem( contItemRoot );
	initRoutTab();

	
#ifdef SHOW_INFO		
	cout << "count use root indexis: " << contItemRoot << endl;
	cout << "tagAccess: " << tagAccess << endl;
#endif

	int sizeItem = 0x100;
	byte item[ sizeItem ];
	
	int bestSizeItem = 0;
	int bestReduct = 0;

	int countItem;
	int reduct;
	BestItem itemInfo;

	int numBestItem = 0;
	int minReduct = 0x0;

	int oldNumDistItem = ( *countItemDist );
	
	numCombineItem = 0;
	while( ( *countItemDist ) < 0x100 )
	{		
		sizeItem = 0x100;
		mChangeOrderItem = true;
		dword elmDict;
		int countBestItem = 1 + ( mMaxBestItemsNum - 1 ) * ( 0x100 - (*countItemDist) ) / 0x100; 
		
		while( generateItem( item , sizeItem , elmDict ) )
		{
			
#ifdef SHOW_INFO
	cout << "sizeItem " << sizeItem << endl;
#endif
			
			countItem = getCountItem( item , sizeItem );
			
			if( countItem > 1 )
			{
				reduct = countItem * ( sizeItem - 1 );
				itemInfo.reduct = reduct;
				itemInfo.sizeItem = sizeItem;
				itemInfo.elmDict = elmDict;
				memcpy( itemInfo.Item , item , sizeItem );
#ifdef SHOW_INFO
	cout << "push item size: " << itemInfo.sizeItem << endl;
#endif
				if( !isInDict( itemInfo.Item , itemInfo.sizeItem ) )
				{
					pushBestItem( itemInfo , countBestItem );
				}
#ifdef SHOW_INFO
	cout << "act best item: " << endl;

	for( int pos = 0 ; pos < BestItem::countActItem ; pos++ )
	{
		hexPrint( bestItems[ pos ].Item , bestItems[ pos ].sizeItem );
	}
#endif
			}

			sizeItem = 0x100;
		}
		minReduct /= 1.1;
		//reductBestItem( minReduct );
		numBestItem = BestItem::countActItem;
		
		while( popBestItem( itemInfo ) && (*countItemDist) < 0x100 )
		{
			
#ifdef SHOW_INFO
	cout << "add item size: " << itemInfo.sizeItem << endl;
#endif
			
			addItem( itemInfo.Item , itemInfo.sizeItem , itemInfo.elmDict );
		}
		
#ifdef SHOW_INFO		
	cout << "countItemDist: " << hex << (*countItemDist) << endl;	
	showItem( dataDict ,mRouteMap , mDictElm );
#endif
	
		if( ( *countItemDist ) == oldNumDistItem )
			break;
		
		oldNumDistItem = ( *countItemDist );
		numCombineItem = (*countItemDist) * ( (*countItemDist) - numBestItem );
	}

	/*memset( mDictElm , 0 , 0x400 );
	
	for( int i = 0 ; i < 0x100 ; i++ )
		*mRouteMap[ i ] = 0xFFFFFFFF;
	
	ofstream DictFile( "dict.bin" , ios::out | ios::binary );
	if( DictFile.is_open() )
	{
		DictFile.write( ( char* )mDictElm , 0x400 );
		
		DictFile.close();
	}
	else
		cout << "err create file Dict" << endl;*/
	return (*countItemDist);
}
