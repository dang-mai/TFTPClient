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
//��С�����
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
    Thread thread; // �������߳�
    QTimer timer; // ������ʱ��
    QSystemTrayIcon* trayIcon = nullptr; // ����
    QFile file; // ��־�ļ�
    QTextStream log; // ��־��

    void init_thread(OPC request_type);

private slots:
    void littleShow(); // ��С����ʾ����
    void task_start(); // ���߳�����ʼ����
    void task_finish(); // ���߳������������
    void cancel(); // ȡ���߳�����
    void clear(); // �����Ϣ��ʾ
    void client_file(); // ѡ��ͻ����ļ�
    void speed_up(QString speed); // ��ʾʵʱ�����ٶ�
    void speed_down(QString speed); // ��ʾʵʱ�����ٶ�
    void TrayIconAction(QSystemTrayIcon::ActivationReason reason); // ������ͼ������Ĳۣ�������ʵ�ֵ���ͼ��ָ����ڹ���
    void recv_message(Message& message); // ��ʾ�ϴ�/�������������Ϣ
    void recv_progress_max(int size); // ���ý��������ֵ
    void recv_process_value(int value); // ���ý�������ǰֵ
    void recv_opts(Opt& opt); // ��ʾ��չѡ����Ϣ

protected:
    void closeEvent(QCloseEvent *event); // ���������ڹر�
};
#endif // WIDGET_H
