#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <sstream>
#include <cstring>
#include <cmath>


#include "Elm.hpp"
#include "Dictionary.hpp"
#include "DataIndex.hpp"
#include "types.hpp"



#include <QtGlobal>

#if QT_VERSION >= 0x050000
	#include <QtWidgets>
#else
	#include <QtGui>
#endif

#include "Grabber.hpp"
#include "ui_MainWindow.h"

// NONE COMPRESS_DAT INDEXIS_DAT RAW_DAT DICT_RAW DICT_COMPRESS
#define TYPE_SAVE_DAT NONE
#define PALETE
//#define COMPRESS_1
#define COMPRESS_2
//#define FIND_COLOR 1
#define BIT_PER_COLOR_PIXEL 2
#define NONE 0
#define COMPRESS_DAT 1
#define INDEXIS_DAT 2
#define RAW_DAT 3
#define DICT_RAW 4
#define DICT_COMPRESS 5


using namespace std;

extern unsigned int di, si;

extern word ax , cx , bp , bVarx;

extern byte stack[ 256 * 4 ];
/*extern byte ah;
extern byte al;*/

static byte &ah = ( ( byte* )&ax )[ 1 ];
static byte &al = ( ( byte* )&ax )[ 0 ];

extern byte *ds , *es;
extern byte glob_0x516D;
extern byte glob_0x516C;
extern word glob_0x5172;
byte buffCompressData[ 0x4400 ];
byte dataLevel1[ 0xFD00 ];
extern int sizeFillDat;

bool decomp();
void initStack();

int sizePalData = 0x60;


Grabber::Grabber(  string filenameImg , int offsetImgAdr , string repleaceFile ,  QWidget *parent ) : QMainWindow(parent), ui(new Ui::MainWindow)
{
	mFilenameImg = filenameImg;
	mOffsetImgAdr = offsetImgAdr;
	mRepleaceFile = repleaceFile;

	ui->setupUi(this);
	//ui->lineEditOffsetFile->setValidator( new QIntValidator(0, 100, this) );
	//ui->lineEditOffsetFile->setInputMask( "Hh hh hh hh" );

	scene = new QGraphicsScene;
	image = new QImage( 640, 400, QImage::Format::Format_RGB888 );
	imageActual = new QImage( 640, 400, QImage::Format::Format_RGB888 );
#if FIND_COLOR == 2
	imageLoad = new QImage( 640, 400, QImage::Format::Format_Indexed8 );
#else
	imageLoad = new QImage( 640, 400, QImage::Format::Format_RGB888 );
#endif
	item = new QGraphicsPixmapItem( QPixmap::fromImage(*image));

	item->setPixmap( QPixmap::fromImage(*image));
	scene->addItem(item);
	ui->graphicsView->setScene(scene);
	ui->graphicsView->show();

	memblock = new byte [ 0xFFFF ];
	vgaPalete = ( RGB* )( dataImage + 0xFA00 );

	ui->scrollBarWidthImg->setMaximum( 320 );
	ui->scrollBarWidthImg->setMinimum( 0 );
	ui->scrollBarWidthImg->setValue( 320 );

	ui->scrollBarOffsetFile->setMinimum( 0 );

	datFile.open( mRepleaceFile.c_str() , ios::in | ios::binary );
	if (datFile.is_open())
	{
		datFile.seekg( 0 , datFile.end );
		int sizeFile = ( int )datFile.tellg();
		datFile.seekg( 0 , datFile.beg );
		sizeFile -= ( int )datFile.tellg();

		if( sizeFile > 0 )
			ui->scrollBarOffsetFile->setMaximum( sizeFile );
	}
	else
		cout << "Error from openning folder" << endl;

	ui->scrollBarOffsetFile->setValue( 0 );
	ui->scrollBarOffsetFile->setSingleStep( 0x100 );


	ui->scrollBarHeightImg->setMaximum( 200 );
	ui->scrollBarHeightImg->setMinimum( 0 );
	ui->scrollBarHeightImg->setValue( 200 );

	ui->scrollBarOffsetImg->setMaximum( 320 * 200 );
	ui->scrollBarOffsetImg->setMinimum( 0 );
	ui->scrollBarOffsetImg->setValue( 0 );

	ui->lineEditWidthImage->setText( "320" );
	ui->lineEditHeightImage->setText( "200" );
	ui->lineEditOffsetImg->setText( "0" );



	connect( ui->lineEditWidthImage, SIGNAL(textChanged(const QString &)), this, SLOT(changeWidthImage(const QString &)));
	connect( ui->lineEditHeightImage, SIGNAL(textChanged(const QString &)), this, SLOT(changeHeightImage(const QString &)));
	connect( ui->lineEditOffsetImg, SIGNAL(textChanged(const QString &)), this, SLOT(changeOffsetImg(const QString &)));
	connect( ui->lineEditOffsetFile, SIGNAL(textChanged(const QString &)), this, SLOT(changeOffsetFileLineEdit(const QString &)));

	connect( ui->scrollBarWidthImg, SIGNAL(valueChanged(int)), this , SLOT(changeScrWidth(int)));
	connect( ui->scrollBarHeightImg, SIGNAL(valueChanged(int)), this , SLOT(changeScrHeight(int)));
	connect( ui->scrollBarOffsetImg, SIGNAL(valueChanged(int)), this , SLOT(changeScrOffset(int)));
	connect( ui->scrollBarOffsetFile, SIGNAL(valueChanged(int)), this , SLOT(changeOffsetFile(int)));
}

