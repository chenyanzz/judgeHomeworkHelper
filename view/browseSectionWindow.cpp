#include "browseSectionWindow.h"
#include "judgeHomeworkWindow.h"

browseSectionWindow::browseSectionWindow(judgeHomeworkWindow* parent):
	judge_homework_window(parent), hw(parent->current_homework), keywords(parent->hw_keywords[parent->current_homework->name]) {
	ui.setupUi(this);
	size_ans = hw->classes[0].students[0].answers.size();
	connect(ui.btn_save, SIGNAL(clicked()), this, SLOT(saveAll()));
	connect(ui.btn_prev, SIGNAL(clicked()), this, SLOT(goToPrev()));
	connect(ui.btn_next, SIGNAL(clicked()), this, SLOT(goToNext()));
	connect(ui.btn_cancel, SIGNAL(clicked()), this, SLOT(cancel()));
	this->setWindowTitle("查看题目/修改关键词");
	ui.btn_prev->setEnabled(false);
	showSection(0);
}

void browseSectionWindow::showSection(int index) {
	int sec_id = hw->classes[0].students[0].answers[index].seqNumber;
	ui.label_title->setText(QString("第%1题").arg(sec_id));
	ui.label_process->setText(QString("第%1题/共%2题").arg(index + 1).arg(size_ans));
	ui.webview_rawHtml->setHtml(hw->sections[sec_id].rawHtml);
	ui.webview_correctAnswer->setHtml(hw->sections[sec_id].answerHtml);
	QString text;
	for(auto s : keywords[index]) {
		text += s;
	}
	ui.text_keywords->setPlainText(text);
}

void browseSectionWindow::saveCurrent() {
	keywords[index] = ui.text_keywords->toPlainText().split(' ', QString::SkipEmptyParts);
}

void browseSectionWindow::goToPrev() {
	saveCurrent();
	index--;
	if(index < 0) {
		index = 0;
		ui.btn_prev->setEnabled(false);
		return;
	}
	showSection(index);
	ui.btn_next->setEnabled(true);
}

void browseSectionWindow::goToNext() {
	saveCurrent();
	index++;
	if(index >= size_ans) {
		index = size_ans - 1;
		ui.btn_next->setEnabled(false);
		return;
	}
	showSection(index);
	ui.btn_prev->setEnabled(true);
}

void browseSectionWindow::saveAll() {
	saveCurrent();
	judge_homework_window->hw_keywords[hw->name] = keywords;
	judge_homework_window->diff();
	emit savedAll();
	this->close();
}

void browseSectionWindow::cancel() {
	this->close();
}
