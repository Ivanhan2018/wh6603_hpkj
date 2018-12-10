
@echo off

set curRootPath=%cd%\..
echo %curRootPath%

echo 当前目录
set curPath=%cd%

echo 上一层目录
set prevPath=%cd%\..\
echo %prevPath%

echo 源资源路径
set srcClientPath=%curPath%\client
echo %srcClientPath%
set srcServerPath=%curPath%\server
echo %srcServerPath%

echo 目标客户端资源路径
set dstClientDebug=%prevPath%运行\ClientDebugUnicode
echo %dstClientDebug%
set dstClientRelease=%prevPath%运行\ClientReleaseUnicode
echo %dstClientRelease%

mkdir %dstClientDebug%
mkdir %dstClientRelease%

echo 目标服务端资源路径
set dstServerDebug=%prevPath%运行\ServDebugUnicode
set dstServerRelease=%prevPath%运行\ServReleaseUnicode
mkdir %dstServerDebug%
mkdir %dstServerRelease%

echo 请按任意键，开始拷贝！！！
pause

title 拷贝运行文件
color 30

xcopy %srcClientPath%\*.* %dstClientDebug%\    /s /Y /e /h
xcopy %srcClientPath%\*.* %dstClientRelease%\  /s /Y /e /h

xcopy %srcServerPath%\*.* %dstServerDebug%\    /s /Y /e /h
xcopy %srcServerPath%\*.* %dstServerRelease%\  /s /Y /e /h

pause