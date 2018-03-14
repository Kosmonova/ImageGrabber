#include <iostream>
#include <fstream>
#include <stdio.h>
#include <sstream>
#include <cstring>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#define PALETE

//#include "VgaPalette.h"

using namespace std;

ALLEGRO_EVENT_QUEUE *queue;

bool globEnd = false;
void (*bx)(void);
void (*dx)(void);

typedef unsigned char byte;
typedef unsigned short int word;
typedef unsigned int dword;

byte stack[ 256 * 4 ] = { 0 };
byte data[ 0x4400 ];

typedef struct RGB_COLL 
{
	byte R;
	byte G;
	byte B;
}RGB;

RGB* vgaPalete;

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

//#define GET_COMPRESS_LEVEL1
//#define GET_DICTIONARY

#ifdef GET_COMPRESS_LEVEL1
int sizeFillDat = 0;
byte dataLevel1[0xFFFF];

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
	
	for( countItem = 0 ; countItem < 0xFF ; countItem++ )
	{
		pNumMemBlock = ( int* )( dataDict + sizeFillDict ); 
		sizeFillDict += sizeof( int );
		al = countItem;
		func_0xcb10();
	}
}

void showItem( byte dataUniversal[] )
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

void printInfo();

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


void decomp()
{
	si = 0;
	di = 0;
	
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
	getDict();
	dx = safeFunc;
	cout << "sizeFillDict: " << sizeFillDict << endl;
	showItemDict( dataDict );
	
#endif	
	
#ifdef GET_COMPRESS_LEVEL1
	origFunction = dx;
	dx = &getCompresLevel1;
#endif
		
	for( cx = 0xFFFF ; cx > 0 ; --cx )
	{
		al = ds[ si++ ];
		bx();
		
		if( globEnd == true )
			break;
	}
	printInfo();
	return;
}

