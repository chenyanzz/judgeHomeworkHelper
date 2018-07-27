#include "judgeHomeworkHelper.h"
#include <QtWidgets/QApplication>
#include <QDebug>
#include <QTextCodec>
#include <QMessageBox>
#include "webHelper.hpp"
#include "models.h"

using namespace std;

void test();

int main(int argc, char* argv[]) {
	QApplication a(argc, argv);
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
	// judgeHomeworkHelper w;
	// w.show();
	test();
	return a.exec();
}

using namespace webHelper;

//只返回可以判的
QVector<Homework> parseHomeworkList();
QVector<Homework> parseHomeworkList_caseEnd(bool hasEnded);
void parseHomework(Homework& homework);
void do_login(char* username, char* password);

void test() {
	if(!initLib()) {
		QMessageBox::critical(nullptr, QString("无法加载 %1").arg(lib_name), libWebHelper.errorString());
		exit(-1);
	}
	char username[] = "zxt2434473";
	char password[] = "2019zhongkao";
	do_login(username, password);

	// qDebug() << homeWorkList;
	auto hwlist = parseHomeworkList();
	auto& hw = hwlist[0];
	parseHomework(hw);
}

void do_login(char* username, char* password) {
	QString err = login(username, password);
	if(err != L"登录成功！") {
		QMessageBox::critical(nullptr, "登录错误", err);
		exit(-1);
	}
}

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

/**
 * WTH IS THIS DOING???
 * @see https://www.cnblogs.com/dyg540/articles/7874214.html
 */
std::string WString2String(const std::wstring& ws) {
	std::string strLocale = setlocale(LC_ALL, "");
	const wchar_t* wchSrc = ws.c_str();
	size_t nDestSize = wcstombs(NULL, wchSrc, 0) + 1;
	char* chDest = new char[nDestSize];
	memset(chDest, 0, nDestSize);
	wcstombs(chDest, wchSrc, nDestSize);
	std::string strResult = chDest;
	delete[]chDest;
	setlocale(LC_ALL, strLocale.c_str());
	return strResult;
}

QJsonDocument parseJson(QString& s) {
	QJsonParseError err;
	QJsonDocument json = QJsonDocument::fromJson(s.toUtf8(), &err);
	if(err.error != QJsonParseError::NoError) {
		QMessageBox::critical(nullptr, "json解析错误", err.errorString());
		exit(-1);
	}
	return json;
}

Answer parseOneAnswer(const QJsonObject& o) {
	Answer ans;
	ans.hasMarked = o["resultType"].toString() != "UNKNOWN";
	auto ans_obj = QJsonDocument::fromJson(o["answer"].toString().toUtf8()).object();
	ans.isPic = ans_obj.contains("pic");
	if(ans.isPic)ans.ans = ans_obj["pic"].toString();
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

QVector<Homework> parseHomeworkList() {
	auto hwlist_1 = parseHomeworkList_caseEnd(true);
	auto hwlist_2 = parseHomeworkList_caseEnd(false);
	return hwlist_1 + hwlist_2;
}


QVector<Homework> parseHomeworkList_caseEnd(bool hasEnded) {
	QString s = getHomeworkList(hasEnded?"true":"false");

	QJsonDocument hwData = parseJson(s);
	// qDebug() << hwData;

	QVector<Homework> homeworks;
	for(QJsonValue val : hwData.array()) {
		QString hwname = val.toObject()["name"].toString();
		QString clsname = val.toObject()["class"].toString();
		Clazz cls;
		cls.name = clsname;
		cls.url = val.toObject()["url"].toString();
		if (cls.url.isEmpty())continue;
		auto it = find_if(homeworks.begin(), homeworks.end(), [&](Homework& hw) { return hw.name == hwname; });
		if(it == homeworks.end()) {
			Homework hw;
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

void parseHomework(Homework& homework) {
	for(auto& clazz : homework.classes) {
		// qDebug() << clazz;
		auto url = WString2String(clazz.url.toStdWString());
		QString hwdata = getHomeworkFromUrl(url.c_str());

		QJsonDocument clsData = parseJson(hwdata);
		for(auto v : clsData.array()) {
			clazz.students.push_back(parseOneStu(v.toObject()));
		}
	}
}