Grabber::~Grabber()
{
	datFile.close();
}


void Grabber::on_btnGrab_clicked()
{
	QString qNumOffset =ui->lineEditOffsetFile->text();
	bool status;
	mOffsetImgAdr = qNumOffset.toInt( &status , 16 );

	es = dataImage;
	ds = buffCompressData;
	memset( dataImage , 0 , 0xFD00 );

/*	datFile.seekg( mOffsetImgAdr , datFile.beg );
	datFile.read( (char*)buffCompressData , 0x4400 );
	if (datFile)
		cout << "all characters read successfully." << endl;
	else
		cout << "error: only " << datFile.gcount() << " could be read" << endl;
*/

	ifstream datFile1;
	datFile1.open( mRepleaceFile.c_str() , ios::in | ios::binary );
	if (datFile1.is_open())
	{
		datFile1.seekg( mOffsetImgAdr , datFile1.beg );
		datFile1.read( (char*)buffCompressData , 0x4400 );
		if (datFile1)
			cout << "all characters read successfully." << endl;
		else
			cout << "error: only " << datFile1.gcount() << " could be read" << endl;
		datFile1.close();
	}
	else
		cout << "Error from openning folder" << endl;



	if( decomp() )
	{
		vgaPalete = ( RGB* )( es + di - 0x300 );

		redrawImage();
	}
}

void Grabber::on_btnGrayPal_clicked()
{
	for( int indexPal = 0 ; indexPal < 0x100 ; indexPal++ )
		vgaPalete[ indexPal ] = ( RGB ){ indexPal , indexPal , indexPal };

	redrawImage();
}

void Grabber::on_btnNextPicture_clicked()
{
	QString qNumOffset =ui->lineEditOffsetFile->text();
	bool status;
	mOffsetImgAdr = qNumOffset.toInt( &status , 16 );

	es = dataImage;
	ds = buffCompressData;
	memset( dataImage , 0 , 0xFD00 );

	int memBlockCompresData = 0;

	do
	{
		mOffsetImgAdr += memBlockCompresData;
		memBlockCompresData = 0x100;

		if( mOffsetImgAdr > ui->scrollBarOffsetFile->maximum() )
		{
			mOffsetImgAdr = ui->scrollBarOffsetFile->maximum();
			break;
		}

		ifstream datFile1;
		datFile1.open( mRepleaceFile.c_str() , ios::in | ios::binary );
		if (datFile1.is_open())
		{
			datFile1.seekg( mOffsetImgAdr , datFile1.beg );
			datFile1.read( (char*)buffCompressData , 0x4400 );
			if (datFile1)
				cout << "all characters read successfully." << endl;
			else
				cout << "error: only " << datFile1.gcount() << " could be read" << endl;
			datFile1.close();
		}
		else
			cout << "Error from openning folder" << endl;

	}
	while( !decomp() );

	/*if( prevMembock == si )
	{
		memBlockCompresData = si;
		memBlockCompresData = 0x100 * ( memBlockCompresData / 0x100 )+ ( memBlockCompresData % 0x100 > 0 ? 0x100 : 0 );
	}*/




	vgaPalete = ( RGB* )( es + di - 0x300 );

	redrawImage();

	mOffsetImgAdr += memBlockCompresData;
	ui->scrollBarOffsetFile->setValue( mOffsetImgAdr );
}

