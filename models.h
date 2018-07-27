﻿#pragma once
#include <QString>
#include <QVector>

struct Answer {
	int seqNumber; //题号
	bool hasMarked;
	bool isPic = false;
	double mark;
	QString ans;
};

struct Student {
	QString name;
	QVector<Answer> answers;
};

struct Clazz {
	QString url;
	QString name;
	QVector<Student> students;
};

struct Homework {
	QString name;
	QVector<Clazz> classes;
};