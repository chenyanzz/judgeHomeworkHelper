// Created by cy on 2018/07/28
// 
// 作业数据的结构体定义
#pragma once

#include <QString>
#include <QVector>
#include <QJsonObject>

struct Answer {
	int seqNumber; //题号
	bool hasMarked;
	bool isPic = false;
	double mark;
	QString text;
	QStringList pic_urls;

private:
	friend class zhixueHelper;
	QString topicPackId;
	QJsonObject topic;
};

struct Student {
	QString name;
	QVector<Answer> answers;

private:
	friend class zhixueHelper;
	QString userId;
};

struct Clazz {
	QString name;
	QVector<Student> students;

private:
	friend class zhixueHelper;
	QString homeworkId;
	QString url;
};

struct Section {
	QString rawHtml;//题目的frame
	QString answerHtml;
	double fullMark;
};

struct Homework {
	QString name;
	bool hasEnded;
	QVector<Clazz> classes;
	QVector<Section> sections;
};

struct UploadMarkPack {
	Homework* homework;
	int class_index;
	int student_index;
	int section_index;
	double mark;
};