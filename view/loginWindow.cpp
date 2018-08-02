#include "loginWindow.h"
#include "model/WebHelper.h"
#include <QStringListModel>
#include <QFile>

loginWindow::loginWindow(QWidget* parent) : QDialog(parent) {
	ui.setupUi(this);
	connect(ui.okButton, SIGNAL(clicked()), this, SLOT(saveUserData()));
	loadUserData();
	ui.lineEdit_username->setText(username);
	ui.lineEdit_password->setText(password);
}

void loginWindow::loadUserData() {
	if(!QFile::exists(user_data_file_name))return;
	QFile f(user_data_file_name);
	f.open(QFile::ReadOnly);
	if (f.size()==0)return;
	QDataStream s;
	s.setDevice(&f);
	s >> username >> password;
}

void loginWindow::saveUserData() {
	username = ui.lineEdit_username->text();
	password = ui.lineEdit_password->text();

	QFile f(user_data_file_name);
	f.open(QFile::Truncate | QFile::WriteOnly);
	if (!ui.checkBox_keepData->isChecked()) {
		accept();
		return;
	}
	QDataStream s;
	s.setDevice(&f);
	s << username << password;
	f.close();

	accept();
}
