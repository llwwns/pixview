#include <QApplication>
#include "mainwindow.h"
// #include <qt_windows.h>
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
	if(argc>1)
	{
		MainWindow w;
        w.show();
		return app.exec();
	}else
	{
		MainWindow w;
		w.show();
		return app.exec();
	}

}

