#pragma once

#include <QWidget>
#include <QTextBrowser>
#include <QTreeWidget>
#include <QVector>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QtWebEngineWidgets/QtWebEngineWidgets>
#include "model/homeworkModel.h"
#include "model/zhixueHelper.h"
#include <QPushButton>
#include <QLabel>
#include "browseSectionWindow.h"

class judgeHomeworkWindow: public QWidget {
Q_OBJECT
public:
	judgeHomeworkWindow(QWidget* parent = Q_NULLPTR);

	friend browseSectionWindow;

public Q_SLOTS:
	void init();

	void goToPrev();
	void goToNext();

	void diff();

	//关闭其他的
	void collapseOthers(QTreeWidgetItem* except);
	void showHomework(QTreeWidgetItem* item, int column);

	void onItemCollapsed(QTreeWidgetItem* item);
	void correctValue(double val);

	void saveMark();
	void uploadMarks();

	void showSectionWindow();
	void saveKeywords();

	void clearRightWindow();
protected:

	QLabel* label_class;
	QLabel* label_sec;
	QLabel* label_name;
	QLabel* label_process;
	QWebEngineView* webview_porcess;
	QPushButton* btn_prev;
	QPushButton* btn_next;
	QTreeWidget* tree_homework;
	QPushButton* btn_upload;
	QWebEngineView* webview_section;
	QWebEngineView* webview_rightAnswer;
	QWebEngineView* webview_keyword;
	QWebEngineView* webview_stuAnswer;
	QDoubleSpinBox* input_mark;
	QLabel* label_fullmark;
	QPushButton* btn_showSection;
	QPushButton* btn_savemark;

	zhixueHelper zxhelper;
	QVector<Homework> homeworks;
	Homework* current_homework;
	QMap<QString, QVector<QStringList>> hw_keywords;
	int class_index;
	int student_index;
	int section_index;
	bool selected_homework;
	QVector<UploadMarkPack> marksToUpload;

	const QString keywords_data_file_name="hw_keywords.dat";

	void setHomeworkTreeData();
	void showAnswer(); //根据三个index变量显示对应的答案数据

	void keyPressEvent(QKeyEvent* event) override;

	void readKeywords();
};
