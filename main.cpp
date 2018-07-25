#include "judgeHomeworkHelper.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	judgeHomeworkHelper w;
	w.show();
	return a.exec();
}
