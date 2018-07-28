#pragma once

#include <QString>
#include <QVector>

#include "homeworkModel.h"

void do_lib_init();
bool do_login(const QString& username, const QString& password);

//只返回可以判的
QVector<Homework> parseHomeworkList();
void parseHomework(Homework& homework);