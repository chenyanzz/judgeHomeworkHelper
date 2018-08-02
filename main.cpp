#include <QtWidgets/QApplication>
#include <QDebug>
#include <QTextCodec>
#include <QMessageBox>
#include <QObject>

#include "model/WebHelper.h"
#include "model/zhixueHelper.h"
#include "view/judgeHomeworkWindow.h"
#include "view/loginWindow.h"
#include "view/WaitPopupWindow.h"

char username[] = "zxt2434473";
char password[] = "2019zhongkao";

#include <view/WaitPopupWindow.h>

int main(int argc, char* argv[]) {
	QApplication a(argc, argv);

	loginWindow login_window;
	judgeHomeworkWindow judge_homework_window;
	QObject::connect(&login_window, SIGNAL(accepted()), &judge_homework_window, SLOT(init()));
	login_window.show();

	// login_window.ui.lineEdit_username->setText(username);
	// login_window.ui.lineEdit_password->setText(password);
	// login_window.ui.okButton->click();


	return a.exec();
}
