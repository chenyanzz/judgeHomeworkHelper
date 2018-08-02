// Created by cy on 2018/07/28
// 
// 

#pragma once
#include <QWidget>
#include <QString>
#include <QLabel>
#include <functional>

class WaitPopupWindow : public QWidget{
	Q_OBJECT
public:
	WaitPopupWindow(QString msg, QWidget* parent=nullptr);

	static void CreateAndWaitForEnd(QString msg, std::function<void()> f);

protected:
	QLabel * label_process;
};
