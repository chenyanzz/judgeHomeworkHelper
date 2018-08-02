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
	browseSectionWindow(class judgeHomeworkWindow* parent=nullptr);

	void showSection(int index);

protected:
	class judgeHomeworkWindow* judge_homework_window;
	Homework* hw;
	Ui_browseSectionWindow ui;
};
