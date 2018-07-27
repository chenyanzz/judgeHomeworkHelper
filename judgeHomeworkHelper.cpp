#include "judgeHomeworkHelper.h"

judgeHomeworkHelper::judgeHomeworkHelper(QWidget* parent)
	: QMainWindow(parent) {
	auto layout_main = new QHBoxLayout;
	tree_homework = new QTreeWidget;
	auto layout_right = new QVBoxLayout;
	auto label_1 = new QLabel("标答关键词");
	text_rightAnswer = new QTextBrowser;
	auto label_2 = new QLabel("学生答案");
	text_stuAnswer = new QTextBrowser;

	this->setLayout(layout_main);
	layout_main->addWidget(tree_homework);
	layout_main->addLayout(layout_right);

	layout_right->addWidget(label_1);
	layout_right->addWidget(text_rightAnswer);
	layout_right->addWidget(label_2);
	layout_right->addWidget(text_stuAnswer);
}

void judgeHomeworkHelper::diff() {
	QString color = "red";

	text_rightAnswer->clear();
	QString keywords[] = {QString::fromWCharArray(L"输导"), QString::fromWCharArray(L"传输")};
	for(QString keyword : keywords) {
		QString str = QString(R"(<span style="color:%1;"> %2 </span>)").arg(color).arg(keyword);
		text_rightAnswer->insertHtml(str);
	}

	text_stuAnswer->clear();
	QString answer = QString::fromWCharArray(L"哈哈哈哈哈哈哈哈A是输导组织哈哈哈哈哈哈哈哈1");
	QString html = answer;
	for(QString keyword : keywords) {
		int start = 0;
		QString str = QString(R"(<span style="color:%1;"> %2 </span>)").arg(color).arg(keyword);
		html.replace(keyword, str, Qt::CaseSensitive);
	}
	text_stuAnswer->setHtml(html);
}
