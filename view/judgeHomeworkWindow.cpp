#include "judgeHomeworkWindow.h"

#include <QInputDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include "loginWindow.h"
#include "model/zhixueHelper.h"
#include "model/HomeworkModel.h"
#include "model/WebHelper.h"
#include <QDebug>
#include <QMessageBox>

#define DECL_HW_VAR	\
	auto& hw = *current_homework;	\
	auto& cls = hw.classes[class_index];	\
	auto& stu = cls.students[student_index];	\
	auto& ans = stu.answers[section_index];	\
	int sec_id = ans.seqNumber;	\
	auto& sec = hw.sections[sec_id]

judgeHomeworkWindow::judgeHomeworkWindow(QWidget* parent)
	: QWidget(parent) {
	//生成对象
	auto layout_main = new QHBoxLayout;
	auto layout_right = new QVBoxLayout;
	tree_homework = new QTreeWidget;
	btn_upload = new QPushButton("上传得分");
	auto layout_left = new QVBoxLayout;
	auto layout_info = new QHBoxLayout;
	label_class = new QLabel;
	label_sec = new QLabel;
	label_name = new QLabel;
	label_process = new QLabel;
	// webview_porcess = new QWebEngineView;
	btn_prev = new QPushButton("上一个");
	btn_next = new QPushButton("下一个");
	webview_section = new QWebEngineView;
	webview_rightAnswer = new QWebEngineView;
	webview_keyword = new QWebEngineView;
	webview_stuAnswer = new QWebEngineView;
	auto layout_input = new QHBoxLayout;
	input_mark = new QDoubleSpinBox;
	label_fullmark = new QLabel;
	btn_showSection = new QPushButton("查看题目/修改关键词");
	btn_savemark = new QPushButton("保存成绩");

	//设置布局
	this->setLayout(layout_main);
	layout_main->addLayout(layout_left, 1);
	layout_main->addLayout(layout_right, 3);

	layout_left->addWidget(tree_homework);
	layout_left->addWidget(btn_upload);

	layout_right->addLayout(layout_info);
	// layout_right->addWidget(new QLabel("题目"));
	// layout_right->addWidget(webview_section, 3);
	// layout_right->addWidget(new QLabel("标准答案"));
	// layout_right->addWidget(webview_rightAnswer, 1);
	layout_right->addWidget(new QLabel("关键词"));
	layout_right->addWidget(webview_keyword, 1);
	layout_right->addWidget(new QLabel("学生答案"));
	layout_right->addWidget(webview_stuAnswer, 3);
	layout_right->addLayout(layout_input);

	layout_info->addWidget(label_class);
	layout_info->addWidget(label_sec);
	layout_info->addWidget(label_name);
	layout_info->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
	layout_info->addWidget(label_process);
	// layout_info->addWidget(webview_porcess);
	layout_info->addWidget(btn_prev);
	layout_info->addWidget(btn_next);

	layout_input->addWidget(input_mark);
	layout_input->addWidget(label_fullmark);
	layout_input->addWidget(btn_savemark);

	//设置属性
	tree_homework->setHeaderLabel("作业列表");
	input_mark->setSingleStep(0.5);
	input_mark->setDecimals(1);
	input_mark->installEventFilter(this);

	//设置消息响应
	connect(tree_homework, SIGNAL(itemExpanded(QTreeWidgetItem*)), this, SLOT(collapseOthers(QTreeWidgetItem*)));
	connect(tree_homework, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(showHomework(QTreeWidgetItem*,int)));
	connect(btn_upload, SIGNAL(clicked()), this, SLOT(uploadMarks()));
	connect(btn_prev, SIGNAL(clicked()), this, SLOT(goToPrev()));
	connect(btn_next, SIGNAL(clicked()), this, SLOT(goToNext()));
	connect(tree_homework, SIGNAL(itemExpanded(QTreeWidgetItem*)), this, SLOT(collapseOthers(QTreeWidgetItem*)));
	connect(tree_homework, SIGNAL(itemCollapsed(QTreeWidgetItem*)), this, SLOT(onItemCollapsed(QTreeWidgetItem*)));
	connect(input_mark, SIGNAL(valueChanged(double)), this, SLOT(correctValue(double)));
	connect(btn_showSection, SIGNAL(clicked()), this, SLOT(showSectionWindow()));
	connect(btn_savemark, SIGNAL(clicked()), this, SLOT(saveMark()));
	this->hide();
}

static QString getColor(int& p) {
	static QStringList colors = {"Aquamarine", "Chartreuse", "green", "CornflowerBlue", "gold", "Cyan", "red", "DodgerBlue"};
	QString color = colors[p];
	p++;
	if(p >= colors.size())p = 0;
	return color;
}

