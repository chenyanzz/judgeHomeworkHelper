// Created by cy on 2018/07/28
// 
// 

#pragma once
#include <QWidget>
#include <QString>

class WaitPopupWindow : public QWidget{
public:
	WaitPopupWindow(QString msg, QWidget* parent=nullptr);
};
