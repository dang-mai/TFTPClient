#include "widget.h"
#include "ui_widget.h"

#pragma execution_character_set("utf-8")

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    // ��������ʽ����
    ui->setupUi(this);
    this->setWindowTitle("TFTPClient");
    this->resize(QSize(1200, 735));
    QPixmap* pixmap = new QPixmap(":/images/school.png");
    pixmap->scaled(ui->lab_item_01->size(), Qt::IgnoreAspectRatio);
    ui->lab_item_01->setScaledContents(true);
    ui->lab_item_01->setPixmap(*pixmap);
    ui->treeWidget->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    // ��������
    QIcon icon = QIcon(":/icons/little_logo.png");
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(icon);
    trayIcon->setToolTip("TFTPClient"); // ��ʾ����
    // ����ģʽѡ��
    ui->cbBoxMode->addItem("OCTET");
    ui->cbBoxMode->addItem("NETASCII");
    // �����ź���ۺ���
    qRegisterMetaType<Message>("Message&"); // ע���Զ�������
    qRegisterMetaType<Opt>("Opt&");
    connect(ui->btnUpload, SIGNAL(clicked()), this, SLOT(task_start()));
    connect(ui->btnDownload, SIGNAL(clicked()), this, SLOT(task_start()));
    connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(cancel()));
    connect(ui->btnClear, SIGNAL(clicked()), this, SLOT(clear()));
    connect(ui->btnClientFile, SIGNAL(clicked()), this, SLOT(client_file()));
    connect(ui->btn_littleshow, SIGNAL(clicked()), this, SLOT(littleShow()));
    connect(&timer, SIGNAL(timeout()), &thread, SLOT(send_speed()));
    connect(&thread, SIGNAL(send_message(Message&)), this, SLOT(recv_message(Message&)), Qt::BlockingQueuedConnection);
    connect(&thread, SIGNAL(send_opts(Opt&)), this, SLOT(recv_opts(Opt&)), Qt::BlockingQueuedConnection);
    connect(&thread, SIGNAL(send_process_value(int)), this, SLOT(recv_process_value(int)), Qt::BlockingQueuedConnection);
    connect(&thread, SIGNAL(send_progress_max(int)), this, SLOT(recv_progress_max(int)), Qt::BlockingQueuedConnection);
    connect(&thread, SIGNAL(send_speed_down(QString)), this, SLOT(speed_down(QString)));
    connect(&thread, SIGNAL(send_speed_up(QString)), this, SLOT(speed_up(QString)));
    connect(&thread, SIGNAL(finished()), this, SLOT(task_finish()));
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(TrayIconAction(QSystemTrayIcon::ActivationReason)));
    // ��ʼ������״̬
    ui->textEditClientFile->setText("C:/Users/dangmai/Desktop/TEST/test.png");
    ui->textEditServerFile->setText("test.png");
    ui->textEditServer->setText("127.0.0.1:69");
    ui->spinBoxTSize->setValue(0);
    ui->spinBoxBlockSize->setValue(512);
    ui->spinBoxTimeOut->setValue(2);
    ui->progressBar->setRange(0,1);
    ui->progressBar->setValue(0);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::init_thread(OPC request_type)
{// ��ʼ���̲߳���
    int mode = (ui->cbBoxMode->currentIndex() == 0) ? OCTET : NETASCII;
    Opt opt {}; // ��ȡ�û�����ѡ��
    if (ui->chkBoxTSize->isChecked())
    {
        opt.fields[QString("tsize")] = ui->spinBoxTSize->value();
    }
    if (ui->chkBoxBlockSize->isChecked())
    {
        opt.fields[QString("blksize")] = ui->spinBoxBlockSize->value();
    }
    if (ui->chkBoxTimeOut->isChecked())
    {
        opt.fields[QString("timeout")] = ui->spinBoxTimeOut->value();
    }
    QString client_file = ui->textEditClientFile->toPlainText(); // ��ȡ�ͻ����ļ�
    QString server_file = ui->textEditServerFile->toPlainText(); // ��ȡ������ļ�
    QString server_ip = QString(ui->textEditServer->toPlainText()).split(":")[0]; // ��ȡ������IP
    Message message;
    message.type = TYPE::INIT_FAIL;
    bool flag = true;
    if (server_ip.isEmpty())
    {// �������IP�Ƿ�Ϊ��
        flag = false;
        message.info = QString("Server IP:Port missing. Please input the Server IP:Port.");
        recv_message(message);
    }
    if ((new QHostAddress(server_ip))->isNull())
    {// �������IP�Ƿ�Ϸ�
        flag = false;
        message.info = QString("Wrong Server IP. Please input the Server IP again.");
        recv_message(message);
    }
    if (client_file.isEmpty())
    {// ���ͻ����ļ��Ƿ�Ϊ��
        flag = false;
        message.info = QString("Local file missing. Please input the local file.");
        recv_message(message);
    }
    if (server_file.isEmpty())
    {// ��������ļ��Ƿ�Ϊ��
        flag = false;
        message.info = QString("Remote file missing. Please input the Remote file.");
        recv_message(message);
    }
    if (request_type == OPC_REQUEST_DOWN && _access(client_file.toStdString().c_str(), 0) != -1)
    {// �Ƿ񸲸��Ѵ����ļ�
        QMessageBox::StandardButton res = QMessageBox::question(this, "Local File Warning.", "File exist. Do you want to overlay it?");
        if (res == QMessageBox::No)
        {
            flag = false;
            message.info = QString("Local File Warning. File exist");
            recv_message(message);
        }
    }
    if (flag)
    {// ���Ϸ����ʼ�����߳�
        thread.init(request_type, mode, opt, server_ip, server_file, client_file);
    }
}

