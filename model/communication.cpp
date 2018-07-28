//处理C与go通信

#include <QDebug>
#include <QMessageBox>
#include "WebHelper.h"
#include "model.h"

void do_lib_init() {
	if(!WebHelper::initLib()) {
		QMessageBox::critical(nullptr, "加载错误", WebHelper::libWebHelper.errorString());
		exit(-1);
	}
}

bool do_login(const QString& _username, const QString& _password) {
	QString err = WebHelper::login(QString2Ascii(_username).c_str(), QString2Ascii(_password).c_str());
	if(err != "登录成功！") {
		QMessageBox::critical(nullptr, "登录错误", err);
		return false;
	}
	return true;
}