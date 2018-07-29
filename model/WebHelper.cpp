#include "WebHelper.h"
#include <QMessageBox>

WebHelper::func_login_t WebHelper::login;
WebHelper::func_getHomeworkList_t WebHelper::getHomeworkList;
WebHelper::func_getHomeworkFromUrl_t WebHelper::getHomeworkFromUrl;
WebHelper::func_saveMark_t WebHelper::saveMark;

const QString WebHelper::lib_name = "webHelper.dll";
QLibrary WebHelper::libWebHelper(lib_name);

bool WebHelper::initLib() {
	if(libWebHelper.isLoaded())libWebHelper.unload();

	if(!libWebHelper.load()) {
		return false;
	}
	login = (func_login_t)libWebHelper.resolve("C_Login");
	getHomeworkList = (func_getHomeworkList_t)libWebHelper.resolve("C_GetHomeworkList");
	getHomeworkFromUrl = (func_getHomeworkFromUrl_t)libWebHelper.resolve("C_GetHomeworkFromUrl");
	saveMark = (func_saveMark_t)libWebHelper.resolve("C_SaveCorrectResult");

	return login && getHomeworkList && getHomeworkFromUrl && saveMark;
}

void WebHelper::closeLib() {
	login = nullptr;
	getHomeworkList = nullptr;
	getHomeworkFromUrl = nullptr;
	saveMark = nullptr;

	libWebHelper.unload();
}
