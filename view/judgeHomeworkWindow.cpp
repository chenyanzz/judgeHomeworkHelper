#include "judgeHomeworkWindow.h"

#include <QInputDialog>
#include <QStringListModel>
#include "loginWindow.h"
#include "model/zhixueHelper.h"
#include "model/HomeworkModel.h"
#include "model/WebHelper.h"
#include <QDebug>

#pragma execution_character_set("utf-8")

judgeHomeworkWindow::judgeHomeworkWindow(QWidget* parent)
	: QWidget(parent) {
	//生成对象
	auto layout_main = new QHBoxLayout(this);
	tree_homework = new QTreeWidget(this);
	auto layout_right = new QVBoxLayout(this);
	auto label_1 = new QLabel("标答关键词", this);
	text_rightAnswer = new QTextBrowser(this);
	auto label_2 = new QLabel("学生答案", this);
	text_stuAnswer = new QTextBrowser(this);

	//设置布局
	this->setLayout(layout_main);
	layout_main->addWidget(tree_homework);
	layout_main->addLayout(layout_right);
	layout_right->addWidget(label_1);
	layout_right->addWidget(text_rightAnswer);
	layout_right->addWidget(label_2);
	layout_right->addWidget(text_stuAnswer);

	//设置属性
	tree_homework->setHeaderLabel("作业列表");
	tree_homework->setMaximumWidth(400);
	layout_main->setStretch(0, 1);
	layout_main->setStretch(1, 2);

	//设置消息响应
	connect(tree_homework, SIGNAL(itemExpanded(QTreeWidgetItem*)), this, SLOT(collapseOthers(QTreeWidgetItem*)));
	connect(tree_homework, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(showHomework(QTreeWidgetItem*,int)));
	connect(tree_homework, SIGNAL(itemExpanded(QTreeWidgetItem*)), this, SLOT(collapseOthers(QTreeWidgetItem*)));
	this->hide();
}

void judgeHomeworkWindow::diff() {
	QString color = "red";

	text_rightAnswer->clear();
	QString keywords[] = {"输导", "传输"};
	for(QString keyword : keywords) {
		QString str = QString(R"(<span style="color:%1;"> %2 </span>)").arg(color).arg(keyword);
		text_rightAnswer->insertHtml(str);
	}

	text_stuAnswer->clear();
	QString answer = "哈哈哈哈哈哈哈哈A是输导组织哈哈哈哈哈哈哈哈1";
	QString html = answer;
	for(QString keyword : keywords) {
		int start = 0;
		QString str = QString(R"(<span style="color:%1;"> %2 </span>)").arg(color).arg(keyword);
		html.replace(keyword, str, Qt::CaseSensitive);
	}
	text_stuAnswer->setHtml(html);
}

void judgeHomeworkWindow::init() {
	auto login_window = qobject_cast<loginWindow *>(sender());
	QString username = login_window->ui.lineEdit_username->text();
	QString password = login_window->ui.lineEdit_password->text();
	if(!zxhelper.do_login(username, password)) {
		zxhelper = zhixueHelper(); //重新加载http，防止出现验证码
		login_window->show();
	}
	login_window->hide();

	this->show();
	setHomeworkTreeData();
}

void judgeHomeworkWindow::setHomeworkTreeData() {
	homeworks = zxhelper.parseHomeworkList();

	if(homeworks.size() == 0) {
		auto root = new QTreeWidgetItem;
		root->setText(0, "[空]");
		root->setToolTip(0, "还无人提交");
		root->setDisabled(true);
		tree_homework->addTopLevelItem(root);
		return;
	}
	for(auto& hw : homeworks) {
		auto root = new QTreeWidgetItem;
		root->setText(0, hw.name);
		root->setToolTip(0, hw.name);
		root->setBackgroundColor(0, QColor::fromRgb(63, 72, 204, 0));
		tree_homework->addTopLevelItem(root);

		if(hw.classes.size() == 0) {
			auto item = new QTreeWidgetItem;
			item->setText(0, "[空]");
			item->setToolTip(0, "还无人提交");
			item->setDisabled(true);
			root->addChild(item);
			continue;
		}
		for(auto& cls : hw.classes) {
			auto item = new QTreeWidgetItem;
			item->setText(0, cls.name);
			item->setToolTip(0, cls.name);
			item->setBackgroundColor(0, QColor::fromRgb(14, 209, 69, 0));
			root->addChild(item);
		}
	}
}

void judgeHomeworkWindow::collapseOthers(QTreeWidgetItem* item) {
	disconnect(tree_homework, SIGNAL(itemExpanded(QTreeWidgetItem*)), this, SLOT(collapseOthers(QTreeWidgetItem*)));
	tree_homework->collapseAll();
	item->setExpanded(true);
	connect(tree_homework, SIGNAL(itemExpanded(QTreeWidgetItem*)), this, SLOT(collapseOthers(QTreeWidgetItem*)));
	int index = tree_homework->indexOfTopLevelItem(item);
	current_homework = homeworks[index];
	zxhelper.parseHomework(current_homework);

	zxhelper.setOneMark(current_homework, 0, 0, 4, 1);
}

void judgeHomeworkWindow::showHomework(QTreeWidgetItem* item, int column) {
	if(item->childCount() != 0)return;
	auto parent = item->parent();
	int cls_index = parent->indexOfChild(item);
	currrent_class = current_homework.classes[cls_index];
}