void Grabber::on_btnNextPalPicture_clicked()
{
	/*QString qNumOffset =ui->lineEditOffsetFile->text();
	bool status;
	mOffsetImgAdr = qNumOffset.toInt( &status , 16 );*/

	es = dataImage;
	ds = buffCompressData;

	int memBlockCompresData = 0;
	bool status1;

	do
	{
		memset( dataImage , 0x0 , 0xFA00 );
		mOffsetImgAdr += memBlockCompresData;
		memBlockCompresData = 0x100;

		if( mOffsetImgAdr > ui->scrollBarOffsetFile->maximum() )
		{
			mOffsetImgAdr = ui->scrollBarOffsetFile->maximum();
			break;
		}

		ifstream datFile1;
		datFile1.open( mRepleaceFile.c_str() , ios::in | ios::binary );
		if (datFile1.is_open())
		{
			datFile1.seekg( mOffsetImgAdr , datFile1.beg );
			datFile1.read( (char*)buffCompressData , 0x4400 );
			if (datFile1)
				cout << "all characters read successfully." << endl;
			else
				cout << "error: only " << datFile1.gcount() << " could be read" << endl;
			datFile1.close();
		}
		else
			cout << "Error from openning folder" << endl;

		status1 = decomp();

		if( ( di < 0xFD0F && di > 0xFCF0 ) && status1 )
		{
			xSizeImg = 320;
			ySizeImg = 200;
		}

		if( ( di < 0x418F && di > 0x417F ) && status1 )
		{
			xSizeImg = 160;
			ySizeImg = 100;
		}
	}
	while( !status1 || !( ( ( di - 0x300 ) % 160 == 0 ) || ( di < 0x418F && di > 0x417F ) ) );

	ui->scrollBarOffsetImg->setMaximum( xSizeImg * ySizeImg );

	if( ui->scrollBarOffsetImg->value() > xSizeImg * ySizeImg )
		ui->scrollBarOffsetImg->setValue( xSizeImg * ySizeImg );

	memBlockCompresData = si - 0xF;
	memBlockCompresData = 0x100 * ( memBlockCompresData / 0x100 ) + ( memBlockCompresData % 0x100 > 0 ? 0x100 : 0 );

	vgaPalete = ( RGB* )( es + di - 0x300 );

	redrawImage();


	ui->scrollBarOffsetFile->setValue( mOffsetImgAdr );
	mOffsetImgAdr += memBlockCompresData;

}

void Grabber::on_btnLoad_clicked()
{
	QString pathLoadImg = ui->lineEditLoadImg->text();
	bool status = imageLoad->load( pathLoadImg.toStdString().c_str() );

	if( status )
	{
#if FIND_COLOR == 2
		*imageLoad = imageLoad->convertToFormat( QImage::Format::Format_Indexed8 );
		QVector<QRgb> colTable;
		imageLoad->setColorCount( 10 );
		colTable = imageLoad->colorTable();
		*imageLoad = imageLoad->convertToFormat( QImage::Format::Format_Indexed8 , colTable );
		cout << "size col table: " << colTable.count() << endl;
#endif
		item->setPixmap( QPixmap::fromImage( imageLoad->scaled( imageLoad->width() * 2 , imageLoad->height() * 2 ) ));
		cout << "susceed load image" << endl;
	}
	else
		cout << "err not valid path " << mFilenameImg << endl;
}

