#include "Tsukuyomi.h"
#include <QtWidgets/QApplication>
#include <QStyleFactory>  

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QStringList list = QStyleFactory::keys();
	QApplication::setStyle(QStyleFactory::create("Fusion"));
	QApplication::setPalette(QApplication::style()->standardPalette());
	Tsukuyomi w;
	w.show();
	return a.exec();
}
