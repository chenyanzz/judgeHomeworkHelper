// Created by cy on 2018/07/31
// 
// 

#pragma once

#include <QWidget>
#include <QVector>
#include <QStringList>
#include "ui_browseSectionWindow.h"
#include "model/homeworkModel.h"

class browseSectionWindow: public QWidget {
Q_OBJECT
public:
	//由judgeHomeworkWindow保证每项作业map都有对应的key，且value.size==sec_size
	browseSectionWindow(class judgeHomeworkWindow* parent);

	void showSection(int index);

	void saveCurrent();

public Q_SLOTS:
	void goToPrev();
	void goToNext();
	void saveAll();
	void cancel();

Q_SIGNALS:
	void savedAll();

protected:
	judgeHomeworkWindow * judge_homework_window;
	int index = 0;
	int size_ans;
	QVector<QStringList> keywords;
	Homework* hw;
	Ui_browseSectionWindow ui;
};