void Grabber::on_btnLoadedImg_clicked()
{
	item->setPixmap( QPixmap::fromImage( imageLoad->scaled( imageLoad->width() * 2 , imageLoad->height() * 2 ) ));
}

void Grabber::on_btnActualImg_clicked()
{
	decomp();
	vgaPalete = ( RGB* )( es + di - 0x300 );
	redrawImage();
}

void Grabber::on_btnRepleace_clicked()
{
	compress();
}

void Grabber::on_btnSaveImg_clicked()
{
	if( mFilenameImg != "" )
	{
		bool status = imageActual->copy( 0 , 0 , xSizeImg , ySizeImg).save( mFilenameImg.c_str() );
		if( status )
			cout << mFilenameImg << " is suscefull save." << endl;
		else
			cout << "err save " << mFilenameImg << endl;
	}
}

void Grabber::redrawImage()
{
	QColor colFill( 0,0,0 );
	imageActual->fill( colFill );

	for( int posY = 0 ; posY < ySizeImg * 2 ; posY++ )
	{
		int rawPosImg = 0;

		for( int posX = 0 ; posX < xSizeImg * 2 ; posX++ )
		{
			rawPosImg = mOffsetImg + posY / 2 * xSizeImg + posX / 2;

			if( rawPosImg >  xSizeImg * ySizeImg )
				break;

			char collorPixel = dataImage[ rawPosImg ];

			char collorB = vgaPalete[ collorPixel ].B;
			char collorG = vgaPalete[ collorPixel ].G;
			char collorR = vgaPalete[ collorPixel ].R;

			QRgb qCollPixel = qRgb( collorR, collorG, collorB );
			imageActual->setPixel( posX,  posY, qCollPixel );
		}

		if( rawPosImg >  xSizeImg * ySizeImg)
			break;
	}

	item->setPixmap( QPixmap::fromImage( imageActual->scaled(imageActual->width() , imageActual->height() ) ));
}

void Grabber::changeWidthImage( const QString &sizeImage )
{
	bool status;
	int sizeImg = sizeImage.toInt( &status , 10 );

	if( sizeImg <= 320 && sizeImg > 0 )
	{
		xSizeImg = sizeImg;
		ui->scrollBarWidthImg->setValue( xSizeImg );
		ui->scrollBarOffsetImg->setMaximum( xSizeImg * ySizeImg );

		if( ui->scrollBarOffsetImg->value() > xSizeImg * ySizeImg )
			ui->scrollBarOffsetImg->setValue( xSizeImg * ySizeImg );

		redrawImage();
	}
}

void Grabber::changeHeightImage( const QString &heightImage )
{
	bool status;
	int heightImg = heightImage.toInt( &status , 10 );

	if( heightImg <= 200 && heightImg > 0 )
	{
		ySizeImg = heightImg;
		ui->scrollBarHeightImg->setValue( ySizeImg );
		ui->scrollBarOffsetImg->setMaximum( xSizeImg * ySizeImg );

		if( ui->scrollBarOffsetImg->value() > xSizeImg * ySizeImg )
			ui->scrollBarOffsetImg->setValue( xSizeImg * ySizeImg );

		redrawImage();
	}
}

void Grabber::changeOffsetImg( const QString &offsetImage )
{
	bool status;
	int offsetImg = offsetImage.toInt( &status , 10 );

	if( offsetImg <= ySizeImg * xSizeImg && offsetImg > 0 )
	{
		mOffsetImg = offsetImg;
		redrawImage();
	}
}

void Grabber::changeOffsetFileLineEdit( const QString &qOffsetFile )
{
	bool status;
	int offsetNum = qOffsetFile.toInt( &status , 16 );

	if( offsetNum <= ui->scrollBarOffsetFile->maximum() && offsetNum > 0 )
	{
		mOffsetImgAdr = offsetNum;
		ui->scrollBarOffsetFile->setValue( mOffsetImgAdr );
	}
}

