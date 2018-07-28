// Created by cy on 2018/07/28
// 
// 解析json成结构体

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QApplication>
#include <QThread>
#include <QMessageBox>
#include "homeworkModel.h"
#include "model/WebHelper.h"
#include "view/WaitPopupWindow.h"

QJsonDocument parseJson(const QString& s) {
	QJsonParseError err;
	QJsonDocument json = QJsonDocument::fromJson(s.toUtf8(), &err);
	if (err.error != QJsonParseError::NoError) {
		QMessageBox::critical(nullptr, "json解析错误", err.errorString());
		exit(-1);
	}
	return json;
}

Answer parseOneAnswer(const QJsonObject& o) {
	Answer ans;
	ans.hasMarked = o["resultType"].toString() != "UNKNOWN";
	auto ans_obj = parseJson(o["answer"].toString()).object();
	ans.isPic = ans_obj.contains("pic");
	if (ans.isPic)ans.ans = ans_obj["pic"].toString();
	else ans.ans = ans_obj["text"].toString();
	ans.mark = o["markScore"].toDouble();
	ans.seqNumber = o["topic"].toObject()["seqNO"].toInt();
	return ans;
}

//for one stu in one homework
Student parseOneStu(const QJsonObject& stu) {
	Student st;
	// qDebug() << stu;
	st.name = stu["userName"].toString();
	auto answers = stu["deatils"].toArray();
	QVector<int> subjective_seqs;
	for (int i = 0; i < answers.size(); i++) {
		auto ans = answers[i];
		if (ans.toObject()["topic"].toObject()["isSubjective"].toBool() == true) {
			subjective_seqs.push_back(i);
		}
	}
	int seq_number = subjective_seqs[0];
	auto& student_answer = st.answers;
	for (int p : subjective_seqs) {
		Answer ans = parseOneAnswer(answers[p].toObject());
		student_answer.push_back(ans);
	}
	return st;
}

QVector<Homework> parseHomeworkList_caseEnd(bool hasEnded) {
	QString s = WebHelper::getHomeworkList(hasEnded ? "true" : "false");

	QJsonDocument hwData = parseJson(s);
	// qDebug() << hwData;

	QVector<Homework> homeworks;
	for (QJsonValue val : hwData.array()) {
		QString hwname = val.toObject()["name"].toString();
		QString clsname = val.toObject()["class"].toString();
		Clazz cls;
		cls.name = clsname;
		cls.url = val.toObject()["url"].toString();
		if (cls.url.isEmpty())continue;
		auto it = std::find_if(homeworks.begin(), homeworks.end(), [&](Homework& hw) { return hw.name == hwname; });
		if (it == homeworks.end()) {
			Homework hw;
			hw.name = hwname;
			hw.classes.push_back(cls);
			homeworks.push_back(hw);
		}
		else {
			Homework& hw = *it; //一定要传引用
			hw.classes.push_back(cls);
		}
		// qDebug() << "size of homeworks[0].classes: " << homeworks[0].classes.size();
	}
	return homeworks;
}

template <typename Func>
void CreateAndWaitForEnd(Func f) {
	WaitPopupWindow p("正在加载智学网数据");
	p.show();
	QThread* thread = QThread::create(f);
	thread->start();
	while (!thread->isFinished()) {
		QApplication::processEvents();
	}
	delete thread;
	p.hide();
}

QVector<Homework> parseHomeworkList() {
	 QVector<Homework> hwlist;
 
	 CreateAndWaitForEnd([&]() {
	 	hwlist.append(parseHomeworkList_caseEnd(true));
	 	hwlist.append(parseHomeworkList_caseEnd(false));
	 });

	return hwlist;
}

void parseHomework(Homework& homework) {
	CreateAndWaitForEnd([&]() {
		for (auto& clazz : homework.classes) {
			// qDebug() << clazz;
			QString hwdata = WebHelper::getHomeworkFromUrl(QString2Ascii(clazz.url).c_str());
			QJsonDocument clsData = parseJson(hwdata);
			for (auto v : clsData.array()) {
				clazz.students.push_back(parseOneStu(v.toObject()));
			}
		}
	});
}
