#ifndef DATAINDEX_H
#define DATAINDEX_H
 
#include "types.hpp"

class DataIndex
{
	private:
		
		typedef struct BstItem
		{
			static int countActItem;
			int reduct;
			int sizeItem;
			dword elmDict;
			byte Item[ 0x100 ];
		}
		BestItem;
		BestItem bestItems[ 0x100 ];
		const int mMaxBestItemsNum = 0x80;
		dword *mRouteMap[ 0x100 ];
		dword mDictElm[ 0x100 ];
		int contItemRoot;
		
		byte *dataDict;
		int sizeFillDict;
		int* countItemDist;
		int tagAccess;
		int numCombineItem;
		bool mChangeOrderItem;
		
		byte *cpData = NULL;
		int sizeData;	
		
		byte *dataIndex = NULL;
		int sizeFillDataIndexis;
		
		int createDict();
		bool isRoot( byte arrayDat[] , int posDat , int contItemRoot );
		bool isRoot( dword index );
		void initRoutTab();
		
		bool generateItem( byte item[] , int &sizeItem , dword &elmDict );
		int getCountItem( byte item[] , int sizeItem );
		bool isInDict( byte Item[] , int size );
		//bool isInDict( dword index );
		void pushBestItem( BestItem item , int countItems = 0x100 );
		void reductBestItem( int minReduct );
		bool popBestItem( BestItem &item );
		void addItem( byte Item[] , int sizeItem , dword elmDict );;
		
		int createIndexis();
		bool addIndex( int fallIndex[] , int remainSize , int &posDatIn );
		int rootItem( int count );
		
	public:
		DataIndex( byte data[] , int size );
		~DataIndex();
		void compress();
		bool getDictionary( byte storage[] , int &maxSize , int &tagAcc );
		void getDictionary( byte storage[] , int &tagAcc );
		bool getIndexis( byte storage[] , int &maxSize );
};


#endif