void Grabber::changeScrHeight( int value )
{
	QString qNumHeight = QString::number( value );
	ui->lineEditHeightImage->setText( qNumHeight );
	changeHeightImage( qNumHeight );
}

void Grabber::changeScrWidth( int value )
{
	QString qNumWidth = QString::number( value );
	ui->lineEditWidthImage->setText( qNumWidth );
	changeWidthImage( qNumWidth );
}

void Grabber::changeScrOffset( int value )
{
	QString qNumOffset = QString::number( value );
	ui->lineEditOffsetImg->setText( qNumOffset );
	changeOffsetImg( qNumOffset );
}

void Grabber::changeOffsetFile( int value )
{
	int newValue = value / 0x100;
	newValue *= 0x100;

	if( value % 0x100 == 0x100 )
		newValue += 0x100;

	QString qNumOffset = QString::number( newValue , 16 );
	ui->scrollBarOffsetFile->setValue( newValue );
	ui->lineEditOffsetFile->setText( qNumOffset );
}

void Grabber::compress()
{
	fstream binRepleaceFile;

	if( mRepleaceFile != "" )
	{
		binRepleaceFile.open( ( const char* )mRepleaceFile.c_str() , ios::out | ios::in | ios::binary /*| ios::trunc */);

		if (!binRepleaceFile.is_open())
		{
			cout << "Error from openning folder" << endl;
			//return -2;
		}
	}

	int numDates = 320 *200;
	vgaPalete = (RGB*)( dataImage + 0xFA00 );

#if FIND_COLOR == 1

	unsigned int maxValuePerColor = pow( 2 , BIT_PER_COLOR_PIXEL ) - 1;
	int resolutionPerColor = 0xFF / pow( 2 , BIT_PER_COLOR_PIXEL );

	for( unsigned int pos1 = 0 ; pos1 < pow( 8 , BIT_PER_COLOR_PIXEL ) ; pos1++ )
	{
		byte* colB = &vgaPalete[ pos1 ].B;
		byte* colG = &vgaPalete[ pos1 ].G;
		byte* colR = &vgaPalete[ pos1 ].R;
		vgaPalete[ pos1 ] = ( RGB ){ 0 , 0 , 0 };

		*colB = pos1 & maxValuePerColor;
		*colB *= resolutionPerColor;
		*colG = ( pos1 >> BIT_PER_COLOR_PIXEL ) & maxValuePerColor;
		*colG *= resolutionPerColor;
		*colR = ( pos1 >> ( BIT_PER_COLOR_PIXEL << 1 ) ) & maxValuePerColor;
		*colR *= resolutionPerColor;

		for( int pos = 0 ; pos < numDates ; pos++ )
		{
			QRgb qCollor = imageLoad->pixel( pos % 320 , pos / 320 );
			byte rCol , gCol , bCol;
			rCol = ( byte )qRed( qCollor );
			gCol = ( byte )qGreen( qCollor );
			bCol = ( byte )qBlue( qCollor );

			dataImage[ pos ] = bCol / resolutionPerColor;
			dataImage[ pos ] += ( gCol / resolutionPerColor ) << BIT_PER_COLOR_PIXEL;
			dataImage[ pos ] += ( rCol / resolutionPerColor ) << ( BIT_PER_COLOR_PIXEL << 1 );
		}
	}
#elif  FIND_COLOR == 2

	QVector<QRgb> colTable;
	//imageLoad->setColorCount(2);
	colTable = imageLoad->colorTable();

	for( int indexPal = 0 ; indexPal < colTable.size() ; indexPal++ )
	{
		QRgb colorItem = colTable[ indexPal ];
		byte rCol , gCol , bCol;
		rCol = ( byte )qRed( colorItem );
		gCol = ( byte )qGreen( colorItem );
		bCol = ( byte )qBlue( colorItem );

		vgaPalete[ indexPal ].B = bCol;
		vgaPalete[ indexPal ].G = gCol;
		vgaPalete[ indexPal ].R = rCol;
	}

	for( int pos = 0 ; pos < numDates ; pos++ )
	{
		byte indexImg = imageLoad->pixelIndex( pos % 320 , pos / 320 );
		if( indexImg > 50 )
			cout << "index <50" << endl;
		dataImage[ pos ]  = indexImg;
	}

#else
	int countCollor = 0;

	for( int pos = 0 ; pos < numDates ; pos++ )
	{
		QRgb colorItem = imageLoad->pixel( pos % 320 , pos / 320 );
		byte rCol , gCol , bCol;
		rCol = ( byte )qRed( colorItem );
		gCol = ( byte )qGreen( colorItem );
		bCol = ( byte )qBlue( colorItem );
		RGB colRGB = { rCol , gCol , bCol };

		for( int indexTable = 0 ; indexTable < countCollor ; indexTable++ )
		{
			if( ( rCol == vgaPalete[ indexTable ].R ) && ( gCol == vgaPalete[ indexTable ].G ) && ( bCol == vgaPalete[ indexTable ].B ) )
			{
				dataImage[ pos ]  = ( byte )indexTable;
				break;
			}
			if( indexTable == countCollor - 1 )
			{
				if( countCollor > 256 )
				{
					cout << "err bitmap have above that 256 collors" << endl;
					cout << "pos: " << pos << endl;
					return;
				}
				else
				{
					dataImage[ pos ]  = ( byte )countCollor;
					vgaPalete[ countCollor++ ] = colRGB;
				}
				break;
			}
		}

		if( countCollor == 0 )
		{
			dataImage[ pos ]  = ( byte )countCollor;
			vgaPalete[ countCollor++ ] = colRGB;
		}
	}
	cout << "countCollor: " << countCollor << endl;
#endif

cout << "code image" << endl;

#ifdef COMPRESS_1

	memblock[ 0 ] = 0;
	memblock[ 1 ] = 0;
	int sizeout = 2;
	sizeout += compressData( dataImage , memblock + 2 , 0xFD00 , 0xFA00 );
	cout << "compress data, size= " << hex << sizeout << endl;

#endif

#ifdef COMPRESS_2

	int sizeout = compressData( dataImage , memblock , 0xFD00 , 0xFA00 );
	cout << "compress data, size= " << hex << sizeout << endl;

#endif

#if TYPE_SAVE_DAT == COMPRESS_DAT

	binFile.write( (char*)memblock , sizeout );
	binFile.flush();

#endif



	ds = memblock;


#ifdef COMPRESS_2

	int sizeDict = sizeout * 4;
	int sizeIndexis = sizeout;
	byte dataDictionary[ sizeDict ];
	byte dataIndexis[ sizeIndexis ];
	DataIndex datIndex( memblock , sizeout );

	datIndex.compress();
cout << "compress end" << endl;
	int tagAccess;
	byte dict0x400[ 0x400 ];
	datIndex.getDictionary( dataDictionary , sizeDict , tagAccess );
	datIndex.getDictionary( dict0x400 , tagAccess );
	datIndex.getIndexis( dataIndexis , sizeIndexis );

cout << "dictionary data, size= " << hex << sizeDict << endl;
cout << "indexis data, size= " << hex << sizeIndexis << endl;

#if TYPE_SAVE_DAT == DICT_RAW

	binFile.write( (char*)dict0x400 , 0x400 );
	binFile.flush();

#endif
	byte compress2Dat[ 0xF000 ] = { 0 };
	Dictionary dictionary( 0x400 );
	dictionary.setInitDat( dict0x400 );
	int posDatActual = dictionary.compress( compress2Dat , 0xFD00 , ( byte )tagAccess );

#if TYPE_SAVE_DAT == DICT_COMPRESS

	binFile.write( (char*)compress2Dat , posDatActual );
	binFile.flush();

#endif
	memcpy( compress2Dat + posDatActual , dataIndexis , sizeIndexis );
	//memcpy( pBuffRplFile + offsetImgAdr , compress2Dat , posDatActual + sizeIndexis );
//	binRepleaceFile.write( pBuffRplFile , remindSizeRplFile );
	QString qNumOffset =ui->lineEditOffsetFile->text();
	bool status;
	mOffsetImgAdr = qNumOffset.toInt( &status , 16 );
	binRepleaceFile.seekg( mOffsetImgAdr , binRepleaceFile.beg );

#ifdef COMPRESS_2
	binRepleaceFile.write( (char*)compress2Dat , posDatActual + sizeIndexis );
#endif
	binRepleaceFile.close();

	memset( memblock , 3 , 0xFD00 );

#if TYPE_SAVE_DAT == INDEXIS_DAT

	binFile.write( (char*)dataIndexis , sizeIndexis );
	binFile.flush();

#endif

	/*binFile.write( (char*)memblock , size );
	binFile.flush();*/
cout << "total compress dat = " << hex << posDatActual + sizeIndexis << endl;
	ds = compress2Dat;
#endif

#ifdef COMPRESS_1
	binRepleaceFile.seekg( mOffsetImgAdr , binRepleaceFile.beg );
	binRepleaceFile.write( (char*)memblock , sizeout );
	binRepleaceFile.close();
#endif

	es = dataImage;

#ifdef COMPRESS_2
	memset( dataImage , 3 , 0xFA00 );
	decomp();
#else
#ifdef COMPRESS_1
	decomp();
#endif
#endif

cout << "decompress data" << endl;
	//al_clear_to_color(al_map_rgb(0,255,0));
	QColor colFill( 0,255,0 );
	image->fill( colFill );

#if TYPE_SAVE_DAT == RAW_DAT

	binFile.write( (char*)dataImage , 0xFD00 );
	binFile.flush();

#endif
	for( int posY = 0 ; posY < ySizeImg * 2 ; posY++ )
		for( int posX = 0 ; posX < xSizeImg * 2; posX++ )
		{
			char collorPixel = dataImage[ posY /2 * xSizeImg + posX / 2 ];
			char collorB = vgaPalete[ collorPixel ].B;
			char collorG = vgaPalete[ collorPixel ].G;
			char collorR = vgaPalete[ collorPixel ].R;

			QRgb qCollPixel = qRgb( collorR, collorG, collorB );
			image->setPixel( posX,  posY, qCollPixel );
		}

	item->setPixmap( QPixmap::fromImage(*image));
}


