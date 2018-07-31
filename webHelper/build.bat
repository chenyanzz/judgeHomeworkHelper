go build -buildmode=c-archive -o webHelper.lib main.go
go build -buildmode=c-shared -o webHelper.dll main.go
copy .\webHelper.dll ..\webHelper.dll