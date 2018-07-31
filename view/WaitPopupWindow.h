// Created by cy on 2018/07/28
// 
// 

#pragma once
#include <QWidget>
#include <QString>
#include <QLabel>

class WaitPopupWindow : public QWidget{
public:
	WaitPopupWindow(QString msg, QWidget* parent=nullptr);
protected:
	QLabel * label_process;
};
