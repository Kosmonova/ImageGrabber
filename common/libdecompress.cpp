#include <iostream>
#include <fstream>
#include <stdio.h>
#include <sstream>
#include <cstring>

//#define GET_COMPRESS_LEVEL1
//#define GET_DICTIONARY

using namespace std;


bool globEnd = false;
void (*bx)(void);
void (*dx)(void);

typedef unsigned char byte;
typedef unsigned short int word;
typedef unsigned int dword;

byte stack[ 256 * 4 ] = { 0 };

word ax , cx , bp , bVarx;
unsigned int di = 0 , si = 0 ;
byte &ah = ( ( byte* )&ax )[ 1 ];
byte &al = ( ( byte* )&ax )[ 0 ];
byte &bVarh = ( ( byte* )&bVarx )[ 1 ];
byte &bVarl = ( ( byte* )&bVarx )[ 0 ];

byte *ds , *es;

byte glob_0x516C = 0x0;
byte glob_0x516D = 0x0;
word glob_W0x516E = 0x0;
byte &glob_0x516E = ( ( byte* )&glob_W0x516E )[ 0 ];
byte &glob_0x516F = ( ( byte* )&glob_W0x516E )[ 1 ];
word glob_0x5170 = 0x0;
word glob_0x5172 = 0x0;


void func_0xcb10();
void func_0xcb38();
void func_0xcb3f();
void func_0xcb59();
void func_0xcb64();
void func_0xcb6e();	
void func_0xcb79();
void func_0xcb80();
void func_0xcb81();
void func_0xcb89();
void func_0xcb9a();
void func_0xcba2();
void func_0xcb3d();
void printInfo();

#ifdef GET_DICTIONARY
#include <iomanip> 

byte dataDict[0xFFFF];
int sizeFillDict = 0;

int* pNumMemBlock;

void fillItem()
{
	dataDict[ sizeFillDict++ ] = al;
	( *pNumMemBlock )++;
}

void getDict()
{
	dx = &fillItem;
	
	int &countItem = *( int* )( dataDict );
	sizeFillDict += sizeof( int );
	
	for( countItem = 0 ; countItem < 0x100 ; countItem++ )
	{
		pNumMemBlock = ( int* )( dataDict + sizeFillDict ); 
		sizeFillDict += sizeof( int );
		al = countItem;
		func_0xcb10();
	}
}

static void showItem( byte dataUniversal[] )
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
		
		cout << pos << ". sizeMemBlock: " << size << endl;
		byte lastChar = dataUniversal[ posMem + size - 1 ];
		byte firstChar = dataUniversal[ posMem ];

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

void showItemDict( byte dataUniversal[] )
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
		
		cout << pos << ". sizeMemBlock: " << size << endl;
		cout << "element: " << hex << setw(8) << (( unsigned int* )stack)[ pos ] << endl;
		cout << setw(2);
		
		byte firstChar = dataUniversal[ posMem ];

		word var1 = ((word*)stack)[ pos * 2 + 1 ];
		byte* var1h = &( ( byte* )&var1 )[ 1 ];
		byte* var1l = &( ( byte* )&var1 )[ 0 ];
		if( *var1h == 0 )
			cout << "item = " << (( unsigned int* )stack)[ pos ]  << endl;
		else 
		{
			cout << "index= " << ( int )(*var1l);
			word var2 = ((word*)stack)[ pos * 2 ];
			byte* var2h = &( ( byte* )&var2 )[ 1 ];
			byte* var2l = &( ( byte* )&var2 )[ 0 ];
			
			cout << " , index= " << hex << ( int )(*var2l) << endl;
				
		}
		
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
#endif


#ifdef GET_COMPRESS_LEVEL1
int sizeFillDat = 0;
extern byte dataLevel1[];

void (*origFunction)(void);

void getCompresLevel1()
{
	dataLevel1[ sizeFillDat++ ] = al;
	dx = origFunction;
	origFunction();
	origFunction = dx;
	dx = &getCompresLevel1;
}
#endif

