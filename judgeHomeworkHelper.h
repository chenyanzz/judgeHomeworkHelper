#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_judgeHomeworkHelper.h"

class judgeHomeworkHelper : public QMainWindow
{
	Q_OBJECT

public:
	judgeHomeworkHelper(QWidget *parent = Q_NULLPTR);

private:
	Ui::judgeHomeworkHelperClass ui;
};
