#pragma once

#include <QLibrary>

namespace webHelper
{
	using cstr = const char*;
	using func_login_t = cstr (*)(cstr username, cstr password);
	using func_getHomeworkList_t = cstr (*)(cstr hasEnd); //"true" or "false"
	using func_getHomeworkFromUrl_t = cstr (*)(cstr url);
	using func_saveMark_t = cstr (*)(cstr userId, cstr homeworkId, cstr topicPackId, cstr mark);

	func_login_t login;
	func_getHomeworkList_t getHomeworkList;
	func_getHomeworkFromUrl_t getHomeworkFromUrl;
	func_saveMark_t saveMark;

	const QString lib_name = "webHelper.dll";

	QLibrary libWebHelper(lib_name);

	inline bool initLib() {
		if(!libWebHelper.load()) {
			return false;
		}
		login = (func_login_t)libWebHelper.resolve("C_Login");
		getHomeworkList = (func_getHomeworkList_t)libWebHelper.resolve("C_GetHomeworkList");
		getHomeworkFromUrl = (func_getHomeworkFromUrl_t)libWebHelper.resolve("C_GetHomeworkFromUrl");
		saveMark = (func_saveMark_t)libWebHelper.resolve("C_SaveCorrectResult");

		return login && getHomeworkList && getHomeworkFromUrl && saveMark;
	}
};