void func_0xcb10loop()
{
	do
	{
		bp = al * 2;
		ax = ((word*)stack)[ bp ];
		if( ah == 0 )
		{
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
	
	func_0xcb10loop();
}
/*
void func_0xcb10()
{
	
	if( al == glob_0x5170 )
	{
		bx = &func_0xcb38;
		return;	
	}
	
	word localVarArray[ 2000 ];
	int indexArray = 0;
	bool jumpCode = false;
	
	do
	{
		do
		{
			if( !jumpCode )
			{
				bp = al * 2;
				ax = ((word*)stack)[ bp ];
				if( ah == 0 )
				{
					dx();
					jumpCode = true;
					
					if( indexArray == 0 )
						return;
				}
				else
				{
					localVarArray[ indexArray++ ] = ((word*)stack)[ bp + 1 ];
				}
			}
			else
			{
				ax = localVarArray[ --indexArray ];
				if( ah != 1 )
					jumpCode = false;
			}
		}	
		while( !jumpCode || ( ah != 1 ) );
		dx();
		jumpCode = true;
	}
	while( indexArray > 0 );
}
*/
void func_0xcb38()
{
	bx = (void(*)(void))&func_0xcb10;
	dx();
	return;
}

void func_0xcb3d()
{
	dx();
	return;
}

void func_0xcb3f()
{
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

void func_0xca8c()
{
	globEnd = true;	
}

void func_0xcb79()
{
	bx = &func_0xca8c;
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
	es[ di ] = al;
	di++;
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
		es[ di ] = al;
		di++;
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





void run()
{
	ALLEGRO_EVENT event;
	bool need_draw = true;

	
	while (1) 
	{
		al_is_event_queue_empty(queue);
		al_wait_for_event(queue, &event);


		switch (event.type) 
		{
			case ALLEGRO_EVENT_DISPLAY_CLOSE:
				return;
				
				
			case ALLEGRO_EVENT_KEY_DOWN:
			{
				if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
					return;	
			}

			break;
		}
	}
}


volatile int xSizeImg = 640 , ySizeImg = 400;


int main(int argc, char **argv)
{
	string path = "LOTUS.DAT";
	int offsetImgAdr = 0x41000;
	string typeDat;
	string filenameImg = "";
	
	for( int param = 1 ; param < argc ; param++ )
	{
		if (string(argv[ param ]) == "-offset")
		{
			istringstream(argv[++param]) >> hex >> offsetImgAdr;
		}
		else if (string(argv[ param ]) == "-typeDat")
		{
			 typeDat = argv[++param];
		}
		else if (string(argv[ param ]) == "-path")
		{
			path = argv[++param];
		}
		else if (string(argv[ param ]) == "-nameImg")
		{
			filenameImg = argv[++param];
		}
		else
		{
			cout << "Unknown program parameter: \"" << argv[ param ] << "\"" << endl;
			return -1;
		}
	}
	
	byte * memblock;
	int size = 320 * 210;//xSizeImg * ySizeImg;
	memblock = new byte [size];
	es = memblock;
	
	ds = data;
	memset( memblock , 0x20 ,320 * 200 );
	
	ofstream binFile;
	binFile.open( "rawData.bin" , ios::out | ios::binary );
	if (!binFile.is_open())
	{
		cout << "Error from openning folder" << endl;
		return -2;
	}
	
	ifstream datFile;
	datFile.open( path.c_str() , ios::in | ios::binary );
	if (!datFile.is_open())
	{
		cout << "Error from openning folder" << endl;
		return -2;
	}
	datFile.seekg( offsetImgAdr , datFile.beg );
	datFile.read( (char*)data , 0x4400 );
	if (datFile)
		cout << "all characters read successfully." << endl;
	else
		cout << "error: only " << datFile.gcount() << " could be read" << endl;
	datFile.close();
		
	decomp();
	vgaPalete = ( RGB* )( es + 0xFA00 );
	
	if( di < 0xFD00 )
	{
		for( int indexPal = 0 ; indexPal < 0x100 ; indexPal++ )
			vgaPalete[ indexPal ] = ( RGB ){ indexPal , indexPal , indexPal };
	}
	if(!al_init()) {
		fprintf(stderr, "failed to initialize allegro!\n");
		//return -1;
	}
	al_init_image_addon();
	al_install_keyboard();
	ALLEGRO_DISPLAY *display = al_create_display(640, 400);
	if(!display) {
		fprintf(stderr, "failed to create display!\n");
		//return -1;
	}

	al_clear_to_color(al_map_rgb(0,255,0));
	
	ALLEGRO_BITMAP *bitmap = al_create_bitmap(320, 200);
	
	
	for( int posY = 0 ; posY < ySizeImg ; posY++ )
		for( int posX = 0 ; posX < xSizeImg ; posX++ )
		{
			char collorPixel = memblock[ posY /2 * xSizeImg / 2 + posX / 2 ];
			
#ifndef PALETE
			//Bit    7  6  5  4  3  2  1  0
			//Data   R  R  R  G  G  G  B  B
			char collorB = ( collorPixel & 0x3 ) << 5;
			char collorG = ( collorPixel & 0x1D ) << 3;
			char collorR = collorPixel & 0xE0;
#else
			char collorB = /*0x4 **/vgaPalete[ collorPixel ].B;//( vgaPalete[ collorPixel ] & 0xff );
			char collorG = /*0x4 **/vgaPalete[ collorPixel ].G;//(( vgaPalete[ collorPixel ] & 0xff00 ) >> 8;	
			char collorR = /*0x4 **/vgaPalete[ collorPixel ].R;//(( vgaPalete[ collorPixel ] & 0xff0000 ) >> 16 ;		
#endif	
			al_draw_pixel( posX , posY , al_map_rgb( collorR , collorG , collorB ));

		}
	
	al_set_target_bitmap( bitmap );
	
	for( int posY = 0 ; posY < ySizeImg / 2 ; posY++ )
		for( int posX = 0 ; posX < xSizeImg / 2 ; posX++ )
		{
			char collorPixel = memblock[ posY * xSizeImg / 2 + posX ];
			
#ifndef PALETE
			//Bit    7  6  5  4  3  2  1  0
			//Data   R  R  R  G  G  G  B  B
			char collorB = ( collorPixel & 0x3 ) << 5;
			char collorG = ( collorPixel & 0x1D ) << 3;
			char collorR = collorPixel & 0xE0;
#else
			char collorB = /*0x4 **/ vgaPalete[ collorPixel ].B;//( vgaPalete[ collorPixel ] & 0xff );
			char collorG = /*0x4 **/ vgaPalete[ collorPixel ].G;//( vgaPalete[ collorPixel ] & 0xff00 ) >> 8;	
			char collorR = /*0x4 **/ vgaPalete[ collorPixel ].R;//( vgaPalete[ collorPixel ] & 0xff0000 ) >> 16 ;		
#endif	

			al_put_pixel( posX , posY ,  al_map_rgb( collorR , collorG , collorB ) );
		}
	
	al_flip_display();
	queue = al_create_event_queue();
	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_display_event_source(display));

	if( filenameImg != "" )
	{ 
		bool status = al_save_bitmap( ( const char* )filenameImg.c_str() , /*al_get_backbuffer( display )*/ bitmap );
		if( status )
			cout << filenameImg << " is suscefull save." << endl;
		else
			cout << "err save " << filenameImg << endl;
	}
		
	
	run();
	
	if( typeDat == "stack" )
	{
		for( int pos = 0 ; pos < 1024 ; pos++ )
		{
			char byte = stack[ pos ];
			binFile << byte;
		}
	}
	#ifdef GET_COMPRESS_LEVEL1
	else if( typeDat == "compresLev1" )
	{
		for( int pos = 0 ; pos < sizeFillDat ; pos++ )
		{
			char byte = dataLevel1[ pos ];
			binFile << byte;
		}	
	}
	#endif
	else
	{
		for( int pos = 0 ; pos < 0xFD00 ; pos++ )
		{
			char byte = memblock[ pos ];
			binFile << byte;
		}		
	}


	
	
	binFile.close();
	
	delete [] memblock;
	al_destroy_display(display);
	al_destroy_bitmap(bitmap);

	return 0;
}
