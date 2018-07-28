#pragma once

#include <QWidget>
#include <QTextBrowser>
#include <QTreeWidget>
#include "model/homeworkModel.h"
#include <QVector>

class judgeHomeworkWindow : public QWidget
{
	Q_OBJECT
public:
	judgeHomeworkWindow(QWidget *parent = Q_NULLPTR);

public Q_SLOTS:
	void diff();
	void init();

	//上色这个标签，关闭其他的
	void collapseOthers(QTreeWidgetItem* item);
	void showHomework(QTreeWidgetItem* item, int column);

private:
	QTreeWidget * tree_homework;
	QTextBrowser* text_rightAnswer;
	QTextBrowser* text_stuAnswer;

	QVector<Homework> homeworks;
	Homework current_homework;
	Clazz currrent_class;

	void setHomeworkTreeData();
};
