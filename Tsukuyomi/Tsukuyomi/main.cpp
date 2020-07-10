#include "Tsukuyomi.h"
#include <QtWidgets/QApplication>
#include <QStyleFactory>  
#include <QTextStream>


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QFile file(":/dark.qss");
	file.open(QFile::ReadOnly | QFile::Text);
	QTextStream stream(&file);
	a.setStyleSheet(stream.readAll());
	Tsukuyomi w;
	w.show();
	return a.exec();
}
