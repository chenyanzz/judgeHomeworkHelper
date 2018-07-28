#include "WaitPopupWindow.h"
#include <QLabel>
#include <QHBoxLayout>

WaitPopupWindow::WaitPopupWindow(QString msg, QWidget* parent) :QWidget(parent){
	QFont font1;
	font1.setFamily(QString::fromUtf8("\351\273\221\344\275\223"));
	font1.setPointSize(10);
	this->setFont(font1);

	QLabel* label = new QLabel(msg, parent);
	auto layout = new QHBoxLayout(this);
	this->setLayout(layout);
	layout->addWidget(label);
	this->setWindowTitle("请等待...");
	this->setWindowModality(Qt::ApplicationModal);
}
