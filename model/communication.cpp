//����C��goͨ��

#include <QDebug>
#include <QMessageBox>
#include "WebHelper.h"
#include "model.h"

void do_lib_init() {
	if(!WebHelper::initLib()) {
		QMessageBox::critical(nullptr, "���ش���", WebHelper::libWebHelper.errorString());
		exit(-1);
	}
}

bool do_login(const QString& _username, const QString& _password) {
	QString err = WebHelper::login(QString2Ascii(_username).c_str(), QString2Ascii(_password).c_str());
	if(err != "��¼�ɹ���") {
		QMessageBox::critical(nullptr, "��¼����", err);
		return false;
	}
	return true;
}