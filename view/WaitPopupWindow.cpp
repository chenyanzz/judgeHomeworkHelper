#include "WaitPopupWindow.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QMovie>
#include <QIcon>

WaitPopupWindow::WaitPopupWindow(QString msg, QWidget* parent) : QWidget(parent) {
	this->setWindowTitle("请等待");

	QLabel* label_text = new QLabel(msg, parent);
	auto layout = new QHBoxLayout(this);
	this->setLayout(layout);

	QLabel* label_wait = new QLabel();
	QMovie* movie = new QMovie("./assets/loading.gif");
	label_wait->setMovie(movie);
	movie->start();

	layout->addWidget(label_text);
	layout->addWidget(label_wait);

	QFont font1;
	font1.setFamily(QString::fromUtf8("\351\273\221\344\275\223"));
	font1.setPointSize(10);
	this->setFont(font1);
	this->setWindowModality(Qt::ApplicationModal);
	this->setFixedSize(window()->sizeHint());
}
