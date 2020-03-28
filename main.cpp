#include <QApplication>
#include "imageview.h"
// #include <qt_windows.h>
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
	if(argc>1)
	{
		ImageView w;
        w.show();
		return app.exec();
	}else
	{
		ImageView w;
		w.show();
		return app.exec();
	}

}

