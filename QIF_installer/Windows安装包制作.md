* 设置Qt Creator中的Release，编译生成exe文件
* 将exe文件拷贝到目录，用windows中的Qt 5.14(MinGW 64bit)运行如下代码,生成依赖库
```
windeployqt Lyrics.exe
```
* 保存libssl-1_1-x64.dll与libcrypto-1_1-x64.dll到目录中，支持HTTPS
* 将翻译文件(qm后缀)保存到tr目录中
### 打包安装包
* 打包运行命令, 切换到`QtIFW-4.0.0\bin`目录下,运行如下命令
```cmd
./binarycreator.exe -c C:\Users\geoge\Desktop\SpotifyLyricQt\config\config.xml -p C:\Users\geoge\Desktop\SpotifyLyricQt\packages SpotifyLyricQt.exe -v
```
