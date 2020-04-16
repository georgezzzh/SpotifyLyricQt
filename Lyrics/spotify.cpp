#if defined (WIN32)
    #include <windows.h>
    #include <tlhelp32.h>
#else
    #include <QtDBus>
#endif
#include <QDebug>
#include "spotify.h"
#define cout qDebug().noquote()<<"["<<__LINE__<<"]"

QString Spotify::song;

#if defined(WIN32)
#define WINDOW_TEXT_LENGTH 256
//这个子函数没啥用
BOOL CALLBACK EnumChildWindowCallBack(HWND hWnd, LPARAM lParam)
{
    DWORD dwPid = 0;
    GetWindowThreadProcessId(hWnd, &dwPid); // 获得找到窗口所属的进程
    if(dwPid == lParam) // 判断是否是目标进程的窗口
    {
        TCHAR buf[WINDOW_TEXT_LENGTH];
        SendMessage(hWnd, WM_GETTEXT, WINDOW_TEXT_LENGTH, (LPARAM)buf);
        QString s=QString::fromWCharArray(buf);
        if(s.indexOf('-') != -1){

        }
        // 递归查找子窗口
        EnumChildWindows(hWnd, EnumChildWindowCallBack, lParam);
    }
    return TRUE;
}

BOOL CALLBACK EnumWindowCallBack(HWND hWnd, LPARAM lParam)
{

    DWORD dwPid = 0;
    //HWND是窗口得句柄
    GetWindowThreadProcessId(hWnd, &dwPid); // 获得找到窗口所属的进程
    if(dwPid == lParam) // 判断是否是目标进程的窗口
    {

        TCHAR buf[WINDOW_TEXT_LENGTH];
        SendMessage(hWnd, WM_GETTEXT, WINDOW_TEXT_LENGTH, (LPARAM)buf);
        QString s=QString::fromWCharArray(buf);
        // 输出窗口信息
        if(s.indexOf('-') != -1){
            Spotify::song=s;
        }
        // 继续查找子窗口
        EnumChildWindows(hWnd, EnumChildWindowCallBack, lParam);
    }
    return TRUE;
}
/*
 * 未找到spotify.exe进程, 返回-1
*/
QString Spotify::getSongName()
{
    //设置文字编码,Chinese Simple
    setlocale(LC_CTYPE, "chs");
    DWORD pid = 0;    // 进程id
    PROCESSENTRY32 pe;  // 进程信息
    pe.dwSize = sizeof(PROCESSENTRY32);
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); // 进程快照
    if(!Process32First(hSnapshot, &pe)) // 得到第一个进程的快照
        return "";
    // 遍历进程直到找打目标进程
    do
    {
        if(!Process32Next(hSnapshot, &pe))
            break;
    } while (QString::fromWCharArray(pe.szExeFile).compare("Spotify.exe"));
    pid = pe.th32ProcessID;
    if(QString::fromWCharArray(pe.szExeFile).compare("Spotify.exe")){
        song = "-1";
        qDebug()<<"未找到spotify.exe进程";
    }else{
        //qDebug()<<"Find out Spotify.exe process: "<<pid<<endl;
        EnumWindows(EnumWindowCallBack, pid);
        //qDebug()<<"song:"<<song;
    }
    return song;
}
#else
QString Spotify::getSongName()
{
    QDBusInterface iface("org.mpris.MediaPlayer2.spotify",
                         "/org/mpris/MediaPlayer2",
                         "org.freedesktop.DBus.Properties",
                         QDBusConnection::sessionBus());
    if(!iface.isValid()){
        qDebug()<<"接口出错";
        qDebug()<<QDBusConnection::sessionBus().lastError().message();
    }
    QDBusMessage reply = iface.call("Get","org.mpris.MediaPlayer2.Player"
                                    ,"Metadata");
    //未能获取到正确信息
    if(reply.type() != QDBusMessage::ReplyMessage){
        cout<<"未能获取到正确信息";
        return "";
    }
    //cout<<"reply:"<<reply;
    QList<QVariant> outArgs = reply.arguments();
    QDBusVariant first =  outArgs.at(0).value<QDBusVariant>();
    //qDebug()<<"type:"<<first.variant().value<QDBusArgument>().currentType();
    const QDBusArgument arg = first.variant().value<QDBusArgument>();
    QString artist;
    QString title;
    arg.beginMapEntry();
    while(!arg.atEnd()){
        arg.beginMap();
        QString key;
        QVariant value;
        arg>>key>>value;
        if(key=="xesam:title"){
            title = value.toString();
        }
        if(key == "xesam:albumArtist"){
            QStringList list=value.toStringList();
            for(int i=0;i<list.size();i++){
                artist += list.at(i)+",";
            }
        }
        //cout<<key<<","<<value;
        arg.endMapEntry();
    }
    arg.endMap();
    artist = artist.left(artist.size()-1);
    return artist+" - "+title;
}
#endif
void Spotify::run()
{
    while(running){
        QString name = getSongName();
        emit threadOver(name);
        msleep(500);
    }
}
Spotify::Spotify(QObject* parent):QThread(parent)
{
    running = true;
    song = "";
}
