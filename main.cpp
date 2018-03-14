#include <QtGlobal>
#include <iostream>
#include <sstream>


#if QT_VERSION >= 0x050000
	#include <QtWidgets>
#else
	#include <QtGui>
#endif

#include "Grabber.hpp"

int main(int argc, char *argv[])
{
	string filenameImg = "";
	string repleaceFile = "";
	int offsetImgAdr = 0x41000;

	for( int param = 1 ; param < argc ; param++ )
	{
		if (string(argv[ param ]) == "-nameImg")
		{
			filenameImg = argv[++param];
		}
		else if (string(argv[ param ]) == "-offset")
		{
			istringstream(argv[++param]) >> hex >> offsetImgAdr;
		}
		else if (string(argv[ param ]) == "-repleaceFile")
		{
			repleaceFile = argv[++param];
		}
		else
		{
			cout << "Unknown program parameter: \"" << argv[ param ] << "\"" << endl;
			return -1;
		}
	}

	if( repleaceFile == "" )
	{
		cout << "Error mandotary parameter -repleaceFile is missing" << endl;
		return -3;
	}

	QApplication a(argc, argv);
	Grabber grabber( filenameImg , offsetImgAdr , repleaceFile );
	grabber.show();
	return a.exec();
}



