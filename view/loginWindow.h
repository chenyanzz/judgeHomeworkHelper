// Created by cy on 2018/07/28
// 
// 

#pragma once

#include "ui_LoginWindow.h"
#include <QDialog>
#include <QString>

class loginWindow: public QDialog {
Q_OBJECT
public:
	loginWindow(QWidget* parent = nullptr);
	Ui_loginDialog ui;
	const QString user_data_file_name = "user.dat";
	QString username, password;

	void loadUserData();
public Q_SLOTS:
	void saveUserData();
};
