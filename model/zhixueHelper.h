#pragma once

#include <QString>
#include <QVector>
#include "HomeworkModel.h"

class zhixueHelper {
public:
	zhixueHelper();
	~zhixueHelper();

	bool do_login(const QString& username, const QString& password);

	//只返回可以判的
	QVector<Homework> parseHomeworkList();
	void parseHomework(Homework& homework);
	bool uploadOneMark(Homework& homework, int clazz_index, int student_index, int answer_index, double mark = -1);
	bool uploadOneMark(const UploadMarkPack& p);
	bool uploadMarks(QVector<UploadMarkPack>& packs);
	QString err;

private:
	void lib_init();
	QJsonDocument parseJson(const QString& s);
	Answer parseOneAnswer(const QJsonObject& o);
	Student parseOneStu(const QJsonObject& stu);
	QVector<Homework> parseHomeworkList_caseEnd(bool hasEnded);
};
