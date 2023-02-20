/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QGridLayout *gridLayout;
    QWidget *w_bg;
    QHBoxLayout *horizontalLayout_7;
    QWidget *menu_bar;
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout;
    QLabel *lab_item_01;
    QLabel *title_label;
    QGridLayout *gridLayout_3;
    QToolButton *btnCancel;
    QToolButton *btnDownload;
    QToolButton *btnUpload;
    QToolButton *btnClear;
    QSpacerItem *verticalSpacer;
    QLabel *label_7;
    QProgressBar *progressBar;
    QSpacerItem *verticalSpacer_5;
    QHBoxLayout *horizontalLayout_6;
    QSpacerItem *horizontalSpacer;
    QPushButton *btn_littleshow;
    QSpacerItem *horizontalSpacer_2;
    QLabel *cpr_label;
    QWidget *main_bar;
    QGridLayout *gridLayout_2;
    QHBoxLayout *horizontalLayout_3;
    QTextEdit *textEditServer;
    QPushButton *btnServer;
    QHBoxLayout *horizontalLayout_2;
    QTextEdit *textEditServerFile;
    QPushButton *btnServerFile;
    QTreeWidget *treeWidget;
    QHBoxLayout *horizontalLayout_5;
    QVBoxLayout *label_all;
    QPushButton *pushButton_index;
    QCheckBox *chkBoxTSize;
    QSpinBox *spinBoxTSize;
    QCheckBox *chkBoxBlockSize;
    QSpinBox *spinBoxBlockSize;
    QCheckBox *chkBoxTimeOut;
    QSpinBox *spinBoxTimeOut;
    QHBoxLayout *horizontalLayout_4;
    QLabel *labelUploadTag;
    QLabel *labelUpload;
    QLabel *labelUploadUnit;
    QLabel *labelDownloadTag;
    QLabel *labelDownload;
    QLabel *labelDownloadUnit;
    QPushButton *btnMode;
    QComboBox *cbBoxMode;
    QHBoxLayout *horizontalLayout;
    QTextEdit *textEditClientFile;
    QPushButton *btnClientFile;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->resize(1041, 602);
        Widget->setMinimumSize(QSize(0, 0));
        Widget->setStyleSheet(QString::fromUtf8(""));
        gridLayout = new QGridLayout(Widget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        w_bg = new QWidget(Widget);
        w_bg->setObjectName(QString::fromUtf8("w_bg"));
        horizontalLayout_7 = new QHBoxLayout(w_bg);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        menu_bar = new QWidget(w_bg);
        menu_bar->setObjectName(QString::fromUtf8("menu_bar"));
        verticalLayout_3 = new QVBoxLayout(menu_bar);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        lab_item_01 = new QLabel(menu_bar);
        lab_item_01->setObjectName(QString::fromUtf8("lab_item_01"));
        lab_item_01->setEnabled(true);
        QSizePolicy sizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(lab_item_01->sizePolicy().hasHeightForWidth());
        lab_item_01->setSizePolicy(sizePolicy);
        lab_item_01->setMaximumSize(QSize(16777215, 16777215));
        lab_item_01->setStyleSheet(QString::fromUtf8(""));
        lab_item_01->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(lab_item_01);

        title_label = new QLabel(menu_bar);
        title_label->setObjectName(QString::fromUtf8("title_label"));
        title_label->setStyleSheet(QString::fromUtf8("font-family: \"Arial\";"));
        title_label->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(title_label);

        verticalLayout->setStretch(0, 2);
        verticalLayout->setStretch(1, 5);

        verticalLayout_3->addLayout(verticalLayout);

        gridLayout_3 = new QGridLayout();
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        btnCancel = new QToolButton(menu_bar);
        btnCancel->setObjectName(QString::fromUtf8("btnCancel"));
        btnCancel->setMinimumSize(QSize(70, 70));
        btnCancel->setCursor(QCursor(Qt::PointingHandCursor));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/restart.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnCancel->setIcon(icon);
        btnCancel->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout_3->addWidget(btnCancel, 2, 0, 1, 1);

        btnDownload = new QToolButton(menu_bar);
        btnDownload->setObjectName(QString::fromUtf8("btnDownload"));
        btnDownload->setMinimumSize(QSize(70, 70));
        btnDownload->setCursor(QCursor(Qt::PointingHandCursor));
        btnDownload->setStyleSheet(QString::fromUtf8(""));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/start.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnDownload->setIcon(icon1);
        btnDownload->setAutoRepeatDelay(308);
        btnDownload->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout_3->addWidget(btnDownload, 1, 1, 1, 1);

        btnUpload = new QToolButton(menu_bar);
        btnUpload->setObjectName(QString::fromUtf8("btnUpload"));
        btnUpload->setMinimumSize(QSize(70, 70));
        btnUpload->setCursor(QCursor(Qt::PointingHandCursor));
        btnUpload->setStyleSheet(QString::fromUtf8(""));
        btnUpload->setIcon(icon1);
        btnUpload->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout_3->addWidget(btnUpload, 1, 0, 1, 1);

        btnClear = new QToolButton(menu_bar);
        btnClear->setObjectName(QString::fromUtf8("btnClear"));
        btnClear->setMinimumSize(QSize(70, 70));
        btnClear->setCursor(QCursor(Qt::PointingHandCursor));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8("icons/clear.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnClear->setIcon(icon2);
        btnClear->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout_3->addWidget(btnClear, 2, 1, 1, 1);


        verticalLayout_3->addLayout(gridLayout_3);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer);

        label_7 = new QLabel(menu_bar);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        QFont font;
        font.setPointSize(10);
        font.setBold(true);
        label_7->setFont(font);
        label_7->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(label_7);

        progressBar = new QProgressBar(menu_bar);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setValue(60);

        verticalLayout_3->addWidget(progressBar);

        verticalSpacer_5 = new QSpacerItem(223, 44, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_5);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer);

        btn_littleshow = new QPushButton(menu_bar);
        btn_littleshow->setObjectName(QString::fromUtf8("btn_littleshow"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8("icons/minimize.png"), QSize(), QIcon::Normal, QIcon::Off);
        btn_littleshow->setIcon(icon3);

        horizontalLayout_6->addWidget(btn_littleshow);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_2);


        verticalLayout_3->addLayout(horizontalLayout_6);

        cpr_label = new QLabel(menu_bar);
        cpr_label->setObjectName(QString::fromUtf8("cpr_label"));
        cpr_label->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(cpr_label);


        horizontalLayout_7->addWidget(menu_bar);

        main_bar = new QWidget(w_bg);
        main_bar->setObjectName(QString::fromUtf8("main_bar"));
        gridLayout_2 = new QGridLayout(main_bar);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        textEditServer = new QTextEdit(main_bar);
        textEditServer->setObjectName(QString::fromUtf8("textEditServer"));
        textEditServer->setMaximumSize(QSize(16777215, 35));
        textEditServer->setStyleSheet(QString::fromUtf8("QTextEdit\n"
"{\n"
"border: 1px solid grey ;  \n"
"border-radius:5px;\n"
"padding: 1px 1px 1px 1px;  \n"
"background-color:rgb(255,255,255)\n"
"}"));

        horizontalLayout_3->addWidget(textEditServer);

        btnServer = new QPushButton(main_bar);
        btnServer->setObjectName(QString::fromUtf8("btnServer"));
        btnServer->setMaximumSize(QSize(16777215, 35));
        btnServer->setCursor(QCursor(Qt::PointingHandCursor));
        btnServer->setStyleSheet(QString::fromUtf8(""));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/icons/gear.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnServer->setIcon(icon4);
        btnServer->setIconSize(QSize(20, 20));
        btnServer->setAutoDefault(false);
        btnServer->setFlat(false);

        horizontalLayout_3->addWidget(btnServer);

        horizontalLayout_3->setStretch(0, 7);
        horizontalLayout_3->setStretch(1, 1);

        gridLayout_2->addLayout(horizontalLayout_3, 2, 0, 1, 2);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        textEditServerFile = new QTextEdit(main_bar);
        textEditServerFile->setObjectName(QString::fromUtf8("textEditServerFile"));
        textEditServerFile->setMaximumSize(QSize(16777215, 35));
        textEditServerFile->setStyleSheet(QString::fromUtf8("QTextEdit\n"
"{\n"
"border: 1px solid grey ;  \n"
"border-radius:5px;\n"
"padding: 1px 1px 1px 1px;  \n"
"background-color:rgb(255,255,255)\n"
"}"));

        horizontalLayout_2->addWidget(textEditServerFile);

        btnServerFile = new QPushButton(main_bar);
        btnServerFile->setObjectName(QString::fromUtf8("btnServerFile"));
        btnServerFile->setMaximumSize(QSize(16777215, 35));
        btnServerFile->setCursor(QCursor(Qt::PointingHandCursor));
        btnServerFile->setStyleSheet(QString::fromUtf8(""));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/icons/open.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnServerFile->setIcon(icon5);
        btnServerFile->setIconSize(QSize(20, 20));
        btnServerFile->setAutoDefault(false);
        btnServerFile->setFlat(false);

        horizontalLayout_2->addWidget(btnServerFile);

        horizontalLayout_2->setStretch(0, 5);
        horizontalLayout_2->setStretch(1, 1);

        gridLayout_2->addLayout(horizontalLayout_2, 0, 1, 1, 1);

        treeWidget = new QTreeWidget(main_bar);
        QIcon icon6;
        icon6.addFile(QString::fromUtf8("icons/name.png"), QSize(), QIcon::Normal, QIcon::Off);
        QFont font1;
        font1.setFamily(QString::fromUtf8("Perpetua"));
        font1.setPointSize(13);
        font1.setBold(true);
        font1.setItalic(true);
        QIcon icon7;
        icon7.addFile(QString::fromUtf8("icons/property.png"), QSize(), QIcon::Normal, QIcon::Off);
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/icons/property.png"), QSize(), QIcon::Normal, QIcon::Off);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setFont(2, font1);
        __qtreewidgetitem->setIcon(2, icon8);
        __qtreewidgetitem->setFont(1, font1);
        __qtreewidgetitem->setIcon(1, icon7);
        __qtreewidgetitem->setFont(0, font1);
        __qtreewidgetitem->setIcon(0, icon6);
        treeWidget->setHeaderItem(__qtreewidgetitem);
        treeWidget->setObjectName(QString::fromUtf8("treeWidget"));
        treeWidget->setStyleSheet(QString::fromUtf8("QTreeWidget{\n"
"border: 1px solid grey ;  \n"
"border-radius:20px;\n"
"padding: 1px 12px 1px 12px;  \n"
"background-color:rgb(255,255,255)\n"
"}"));

        gridLayout_2->addWidget(treeWidget, 1, 0, 1, 2);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        label_all = new QVBoxLayout();
        label_all->setObjectName(QString::fromUtf8("label_all"));
        pushButton_index = new QPushButton(main_bar);
        pushButton_index->setObjectName(QString::fromUtf8("pushButton_index"));
        pushButton_index->setMinimumSize(QSize(0, 10));
        pushButton_index->setMaximumSize(QSize(16777215, 40));
        QFont font2;
        font2.setPointSize(12);
        font2.setBold(true);
        pushButton_index->setFont(font2);
        pushButton_index->setStyleSheet(QString::fromUtf8("font-size:12pt;"));

        label_all->addWidget(pushButton_index);

        chkBoxTSize = new QCheckBox(main_bar);
        chkBoxTSize->setObjectName(QString::fromUtf8("chkBoxTSize"));
        chkBoxTSize->setMinimumSize(QSize(4, 0));
        QFont font3;
        font3.setPointSize(9);
        font3.setBold(true);
        font3.setUnderline(false);
        chkBoxTSize->setFont(font3);

        label_all->addWidget(chkBoxTSize);

        spinBoxTSize = new QSpinBox(main_bar);
        spinBoxTSize->setObjectName(QString::fromUtf8("spinBoxTSize"));
        spinBoxTSize->setMaximum(999999999);

        label_all->addWidget(spinBoxTSize);

        chkBoxBlockSize = new QCheckBox(main_bar);
        chkBoxBlockSize->setObjectName(QString::fromUtf8("chkBoxBlockSize"));
        QFont font4;
        font4.setBold(true);
        chkBoxBlockSize->setFont(font4);

        label_all->addWidget(chkBoxBlockSize);

        spinBoxBlockSize = new QSpinBox(main_bar);
        spinBoxBlockSize->setObjectName(QString::fromUtf8("spinBoxBlockSize"));
        spinBoxBlockSize->setMaximum(999999999);

        label_all->addWidget(spinBoxBlockSize);

        chkBoxTimeOut = new QCheckBox(main_bar);
        chkBoxTimeOut->setObjectName(QString::fromUtf8("chkBoxTimeOut"));
        chkBoxTimeOut->setFont(font4);

        label_all->addWidget(chkBoxTimeOut);

        spinBoxTimeOut = new QSpinBox(main_bar);
        spinBoxTimeOut->setObjectName(QString::fromUtf8("spinBoxTimeOut"));
        spinBoxTimeOut->setMaximum(999999999);

        label_all->addWidget(spinBoxTimeOut);


        horizontalLayout_5->addLayout(label_all);

        horizontalLayout_5->setStretch(0, 1);

        gridLayout_2->addLayout(horizontalLayout_5, 4, 0, 1, 2);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        labelUploadTag = new QLabel(main_bar);
        labelUploadTag->setObjectName(QString::fromUtf8("labelUploadTag"));
        labelUploadTag->setFont(font4);

        horizontalLayout_4->addWidget(labelUploadTag);

        labelUpload = new QLabel(main_bar);
        labelUpload->setObjectName(QString::fromUtf8("labelUpload"));
        labelUpload->setFont(font4);

        horizontalLayout_4->addWidget(labelUpload);

        labelUploadUnit = new QLabel(main_bar);
        labelUploadUnit->setObjectName(QString::fromUtf8("labelUploadUnit"));
        labelUploadUnit->setFont(font4);

        horizontalLayout_4->addWidget(labelUploadUnit);

        labelDownloadTag = new QLabel(main_bar);
        labelDownloadTag->setObjectName(QString::fromUtf8("labelDownloadTag"));
        labelDownloadTag->setFont(font4);

        horizontalLayout_4->addWidget(labelDownloadTag);

        labelDownload = new QLabel(main_bar);
        labelDownload->setObjectName(QString::fromUtf8("labelDownload"));
        labelDownload->setFont(font4);

        horizontalLayout_4->addWidget(labelDownload);

        labelDownloadUnit = new QLabel(main_bar);
        labelDownloadUnit->setObjectName(QString::fromUtf8("labelDownloadUnit"));
        labelDownloadUnit->setFont(font4);

        horizontalLayout_4->addWidget(labelDownloadUnit);

        btnMode = new QPushButton(main_bar);
        btnMode->setObjectName(QString::fromUtf8("btnMode"));
        btnMode->setFont(font4);

        horizontalLayout_4->addWidget(btnMode);

        cbBoxMode = new QComboBox(main_bar);
        cbBoxMode->setObjectName(QString::fromUtf8("cbBoxMode"));
        cbBoxMode->setFont(font4);

        horizontalLayout_4->addWidget(cbBoxMode);


        gridLayout_2->addLayout(horizontalLayout_4, 3, 0, 1, 2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        textEditClientFile = new QTextEdit(main_bar);
        textEditClientFile->setObjectName(QString::fromUtf8("textEditClientFile"));
        textEditClientFile->setMaximumSize(QSize(16777215, 35));
        textEditClientFile->setStyleSheet(QString::fromUtf8("QTextEdit\n"
"{\n"
"border: 1px solid grey ;  \n"
"border-radius:5px;\n"
"padding: 1px 1px 1px 1px;  \n"
"background-color:rgb(255,255,255)\n"
"}"));

        horizontalLayout->addWidget(textEditClientFile);

        btnClientFile = new QPushButton(main_bar);
        btnClientFile->setObjectName(QString::fromUtf8("btnClientFile"));
        btnClientFile->setMaximumSize(QSize(16777215, 35));
        btnClientFile->setCursor(QCursor(Qt::PointingHandCursor));
        btnClientFile->setStyleSheet(QString::fromUtf8(""));
        btnClientFile->setIcon(icon5);
        btnClientFile->setIconSize(QSize(20, 20));
        btnClientFile->setAutoDefault(false);
        btnClientFile->setFlat(false);

        horizontalLayout->addWidget(btnClientFile);

        horizontalLayout->setStretch(0, 5);
        horizontalLayout->setStretch(1, 1);

        gridLayout_2->addLayout(horizontalLayout, 0, 0, 1, 1);


        horizontalLayout_7->addWidget(main_bar);

        horizontalLayout_7->setStretch(0, 1);
        horizontalLayout_7->setStretch(1, 3);

        gridLayout->addWidget(w_bg, 0, 0, 1, 1);


        retranslateUi(Widget);

        btnServer->setDefault(false);
        btnServerFile->setDefault(false);
        btnClientFile->setDefault(false);


        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "Widget", nullptr));
        lab_item_01->setText(QString());
        title_label->setText(QCoreApplication::translate("Widget", "TFTPClient", nullptr));
        btnCancel->setText(QCoreApplication::translate("Widget", "Cancel", nullptr));
        btnDownload->setText(QCoreApplication::translate("Widget", "Downlaod", nullptr));
        btnUpload->setText(QCoreApplication::translate("Widget", "Upload", nullptr));
        btnClear->setText(QCoreApplication::translate("Widget", "Clear", nullptr));
        label_7->setText(QCoreApplication::translate("Widget", "Upload/Download Progress", nullptr));
        btn_littleshow->setText(QString());
        cpr_label->setText(QCoreApplication::translate("Widget", "copyright\302\2512022", nullptr));
        btnServer->setText(QCoreApplication::translate("Widget", "Server", nullptr));
        btnServerFile->setText(QCoreApplication::translate("Widget", "Server File", nullptr));
        QTreeWidgetItem *___qtreewidgetitem = treeWidget->headerItem();
        ___qtreewidgetitem->setText(2, QCoreApplication::translate("Widget", "Info", nullptr));
        ___qtreewidgetitem->setText(1, QCoreApplication::translate("Widget", "State", nullptr));
        ___qtreewidgetitem->setText(0, QCoreApplication::translate("Widget", "Time", nullptr));
        pushButton_index->setText(QCoreApplication::translate("Widget", "Extended Options", nullptr));
        chkBoxTSize->setText(QCoreApplication::translate("Widget", "\342\234\247TSize\342\234\247", nullptr));
        chkBoxBlockSize->setText(QCoreApplication::translate("Widget", "\342\234\247BlockSize\342\234\247", nullptr));
        chkBoxTimeOut->setText(QCoreApplication::translate("Widget", "\342\234\247TimeOut\342\234\247", nullptr));
        labelUploadTag->setText(QCoreApplication::translate("Widget", "\342\206\221\357\274\232", nullptr));
        labelUpload->setText(QCoreApplication::translate("Widget", "0.00", nullptr));
        labelUploadUnit->setText(QCoreApplication::translate("Widget", "KB/s", nullptr));
        labelDownloadTag->setText(QCoreApplication::translate("Widget", "\342\206\223\357\274\232", nullptr));
        labelDownload->setText(QCoreApplication::translate("Widget", "0.00", nullptr));
        labelDownloadUnit->setText(QCoreApplication::translate("Widget", "KB/s", nullptr));
        btnMode->setText(QCoreApplication::translate("Widget", "Mode", nullptr));
        btnClientFile->setText(QCoreApplication::translate("Widget", "Client File", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
