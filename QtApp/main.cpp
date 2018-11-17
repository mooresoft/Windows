#include "mainwindow.h"
#include <QApplication>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    MainWindow w1;
    w1.show();

    QDesktopWidget* desktopWidget = QApplication::desktop();
    //获取可用桌面大小
    QRect deskRect = desktopWidget->availableGeometry();
    //获取设备屏幕大小
    QRect screenRect = desktopWidget->screenGeometry();

    int g_nActScreenX = screenRect.width();
    int g_nActScreenY = screenRect.height();
    //g_nActScreenX = deskRect.width();
    //g_nActScreenY = deskRect.height();

    //获取系统设置的屏幕个数（屏幕拷贝方式该值为1）
    int g_nScreenCount = desktopWidget->screenCount();

    QDesktopWidget *dwsktopwidget = QApplication::desktop();
    QRect deskrect = dwsktopwidget->availableGeometry();
    QRect screenrect = dwsktopwidget->screenGeometry();
    int scrcount = dwsktopwidget->screenCount();
    qCritical("screenrect.w==%s\n",qPrintable(QString::number(screenrect.width())));
    qCritical("screenrect.h==%s\n",qPrintable(QString::number(screenrect.height())));
    qCritical("deskrect.w==%s\n",qPrintable(QString::number(deskrect.width())));
    qCritical("deskrect.h==%s\n",qPrintable(QString::number(deskrect.height())));
    qCritical("scrcount==%s\n",qPrintable(QString::number(scrcount)));

    QDesktopWidget *desktop = QApplication::desktop();
    int screenNum = desktop->screenCount();
    for (int i=0; i<screenNum; i++)
    {
        QRect screen = desktop->screenGeometry();
        qDebug("screen %d, width %d, height %d", i, screen.width(), screen.height());
    }

    return a.exec();
}