void initStack()
{
	do
	{
		glob_0x5172 = ds[ si++ ];
		bp = glob_0x5172 * 2;
		glob_0x516D = ds[ si++ ];
	
		al = ds[ si++ ];
		
		
		bVarx = al * 4;
		ah = 1;

		if( stack[ bVarx+1 ] != 0 )
			++ah;
		
		((word*)stack)[ bp+1 ] = ax;
		al = glob_0x516D;
		bVarx = al * 4;
		ah = 1;
		
		if( stack[ bVarx+1 ] != 0 )
			++ah;
		
		((word*)stack)[ bp ] = ax;
		--glob_0x516C;
	}
	while( glob_0x516C != 0 );
	
	bx = (void(*)(void))&func_0xcb10;
}

void func_0xcb3d()
{
	dx();
	return;
}

int cycleNextCall;
int static numCallRecursive = 0;
bool valid;

bool decomp()
{
	si = 0;
	di = 0;
	globEnd = false;
	valid = true;

	glob_0x516C = ds[ si++ ];
	dx = &func_0xcb3f;
	
	if( glob_0x516C != 0 )
	{
		glob_0x5170 = ds[ si++ ];
		al = 0;
		
		for( int pos = 0; pos < 256 ; pos++ )
			((word*)stack)[ 2 * pos ]=pos;

		initStack();
	}
	else
	{
		al = ds[ si++ ];
		bx = &func_0xcb3d;
	}
	
#ifdef GET_DICTIONARY
	void ( *safeFunc )( void ) = dx;
	void ( *safeFunc1 )( void ) = bx;
	getDict();
	dx = safeFunc;
	bx = safeFunc1;
	cout << "sizeFillDict: " << sizeFillDict << endl;
	showItemDict( dataDict );
	
#endif	
	
#ifdef GET_COMPRESS_LEVEL1
	origFunction = dx;
	dx = &getCompresLevel1;
#endif
	
	for( cx = 0xFFFF ; cx > 0 ; --cx )
	{
		//printInfo();
		al = ds[ si++ ];
		cycleNextCall = 0;
		bx();
		numCallRecursive = 0;
		if( globEnd == true || valid == false )
			break;
	}
	printInfo();
	return valid;
}

void func_0xcb10loop()
{


	do
	{
		numCallRecursive++;

		if( numCallRecursive > 0xFF || valid == false )
		{
			bx = &func_0xcb80;
			dx = &func_0xcb80;
			valid = false;
			return;
		}

		bp = al * 2;
		ax = ((word*)stack)[ bp ];

		if( ah == 0 )
		{//cout << "al for dx: " << hex << (int) al << endl;
			dx();
			return;
		}
		word loclaVar1 = ((word*)stack)[ bp + 1 ];

		if( ah != 1 )
			func_0xcb10loop();
		else
			dx();
		
		ax = loclaVar1;	
	}
	while( ah != 1 );
//cout << "al for dx: " << hex << (int) al << endl;	
	dx();
	return;	
}

void func_0xcb10()
{	
	if( al == glob_0x5170 )
	{
		bx = &func_0xcb38;
		return;
	}
	numCallRecursive = 0;
	func_0xcb10loop();
}

void func_0xcb38()
{//cout << "al for dx: " << hex << (int) al << endl;
	bx = (void(*)(void))&func_0xcb10;
	dx();
	return;
}



void func_0xcb3f()
{
	cycleNextCall++;

	if( cycleNextCall > 0xF || valid == false )
	{
		bx = &func_0xcb80;
		dx = &func_0xcb80;
		valid = false;
		globEnd = true;
		return;
	}


	if( al == 0 )
	{
		func_0xcb79();
		return;
	}
	if( al < 0x40 )
	{
		func_0xcb6e();
		return;	
	}
	if( al < 0x80 )
	{
		func_0xcb64();
		return;	
	}	
	if( al < 0xC0 )
	{
		func_0xcb59();	
		return;
	}
	al &= 0x3F;
	glob_0x516F = al;
	dx = &func_0xcb9a;
	return;
}

