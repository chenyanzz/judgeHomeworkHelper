#pragma once

#include <QLibrary>
#include "common.h"

class WebHelper
{
	friend class zhixueHelper;
protected:
	using func_login_t = cstr (*)(cstr username, cstr password);
	using func_getHomeworkList_t = cstr (*)(cstr hasEnd); //"true" or "false"
	using func_getHomeworkFromUrl_t = cstr (*)(cstr url);
	using func_saveMark_t = cstr (*)(cstr userId, cstr homeworkId, cstr topicPackId, cstr mark);

	static func_login_t login;
	static 	func_getHomeworkList_t getHomeworkList;
	static func_getHomeworkFromUrl_t getHomeworkFromUrl;
	static func_saveMark_t saveMark;

	static const QString lib_name;

	static QLibrary libWebHelper;

	static bool initLib();
	static void closeLib();
};