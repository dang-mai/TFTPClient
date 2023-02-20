#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QDateTime>
#include <QFileDialog>
#include <QHostAddress>
#include <QProcessEnvironment>
#include <QMetaType>
//最小化相关
#include <QSystemTrayIcon>
#include <QCloseEvent>
#include <QMenu>
#include <QAction>

#include "thread.h"

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;
    Thread thread; // 创建次线程
    QTimer timer; // 创建定时器
    QSystemTrayIcon* trayIcon = nullptr; // 托盘
    QFile file; // 日志文件
    QTextStream log; // 日志流

    void init_thread(OPC request_type);

private slots:
    void littleShow(); // 最小化显示函数
    void task_start(); // 次线程任务开始函数
    void task_finish(); // 次线程任务结束函数
    void cancel(); // 取消线程任务
    void clear(); // 清空信息显示
    void client_file(); // 选择客户端文件
    void speed_up(QString speed); // 显示实时上行速度
    void speed_down(QString speed); // 显示实时下行速度
    void TrayIconAction(QSystemTrayIcon::ActivationReason reason); // 对托盘图标操作的槽：本代码实现单击图标恢复窗口功能
    void recv_message(Message& message); // 显示上传/下载任务过程信息
    void recv_progress_max(int size); // 设置进度条最大值
    void recv_process_value(int value); // 设置进度条当前值
    void recv_opts(Opt& opt); // 显示扩展选项信息

protected:
    void closeEvent(QCloseEvent *event); // 设置主窗口关闭
};
#endif // WIDGET_H