void Widget::recv_message(Message& message)
{// ͼ�ν�����ʾ��Ϣ����¼����־��
    QTreeWidgetItem* item = new QTreeWidgetItem();
    auto time = QDateTime::currentDateTime().toString("[MM/dd hh:mm:ss.zzz]");
    item->setData(0, 0, time);
    log << time << "\t\t";
    auto type = enum_name(message.type).data();
    item->setData(1, 0, type);
    log << type << "\t\t";
    auto info = message.info;
    item->setData(2, 0, info);
    log << info << "\t\t";
    ui->treeWidget->addTopLevelItem(item);
    ui->treeWidget->scrollToItem(item); // �ƶ����������ײ�
    log << "\n";
}

void Widget::recv_opts(Opt& opt)
{// ��ʾ���յ�����չѡ��
    ui->spinBoxTSize->setValue(opt.fields[QString("tsize")]);
    ui->spinBoxBlockSize->setValue(opt.fields[QString("blksize")]);
    ui->spinBoxTimeOut->setValue(opt.fields[QString("timeout")]);
}

void Widget::recv_progress_max(int size)
{// �޸Ľ��������ֵ
    ui->progressBar->setRange(0, size);
}

void Widget::recv_process_value(int value)
{// �޸Ľ�������ǰֵ
    ui->progressBar->setValue(value);
}

void Widget::speed_up(QString speed)
{// ��ʾʵʱ�����ٶ�(������)
    ui->labelUpload->setText(speed);
}
void Widget::speed_down(QString speed)
{// ��ʾʵʱ�����ٶ�(������)
    ui->labelDownload->setText(speed);
}

void Widget::task_start()
{// �������̵߳��ϴ�/��������
    // ��ʼ�����������
    ui->progressBar->setRange(0,1);
    ui->progressBar->setValue(0);
    if (qobject_cast<QToolButton *>(sender())->text() == "Upload")
    {// �ϴ�����
        init_thread(OPC_REQUEST_UP);
    }
    else
    {// ��������
        init_thread(OPC_REQUEST_DOWN);
    }
    file.setFileName("log.txt");
    if (!file.open(QIODevice::WriteOnly|QIODevice::Text))
    {// ����־�ļ�
        qDebug() << "���ļ�ʧ��";
    };
    log.setDevice(&file);
    timer.start(REFRESH_INTERVAL); // ������ʱ��
    thread.start();
}

void Widget::task_finish()
{// ���̵߳��ϴ�/�����������
    timer.stop(); // �رռ�ʱ��
    log.flush();
    file.close(); // �ر���־�ļ�
}

void Widget::cancel()
{// ȡ�����̵߳��ϴ�/��������
    if (thread.isRunning())
    {
        thread.terminate();
        thread.wait();
    }
    task_finish();
}

void Widget::clear()
{// ��չ���
    ui->treeWidget->clear();
}

void Widget::client_file()
{// ��ȡ�ͻ����ļ�
    QString desktop_path = QProcessEnvironment::systemEnvironment().value("USERPROFILE") + "\\desktop";
    QString client_file = QFileDialog::getSaveFileName(this, "��ѡ�񱾵��ļ�", desktop_path);
    if(!client_file.isEmpty())
    {
        ui->textEditClientFile->setText(client_file);
    }
}

void Widget::littleShow()
{// ��С������
    this->hide(); // ����������
    trayIcon->show(); // ��ʾ����
    // ��ʾ��ϵͳ��ʾ�����Ϣ
    QString title = "TFTPClient";
    QString text = "���Զ��ں�̨����";
    trayIcon->showMessage(title, text, QSystemTrayIcon::Information, 1500); // �˲���Ϊ��ʾʱ��
}

void Widget::TrayIconAction(QSystemTrayIcon::ActivationReason reason)
{// ����ͼ�꣬�ָ���ʾ������С�Ĵ���
    if (reason == QSystemTrayIcon::Trigger)
        this->showNormal();
}

void Widget::closeEvent(QCloseEvent *event)
{// ���������ڹر�
    if (thread.isRunning())
    {
        thread.terminate();
        thread.wait();
    }
    event->accept();
}
