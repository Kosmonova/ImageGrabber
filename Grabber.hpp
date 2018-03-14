#include <QMainWindow>
#include <QGraphicsScene>
#include "types.hpp"
#include <fstream>

using namespace std;
namespace Ui {
class MainWindow;
}

class Grabber : public QMainWindow
{
	Q_OBJECT

	private:

		string mFilenameImg;
		int mOffsetImgAdr;
		string mRepleaceFile;

		Ui::MainWindow *ui;
		QImage *image , *imageActual , *imageLoad;
		QGraphicsScene* scene;
		QGraphicsPixmapItem* item;

		volatile int xSizeImg = 320 , ySizeImg = 200 , mOffsetImg = 0;
		typedef struct RGB_COLL
		{
			byte R;
			byte G;
			byte B;
		}RGB;
		RGB* vgaPalete;
		byte * memblock;
		byte dataImage[ 0xFD00 ] = { 0x3 };
		ifstream datFile;

		int compressData( byte dataIn[] , byte dataOut[] , int sizeIn , int sizeOut );
		void redrawImage();

	public:
		explicit Grabber( string filenameImg , int offsetImgAdr , string repleaceFile , QWidget *parent = 0);
		~Grabber();
		void compress();

	private slots:
		void on_btnGrab_clicked();
		void on_btnGrayPal_clicked();
		void on_btnNextPicture_clicked();
		void on_btnNextPalPicture_clicked();
		void on_btnLoad_clicked();
		void on_btnLoadedImg_clicked();
		void on_btnActualImg_clicked();
		void on_btnRepleace_clicked();
		void on_btnSaveImg_clicked();

		void changeWidthImage( const QString &sizeImage );
		void changeHeightImage( const QString &heightImage );
		void changeOffsetImg( const QString &heightImage );
		void changeOffsetFileLineEdit( const QString &heightImage );
		void changeScrHeight( int value );
		void changeScrWidth( int value );
		void changeScrOffset( int value );
		void changeOffsetFile( int value );
};