void func_0xcb59()
{
	ax &= 0x3f;
	glob_W0x516E = ax;
	dx = &func_0xcba2;
	return;
}

void func_0xcb64()
{
	al &= 0x3F;
	glob_0x516F = al;
	dx = &func_0xcb81;
	return;
}

void func_0xcb6e()
{
	ax &= 0x3F;
	glob_W0x516E = ax;
	dx = &func_0xcb89;
	return;
}

void func_0xcb79()
{
	globEnd = true;
	dx = &func_0xcb80;
	return;
}

void func_0xcb80()
{
	return;
}

void func_0xcb81()
{
	glob_0x516E = al;
	dx = &func_0xcb89;
	return;
}

void func_0xcb89()
{
	if( di > 0xFD00 )
	{
		valid = false;
		return;
	}

	es[ di++ ] = al;
	glob_W0x516E--;
	if( glob_W0x516E == 0 )
		dx = &func_0xcb3f;
	return;
}

void func_0xcb9a()
{
	glob_0x516E = al;
	dx = &func_0xcba2;
	return;
}

void func_0xcba2()
{
	word localVar1 = cx;
	cx = glob_W0x516E;
	for( ; cx > 0 ; --cx )
	{
		if( di > 0xFD00 )
		{
			valid = false;
			return;
		}

		es[ di++ ] = al;
	}
	cx = localVar1;
	dx = &func_0xcb3f;
	return;
}

void printInfo()
{
	{
		cout << "di = " << hex << di << endl;
		cout << "ax = " << hex << ax << endl;
		cout << "si = " << hex << si << endl;
		cout << "cx = " << hex << cx << endl;
		if( bx == (void(*)(void))&func_0xcb10 )
			cout << "bx = func_0xcb10" << endl;
		else if( bx == &func_0xcb38 )
			cout << "bx = func_0xcb38" << endl;
		else if( bx == &func_0xcb3f )
			cout << "bx = func_0xcb3f" << endl;
		else if( bx == &func_0xcb59 )
			cout << "bx = func_0xcb59" << endl;
		else if( bx == &func_0xcb64 )
			cout << "bx = func_0xcb64" << endl;
		else if( bx == &func_0xcb6e )
			cout << "bx = func_0xcb6e" << endl;
		else if( bx == &func_0xcb79 )
			cout << "bx = func_0xcb79" << endl;
		else if( bx == &func_0xcb80 )
			cout << "bx = func_0xcb80" << endl;
		else if( bx == &func_0xcb81 )
			cout << "bx = func_0xcb81" << endl;
		else if( bx == &func_0xcb89 )
			cout << "bx = func_0xcb89" << endl;
		else if( bx == &func_0xcb9a )
			cout << "bx = func_0xcb9a" << endl;
		else if( bx == &func_0xcba2 )
			cout << "bx = func_0xcba2" << endl;
		else
			cout << "bx = not defaint" << endl;
		
		
		if( dx == &func_0xcb38 )
			cout << "dx = func_0xcb38" << endl;
		else if( dx == &func_0xcb3f )
			cout << "dx = func_0xcb3f" << endl;
		else if( dx == &func_0xcb59 )
			cout << "dx = func_0xcb59" << endl;
		else if( dx == &func_0xcb64 )
			cout << "dx = func_0xcb64" << endl;
		else if( dx == &func_0xcb6e )
			cout << "dx = func_0xcb6e" << endl;
		else if( dx == &func_0xcb79 )
			cout << "dx = func_0xcb79" << endl;
		else if( dx == &func_0xcb80 )
			cout << "dx = func_0xcb80" << endl;
		else if( dx == &func_0xcb81 )
			cout << "dx = func_0xcb81" << endl;
		else if( dx == &func_0xcb89 )
			cout << "dx = func_0xcb89" << endl;
		else if( dx == &func_0xcb9a )
			cout << "dx = func_0xcb9a" << endl;
		else if( dx == &func_0xcba2 )
			cout << "dx = func_0xcba2" << endl;
		else 
			cout << "dx = not defaint" << endl;
		
		cout << endl << endl << endl;
	}	
}







