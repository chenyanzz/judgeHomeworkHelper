#include "browseSectionWindow.h"
#include "judgeHomeworkWindow.h"

browseSectionWindow::browseSectionWindow(judgeHomeworkWindow* parent): QWidget((QWidget*)parent), hw(parent->current_homework) {
	ui.setupUi(this);
	connect(ui.btn_save, SIGNAL(clicked()), this, SLOT());
	connect(ui.btn_prev, SIGNAL(clicked()), this, SLOT());
	connect(ui.btn_next, SIGNAL(clicked()), this, SLOT());
	showSection(0);
}

void browseSectionWindow::showSection(int index) {
	int sec_id = hw->classes[0].students[0].answers[index].seqNumber;
	ui.label_title->setText(QString("第%1题").arg(sec_id));
	ui.label_process->setText(QString("第%1题/共%2题").arg(index + 1).arg(hw->classes[0].students[0].answers.size()));
	ui.webview_rawHtml->setHtml(hw->sections[sec_id].rawHtml);
	ui.webview_correctAnswer->setHtml(hw->sections[sec_id].answerHtml);
}
