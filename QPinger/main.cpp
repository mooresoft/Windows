#include <QCoreApplication>
#include <QProcess>
#include <QString>
#include <QTextCodec>
#include <QTextStream>
#include <iostream>


void ping(QString host)
{
    QProcess *cmd = new QProcess;
    QTextCodec * codec = QTextCodec::codecForName("gbk");
    QTextStream qout(stdout);

#ifdef _TTY_ARMV4_
    QString strArg = "ping -s 1 -c 1 " + b_tmpIpStr;        //linux平台下的格式
#else
    //QString strArg = "ping " + host + " -n 1 -w " + QString::number(m_timeoutInt) ;  //windows下的格式
    QString strArg = "ping " + host + " -n 3 " ;
#endif
    cmd->start(strArg);
    cmd->waitForReadyRead();
    cmd->waitForFinished();

    QString retStr = codec->toUnicode(cmd->readAll());

    qout<<retStr;
    if (-1 != retStr.indexOf("TTL"))
    {
        qout << (host + " is online!\n");
    }
    else
    {
        qout << (host + " is offline!\n");
    }
    retStr.clear();
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    ping("127.0.0.1");
    return a.exec();
}
