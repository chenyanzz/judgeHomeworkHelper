#include "judgeHomeworkHelper.h"
#include <QtWidgets/QApplication>
#include <QDebug>
#include <QTextCodec>
#include <QMessageBox>
#include "webHelper.hpp"

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

void parseHomeworkList();
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
	parseHomeworkList();
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
std::string WString2String(const std::wstring& ws)
{
	std::string strLocale = setlocale(LC_ALL, "");
	const wchar_t* wchSrc = ws.c_str();
	size_t nDestSize = wcstombs(NULL, wchSrc, 0) + 1;
	char *chDest = new char[nDestSize];
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
	if (err.error != QJsonParseError::NoError) {
		QMessageBox::critical(nullptr, "json解析错误", err.errorString());
		exit(-1);
	}
	return json;
}

struct Answer {
	bool hasMarked;
	bool isPic=false;
	double mark;
	QString ans;
};

 Answer parseOneAnswer(const QJsonObject& o) {
	 Answer ans;
	 ans.hasMarked = o["resultType"].toString() != "UNKNOWN";
	 auto ans_obj = QJsonDocument::fromJson(o["answer"].toString().toUtf8()).object();
	 ans.isPic = ans_obj.contains("pic");
	 if (ans.isPic)ans.ans = ans_obj["pic"].toString();
	 else ans.ans = ans_obj["text"].toString();
	 ans.mark = o["markScore"].toDouble();
	 return ans;
}

void parseHomeworkList() {
	QString s = getHomeworkList("true");

	QJsonDocument json = parseJson(s);
	// qDebug() << json;

	QMultiMap<QString,QJsonObject> map;
	for(QJsonValue val : json.array()) {
		QString name = val.toObject()["name"].toString();
		map.insert(name, val.toObject());
	}

	//获取一个已结束的班，测试
	auto clazz = json.array()[0].toObject();
	// qDebug() << clazz;

	auto url = WString2String(clazz.value("url").toString().toStdWString());
	QString hwdata = getHomeworkFromUrl(url.c_str());

	json = parseJson(hwdata);

	auto stu = json.array()[0].toObject();
	
	// qDebug() << stu;
	auto stu_name = stu["userName"].toString();
	auto answers = stu["deatils"].toArray();
	QVector<int> subjective_job_pos;
	for (int i = 0; i < answers.size();i++) {
		auto ans = answers[i];
		if(ans.toObject()["topic"].toObject()["isSubjective"].toBool()==true) {
			subjective_job_pos.push_back(i);
		}
	}
	int job_number = subjective_job_pos[0];
	Answer ans = parseOneAnswer(answers[job_number].toObject());
}
