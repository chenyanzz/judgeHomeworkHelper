#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_judgeHomeworkHelper.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTextBrowser>
#include <QTreeWidget>

class judgeHomeworkHelper : public QMainWindow
{
	Q_OBJECT

public:
	judgeHomeworkHelper(QWidget *parent = Q_NULLPTR);

	void diff();

private:
	QTreeWidget * tree_homework;
	QTextBrowser* text_rightAnswer;
	QTextBrowser* text_stuAnswer;
};
