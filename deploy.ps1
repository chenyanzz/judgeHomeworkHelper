devenv.exe .\judgeHomeworkHelper.vcxproj /build "Release|x64"
mkdir -p installpack
Copy-Item .\x64\Release\judgeHomeworkHelper.exe .\installpack\judgeHomeworkHelper.exe
Copy-Item .\user.dat .\installpack\user.dat
Copy-Item .\hw_keywords.dat .\installpack\hw_keywords.dat
Copy-Item -r .\assets\ .\installpack\
windeployqt.exe .\installpack\judgeHomeworkHelper.exe --no-quick-import --no-translations