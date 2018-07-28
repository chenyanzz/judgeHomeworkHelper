#include <QtWidgets/QApplication>
#include <QDebug>
#include <QTextCodec>
#include <QMessageBox>
#include <QObject>

#include "model/WebHelper.h"
#include "model/model.h"
#include "view/judgeHomeworkWindow.h"
#include "view/loginWindow.h"

char username[] = "zxt2434473";
char password[] = "2019zhongkao";

int main(int argc, char* argv[]) {
	QApplication a(argc, argv);
	do_lib_init();
	loginWindow login_window;
	judgeHomeworkWindow judge_homework_window;

	login_window.ui.lineEdit_username->setText(username);
	login_window.ui.lineEdit_password->setText(password);

	QObject::connect(&login_window, SIGNAL(accepted()), &judge_homework_window, SLOT(init()));
	login_window.show();

	return a.exec();
}