void judgeHomeworkWindow::diff() {
	DECL_HW_VAR;
	if(ans.isPic)return;
	if(ans.seqNumber != 21)return;
	QStringList keywords = {"肌肉", "结缔", "神经", "上皮"};

	QString html_keywords;
	QString html_ans = ans.text;
	int p = 0;
	for(QString keyword : keywords) {
		auto color = getColor(p);
		QString str = QString(R"(<span style="background-color:%1;margin-left:10px;"> %2 </span>)").arg(color).arg(keyword);
		html_keywords.append(str);
		html_ans.replace(keyword, str, Qt::CaseSensitive);

	}
	webview_keyword->setHtml(html_keywords);
	webview_stuAnswer->setHtml(html_ans);
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
	selected_homework = false;
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

void judgeHomeworkWindow::collapseOthers(QTreeWidgetItem* except) {
	disconnect(tree_homework, SIGNAL(itemExpanded(QTreeWidgetItem*)), this, SLOT(collapseOthers(QTreeWidgetItem*)));
	tree_homework->collapseAll();
	except->setExpanded(true);
	connect(tree_homework, SIGNAL(itemExpanded(QTreeWidgetItem*)), this, SLOT(collapseOthers(QTreeWidgetItem*)));
	int index = tree_homework->indexOfTopLevelItem(except);
	current_homework = &homeworks[index];
	zxhelper.parseHomework(*current_homework);
	selected_homework = true;

	// zxhelper.uploadOneMark(current_homework, 0, 0, 4, 1);
}

void judgeHomeworkWindow::showHomework(QTreeWidgetItem* item, int column) {
	if(item->childCount() != 0)return;
	auto parent = item->parent();
	class_index = parent->indexOfChild(item);
	section_index = 0;
	student_index = 0;
	DECL_HW_VAR;
	if(ans.hasMarked)goToNext();
	showAnswer();
}

void judgeHomeworkWindow::onItemCollapsed(QTreeWidgetItem* item) {
	for(int i = 0; i < tree_homework->topLevelItemCount(); i++) {
		if(tree_homework->topLevelItem(i)->isExpanded())return; //有展开就返回
	}
	selected_homework = false;
}

void judgeHomeworkWindow::correctValue(double val) {
	int i = val / 0.5;
	double d = i * 0.5;
	input_mark->setValue(d);
}

void judgeHomeworkWindow::goToPrev() {
	if(!selected_homework)return;
	saveMark();
	DECL_HW_VAR;
	student_index--;
	if(student_index < 0) {
		student_index = cls.students.size() - 1;
		section_index--;
	}
	if(section_index < 0) {
		section_index = stu.answers.size() - 1;
		class_index--;
	}
	if(class_index < 0) {
		student_index = 0;
		section_index = 0;
		class_index = 0;
		QMessageBox::information(this, "提示", "已达开始，没有上一个了！");
	}
	showAnswer();
}

void judgeHomeworkWindow::goToNext() {
	if(!selected_homework)return;
	saveMark();
	DECL_HW_VAR;
	student_index++;
	if(student_index >= cls.students.size()) {
		student_index = 0;
		section_index++;
	}
	if(section_index >= stu.answers.size()) {
		section_index = 0;
		class_index++;
	}
	if(class_index >= hw.classes.size()) {
		student_index = cls.students.size() - 1;
		section_index = stu.answers.size() - 1;
		class_index = hw.classes.size() - 1;
		QMessageBox::information(this, "提示", "已达最后，没有下一个了！");
		return;
	}

	{
		DECL_HW_VAR;
		if(ans.hasMarked)goToNext();
	}

	showAnswer();
}

void judgeHomeworkWindow::showAnswer() {
	DECL_HW_VAR;
	label_class->setText(cls.name);
	label_sec->setText(QString("第%1题").arg(sec_id));
	label_name->setText(stu.name);
	webview_section->setHtml(sec.rawHtml);
	webview_rightAnswer->setHtml(sec.answerHtml);
	label_fullmark->setText(QString("(共%1分)").arg(sec.fullMark));

	if(ans.hasMarked) {
		input_mark->setSuffix("    (此题已判)");
		input_mark->setValue(ans.mark);
	} else {
		input_mark->setSuffix("");
		input_mark->setValue(0);
	}

	QString html_ans;
	if(ans.isPic) {
		for(auto& url : ans.pic_urls) {
			html_ans.append(QString(R"(<img src="%1"/><br>)").arg(url.toHtmlEscaped()));
		}
	} else {
		html_ans = ans.text.toHtmlEscaped();
		html_ans.replace("\n", "<br>");
	}
	diff();

	input_mark->setRange(0, sec.fullMark);
	input_mark->setFocus();
	input_mark->selectAll();
}

void judgeHomeworkWindow::keyPressEvent(QKeyEvent* event) {
	switch(event->key()) {
	case Qt::Key_Enter:
	case Qt::Key_Return:
		goToNext();
		break;
	}
	QWidget::keyPressEvent(event);
}

void judgeHomeworkWindow::saveMark() {
	DECL_HW_VAR;
	double mark = input_mark->value();
	if(ans.hasMarked && ans.mark == mark)return;
	UploadMarkPack p;
	p.homework = current_homework;
	p.class_index = class_index;
	p.section_index = section_index;
	p.student_index = student_index;
	p.mark = mark;
	marksToUpload.push_back(p);
	ans.hasMarked = true;
}

void judgeHomeworkWindow::uploadMarks() {
	bool ret;
	if(zxhelper.uploadMarks(marksToUpload)) {
		marksToUpload.clear();
		QMessageBox::information(this, "上传成功", "上传成功！");
	} else {
		QMessageBox::information(this, "上传失败", zxhelper.err + "\n请重试");
	}
}

void judgeHomeworkWindow::showSectionWindow() {
	browseSectionWindow w(this);
	w.setWindowModality(Qt::ApplicationModal);
	w.show();
}

#undef DECL_HW_VAR
