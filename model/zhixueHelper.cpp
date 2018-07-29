﻿// Created by cy on 2018/07/28
// 
// 解析json成结构体

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QApplication>
#include <QThread>
#include <QMessageBox>
#include <QDebug>
#include <fstream>
#include "view/WaitPopupWindow.h"
#include "homeworkModel.h"
#include "WebHelper.h"
#include "zhixueHelper.h"

zhixueHelper::zhixueHelper() {
	lib_init();
}

zhixueHelper::~zhixueHelper() {
	WebHelper::closeLib();
}

void zhixueHelper::lib_init() {
	if(!WebHelper::initLib()) {
		QMessageBox::critical(nullptr, "加载错误", WebHelper::libWebHelper.errorString());
		exit(-1);
	}
}

bool zhixueHelper::do_login(const QString& _username, const QString& _password) {
	QString err = WebHelper::login(QString2Ascii(_username).c_str(), QString2Ascii(_password).c_str());
	if(err != "登录成功！") {
		QMessageBox::critical(nullptr, "登录错误", err);
		return false;
	}
	return true;
}

QJsonDocument zhixueHelper::parseJson(const QString& s) {
	QJsonParseError err;
	QJsonDocument json = QJsonDocument::fromJson(s.toUtf8(), &err);
	if(err.error != QJsonParseError::NoError) {
		QMessageBox::critical(nullptr, "json解析错误", err.errorString());
		exit(-1);
	}
	return json;
}

Answer zhixueHelper::parseOneAnswer(const QJsonObject& o) {
	Answer ans;
	// qDebug() << o;
	ans.hasMarked = o["resultType"].toString() != "UNKNOWN";
	auto ans_obj = parseJson(o["answer"].toString()).object();
	ans.isPic = ans_obj.contains("pic");
	if(ans.isPic)ans.ans = ans_obj["pic"].toString();
	else ans.ans = ans_obj["text"].toString();
	ans.mark = o["markScore"].toDouble();
	ans.topic = o["topic"].toObject();
	ans.topic.remove("score");
	ans.topic.remove("subTopicSort");
	ans.seqNumber = ans.topic["seqNO"].toInt();
	ans.topicPackId = o["answerRecord"].toObject()["topicPackage"].toObject()["id"].toString();
	return ans;
}

//for one stu in one homework
Student zhixueHelper::parseOneStu(const QJsonObject& stu) {
	Student st;
	// qDebug() << stu;
	st.name = stu["userName"].toString();
	auto answers = stu["deatils"].toArray();
	st.userId = answers[0].toObject()["answerRecord"].toObject()["user"].toObject()["id"].toString();
	QVector<int> subjective_seqs;
	for(int i = 0; i < answers.size(); i++) {
		auto ans = answers[i];
		if(ans.toObject()["topic"].toObject()["isSubjective"].toBool() == true) {
			subjective_seqs.push_back(i);
		}
	}
	int seq_number = subjective_seqs[0];
	auto& student_answer = st.answers;
	for(int p : subjective_seqs) {
		Answer ans = parseOneAnswer(answers[p].toObject());
		student_answer.push_back(ans);
	}
	return st;
}

QVector<Homework> zhixueHelper::parseHomeworkList_caseEnd(bool hasEnded) {
	QString s = WebHelper::getHomeworkList(hasEnded ? "true" : "false");

	QJsonDocument hwData = parseJson(s);

	// qDebug() << hwData;

	QVector<Homework> homeworks;

	for(QJsonValue val : hwData.array()) {
		auto o = val.toObject();
		QString hwname = o["name"].toString();
		QString clsname = o["class"].toString();
		Clazz cls;
		cls.name = clsname;
		cls.url = o["url"].toString();
		cls.homeworkId = o["homeworkId"].toString();
		if(cls.url.isEmpty())continue;
		auto it = std::find_if(homeworks.begin(), homeworks.end(), [&](Homework& hw) { return hw.name == hwname; });
		if(it == homeworks.end()) {
			Homework hw;
			hw.hasEnded = hasEnded;
			hw.name = hwname;
			hw.classes.push_back(cls);
			homeworks.push_back(hw);
		} else {
			Homework& hw = *it; //一定要传引用
			hw.classes.push_back(cls);
		}
		// qDebug() << "size of homeworks[0].classes: " << homeworks[0].classes.size();
	}
	return homeworks;
}

template <typename Function>
void zhixueHelper::CreateAndWaitForEnd(Function f) {
	WaitPopupWindow p("正在加载智学网数据");
	p.show();
	QThread* thread = QThread::create(f);
	thread->start();
	while(!thread->isFinished()) {
		QApplication::processEvents();
	}
	delete thread;
	p.hide();
}

QVector<Homework> zhixueHelper::parseHomeworkList() {
	QVector<Homework> hwlist;

	CreateAndWaitForEnd([&]() {
		hwlist.append(parseHomeworkList_caseEnd(true));
		hwlist.append(parseHomeworkList_caseEnd(false));
	});
	return hwlist;
}

void zhixueHelper::parseHomework(Homework& homework) {
	CreateAndWaitForEnd([&]() {
		for(auto& clazz : homework.classes) {
			QString hwdata = WebHelper::getHomeworkFromUrl(QString2Ascii(clazz.url).c_str());
			QJsonDocument clsData = parseJson(hwdata);
			for(auto v : clsData.array()) {
				clazz.students.push_back(parseOneStu(v.toObject()));
			}
		}
	});
}

bool zhixueHelper::setOneMark(Homework& homework, int clazz_index,
	int student_index, int answer_index ,int mark) {
	auto& clazz = homework.classes[clazz_index];
	auto& student = clazz.students[student_index];
	auto& ans = student.answers[answer_index];
	if (mark != -1)ans.mark = mark;

	QJsonObject ret;
	ret.insert("topic", ans.topic);
	ret.insert("markScore", ans.mark);

	QString type;
	// if (ans.mark == 0)type = "WRONG";
	// else if (homework.sections[ans.answer_index].fullMark == ans.mark)type = "CORRECT";
	// else type = "HALFCORRECT";
	if (ans.mark == 0)type = "WRONG";
	else if (2 == ans.mark)type = "CORRECT";
	else type = "HALFCORRECT";

	ret.insert("resultType", type);
	std::string json =  QString2Ascii(QString::fromUtf8(QJsonDocument(ret).toJson()));
	QString s = WebHelper::saveMark(QString2Ascii(student.userId).c_str(), QString2Ascii(clazz.homeworkId).c_str(), QString2Ascii(ans.topicPackId).c_str(), json.c_str());

	if(s!="提交成功！") {
		QMessageBox::warning(nullptr, "成绩上传错误", s);
		return false;
	}

	return true;
}
