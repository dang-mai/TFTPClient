#include "widget.h"
#include "ui_widget.h"

#pragma execution_character_set("utf-8")

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    // 主窗口样式设置
    ui->setupUi(this);
    this->setWindowTitle("TFTPClient");
    this->resize(QSize(1200, 735));
    QPixmap* pixmap = new QPixmap(":/images/school.png");
    pixmap->scaled(ui->lab_item_01->size(), Qt::IgnoreAspectRatio);
    ui->lab_item_01->setScaledContents(true);
    ui->lab_item_01->setPixmap(*pixmap);
    ui->treeWidget->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    // 托盘设置
    QIcon icon = QIcon(":/icons/little_logo.png");
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(icon);
    trayIcon->setToolTip("TFTPClient"); // 提示文字
    // 设置模式选项
    ui->cbBoxMode->addItem("OCTET");
    ui->cbBoxMode->addItem("NETASCII");
    // 连接信号与槽函数
    qRegisterMetaType<Message>("Message&"); // 注册自定义类型
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
    // 初始化部件状态
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
{// 初始化线程参数
    int mode = (ui->cbBoxMode->currentIndex() == 0) ? OCTET : NETASCII;
    Opt opt {}; // 获取用户设置选项
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
    QString client_file = ui->textEditClientFile->toPlainText(); // 获取客户端文件
    QString server_file = ui->textEditServerFile->toPlainText(); // 获取服务端文件
    QString server_ip = QString(ui->textEditServer->toPlainText()).split(":")[0]; // 获取服务器IP
    Message message;
    message.type = TYPE::INIT_FAIL;
    bool flag = true;
    if (server_ip.isEmpty())
    {// 检查服务端IP是否为空
        flag = false;
        message.info = QString("Server IP:Port missing. Please input the Server IP:Port.");
        recv_message(message);
    }
    if ((new QHostAddress(server_ip))->isNull())
    {// 检查服务端IP是否合法
        flag = false;
        message.info = QString("Wrong Server IP. Please input the Server IP again.");
        recv_message(message);
    }
    if (client_file.isEmpty())
    {// 检查客户端文件是否为空
        flag = false;
        message.info = QString("Local file missing. Please input the local file.");
        recv_message(message);
    }
    if (server_file.isEmpty())
    {// 检查服务端文件是否为空
        flag = false;
        message.info = QString("Remote file missing. Please input the Remote file.");
        recv_message(message);
    }
    if (request_type == OPC_REQUEST_DOWN && _access(client_file.toStdString().c_str(), 0) != -1)
    {// 是否覆盖已存在文件
        QMessageBox::StandardButton res = QMessageBox::question(this, "Local File Warning.", "File exist. Do you want to overlay it?");
        if (res == QMessageBox::No)
        {
            flag = false;
            message.info = QString("Local File Warning. File exist");
            recv_message(message);
        }
    }
    if (flag)
    {// 检查合法后初始化次线程
        thread.init(request_type, mode, opt, server_ip, server_file, client_file);
    }
}

void Widget::recv_message(Message& message)
{// 图形界面显示信息并记录到日志中
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
    ui->treeWidget->scrollToItem(item); // 移动滚动条到底部
    log << "\n";
}

void Widget::recv_opts(Opt& opt)
{// 显示接收到的扩展选项
    ui->spinBoxTSize->setValue(opt.fields[QString("tsize")]);
    ui->spinBoxBlockSize->setValue(opt.fields[QString("blksize")]);
    ui->spinBoxTimeOut->setValue(opt.fields[QString("timeout")]);
}

void Widget::recv_progress_max(int size)
{// 修改进度条最大值
    ui->progressBar->setRange(0, size);
}

void Widget::recv_process_value(int value)
{// 修改进度条当前值
    ui->progressBar->setValue(value);
}

void Widget::speed_up(QString speed)
{// 显示实时上行速度(吞吐量)
    ui->labelUpload->setText(speed);
}
void Widget::speed_down(QString speed)
{// 显示实时下行速度(吞吐量)
    ui->labelDownload->setText(speed);
}

void Widget::task_start()
{// 启动次线程的上传/下载任务
    // 初始化任务进度条
    ui->progressBar->setRange(0,1);
    ui->progressBar->setValue(0);
    if (qobject_cast<QToolButton *>(sender())->text() == "Upload")
    {// 上传任务
        init_thread(OPC_REQUEST_UP);
    }
    else
    {// 下载任务
        init_thread(OPC_REQUEST_DOWN);
    }
    file.setFileName("log.txt");
    if (!file.open(QIODevice::WriteOnly|QIODevice::Text))
    {// 打开日志文件
        qDebug() << "打开文件失败";
    };
    log.setDevice(&file);
    timer.start(REFRESH_INTERVAL); // 启动计时器
    thread.start();
}

void Widget::task_finish()
{// 次线程的上传/下载任务完成
    timer.stop(); // 关闭计时器
    log.flush();
    file.close(); // 关闭日志文件
}

void Widget::cancel()
{// 取消次线程的上传/下载任务
    if (thread.isRunning())
    {
        thread.terminate();
        thread.wait();
    }
    task_finish();
}

void Widget::clear()
{// 清空功能
    ui->treeWidget->clear();
}

void Widget::client_file()
{// 获取客户端文件
    QString desktop_path = QProcessEnvironment::systemEnvironment().value("USERPROFILE") + "\\desktop";
    QString client_file = QFileDialog::getSaveFileName(this, "请选择本地文件", desktop_path);
    if(!client_file.isEmpty())
    {
        ui->textEditClientFile->setText(client_file);
    }
}

void Widget::littleShow()
{// 最小化窗口
    this->hide(); // 隐藏主窗口
    trayIcon->show(); // 显示托盘
    // 显示到系统提示框的信息
    QString title = "TFTPClient";
    QString text = "正自动在后台运行";
    trayIcon->showMessage(title, text, QSystemTrayIcon::Information, 1500); // 此参数为提示时长
}

void Widget::TrayIconAction(QSystemTrayIcon::ActivationReason reason)
{// 单击图标，恢复显示正常大小的窗口
    if (reason == QSystemTrayIcon::Trigger)
        this->showNormal();
}

void Widget::closeEvent(QCloseEvent *event)
{// 设置主窗口关闭
    if (thread.isRunning())
    {
        thread.terminate();
        thread.wait();
    }
    event->accept();
}