int Grabber::compressData( byte dataIn[] , byte dataOut[] , int sizeIn , int sizeOut )
{
	di = 0;
	si = 0;

		
	byte *numElm = NULL;

	while( di < sizeOut && si < sizeIn )
	{
		byte element = dataIn[ si++ ];
		cx = 1;
		
		while( element == dataIn[ si ] )
		{
			si++;
			cx++;
		}
			
		if( cx > 1 )
		{
			numElm = NULL;
			do
			{
				if( cx >= 0xFF )
				{
					byte numByte = 0;
					
					while( ( cx > 0x100 ) && ( numByte < 0x3F ) )
					{
						cx -= 0x100;
						numByte++;
					}
					
					dataOut[ di++ ] = 0xC0 + numByte;
					
					numByte =  ( byte )( ( cx >= 0xFF ) ? 0xFF : cx );
					cx -= (word)numByte;
					dataOut[ di++ ] = numByte;
					
					dataOut[ di++ ] = element;
				}
				else if( cx > 0x3F )
				{
					dataOut[ di++ ] = 0xBF;
					dataOut[ di++ ] = element;
					cx -= 0x3F;
				}
				else
				{
					dataOut[ di++ ] = 0x80 + ( byte )cx;
					dataOut[ di++ ] = element;
					cx = 0;	
				}
			}
			while( cx > 0 );
		}
		else
		{
			if( ( numElm == NULL) || ( (*numElm) > 0x3E ) )
			{
				numElm = &dataOut[ di++ ];
				(*numElm) = 0;
			}

			dataOut[ di++ ] = element;
			(*numElm)++;	
		}
	}
	
	dataOut[ di++ ] = 0;
	
	if( di > sizeOut )
	{
		cout << "error sizeOut" << endl;
		return di;
	}
	else
	{
		return di;
	}
}



