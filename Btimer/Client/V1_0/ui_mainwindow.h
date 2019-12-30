/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Tue Mar 15 15:19:15 2016
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QPushButton>
#include <QtGui/QTabWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    QWidget *tab1;
    QFrame *frame;
    QPushButton *Ch1Time;
    QLabel *label1_3;
    QFrame *frame_3;
    QPushButton *Ch2Time;
    QLabel *label1_2;
    QFrame *frame_6;
    QLabel *label1;
    QFrame *frame_7;
    QLabel *labelStErr;
    QLabel *labelPulse;
    QWidget *tab2;
    QFrame *frame_10;
    QPushButton *Ch12Time;
    QLabel *label1_8;
    QFrame *frame_12;
    QPushButton *Ch9Time;
    QLabel *label1_4;
    QFrame *frame_14;
    QPushButton *Ch10Time;
    QLabel *label1_7;
    QFrame *frame_15;
    QPushButton *Ch11Time;
    QLabel *label1_6;
    QFrame *frame_26;
    QLabel *label2;
    QWidget *tab3;
    QFrame *frame_18;
    QPushButton *Ch20Time;
    QLabel *label1_12;
    QFrame *frame_19;
    QPushButton *Ch21Time;
    QLabel *label1_13;
    QFrame *frame_20;
    QPushButton *Ch17Time;
    QLabel *label1_9;
    QFrame *frame_22;
    QPushButton *Ch18Time;
    QLabel *label1_10;
    QFrame *frame_23;
    QPushButton *Ch19Time;
    QLabel *label1_11;
    QFrame *frame_24;
    QPushButton *Ch22Time;
    QLabel *label1_14;
    QFrame *frame_27;
    QLabel *label3;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->setEnabled(true);
        MainWindow->resize(818, 498);
        QFont font;
        font.setPointSize(20);
        font.setBold(true);
        font.setWeight(75);
        MainWindow->setFont(font);
        MainWindow->setAutoFillBackground(true);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        centralWidget->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 250, 235);"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setMinimumSize(QSize(0, 425));
        tabWidget->setMaximumSize(QSize(800, 480));
        QFont font1;
        font1.setPointSize(24);
        font1.setBold(true);
        font1.setWeight(75);
        tabWidget->setFont(font1);
        tabWidget->setTabShape(QTabWidget::Rounded);
        tabWidget->setIconSize(QSize(20, 20));
        tabWidget->setElideMode(Qt::ElideNone);
        tabWidget->setUsesScrollButtons(true);
        tabWidget->setDocumentMode(false);
        tabWidget->setTabsClosable(false);
        tabWidget->setMovable(false);
        tab1 = new QWidget();
        tab1->setObjectName(QString::fromUtf8("tab1"));
        frame = new QFrame(tab1);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(10, 30, 661, 101));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        Ch1Time = new QPushButton(frame);
        Ch1Time->setObjectName(QString::fromUtf8("Ch1Time"));
        Ch1Time->setEnabled(true);
        Ch1Time->setGeometry(QRect(250, 20, 401, 71));
        Ch1Time->setFont(font1);
        label1_3 = new QLabel(frame);
        label1_3->setObjectName(QString::fromUtf8("label1_3"));
        label1_3->setGeometry(QRect(20, 40, 141, 31));
        label1_3->setFont(font1);
        frame_3 = new QFrame(tab1);
        frame_3->setObjectName(QString::fromUtf8("frame_3"));
        frame_3->setGeometry(QRect(10, 160, 661, 111));
        frame_3->setFrameShape(QFrame::StyledPanel);
        frame_3->setFrameShadow(QFrame::Raised);
        Ch2Time = new QPushButton(frame_3);
        Ch2Time->setObjectName(QString::fromUtf8("Ch2Time"));
        Ch2Time->setGeometry(QRect(250, 20, 401, 71));
        Ch2Time->setFont(font1);
        label1_2 = new QLabel(frame_3);
        label1_2->setObjectName(QString::fromUtf8("label1_2"));
        label1_2->setGeometry(QRect(20, 40, 141, 31));
        label1_2->setFont(font1);
        frame_6 = new QFrame(tab1);
        frame_6->setObjectName(QString::fromUtf8("frame_6"));
        frame_6->setGeometry(QRect(10, 370, 771, 51));
        frame_6->setFrameShape(QFrame::StyledPanel);
        frame_6->setFrameShadow(QFrame::Raised);
        label1 = new QLabel(frame_6);
        label1->setObjectName(QString::fromUtf8("label1"));
        label1->setGeometry(QRect(20, 10, 751, 31));
        label1->setFont(font1);
        frame_7 = new QFrame(tab1);
        frame_7->setObjectName(QString::fromUtf8("frame_7"));
        frame_7->setGeometry(QRect(10, 280, 771, 81));
        frame_7->setFrameShape(QFrame::StyledPanel);
        frame_7->setFrameShadow(QFrame::Raised);
        labelStErr = new QLabel(frame_7);
        labelStErr->setObjectName(QString::fromUtf8("labelStErr"));
        labelStErr->setGeometry(QRect(20, 10, 751, 31));
        labelStErr->setFont(font1);
        labelPulse = new QLabel(frame_7);
        labelPulse->setObjectName(QString::fromUtf8("labelPulse"));
        labelPulse->setGeometry(QRect(20, 40, 751, 31));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Bitstream Charter"));
        font2.setPointSize(18);
        font2.setBold(true);
        font2.setWeight(75);
        labelPulse->setFont(font2);
        labelStErr->raise();
        labelPulse->raise();
        labelPulse->raise();
        tabWidget->addTab(tab1, QString());
        frame_7->raise();
        frame->raise();
        frame_3->raise();
        frame_6->raise();
        tab2 = new QWidget();
        tab2->setObjectName(QString::fromUtf8("tab2"));
        QFont font3;
        font3.setPointSize(14);
        tab2->setFont(font3);
        frame_10 = new QFrame(tab2);
        frame_10->setObjectName(QString::fromUtf8("frame_10"));
        frame_10->setGeometry(QRect(10, 280, 781, 86));
        frame_10->setFrameShape(QFrame::StyledPanel);
        frame_10->setFrameShadow(QFrame::Raised);
        Ch12Time = new QPushButton(frame_10);
        Ch12Time->setObjectName(QString::fromUtf8("Ch12Time"));
        Ch12Time->setGeometry(QRect(550, 10, 221, 71));
        Ch12Time->setFont(font1);
        label1_8 = new QLabel(frame_10);
        label1_8->setObjectName(QString::fromUtf8("label1_8"));
        label1_8->setGeometry(QRect(10, 30, 541, 31));
        QFont font4;
        font4.setFamily(QString::fromUtf8("Serif"));
        font4.setPointSize(18);
        font4.setBold(true);
        font4.setWeight(75);
        label1_8->setFont(font4);
        frame_12 = new QFrame(tab2);
        frame_12->setObjectName(QString::fromUtf8("frame_12"));
        frame_12->setGeometry(QRect(10, 10, 781, 86));
        frame_12->setFrameShape(QFrame::StyledPanel);
        frame_12->setFrameShadow(QFrame::Raised);
        Ch9Time = new QPushButton(frame_12);
        Ch9Time->setObjectName(QString::fromUtf8("Ch9Time"));
        Ch9Time->setGeometry(QRect(550, 10, 221, 71));
        Ch9Time->setFont(font1);
        label1_4 = new QLabel(frame_12);
        label1_4->setObjectName(QString::fromUtf8("label1_4"));
        label1_4->setGeometry(QRect(10, 30, 541, 31));
        label1_4->setFont(font4);
        frame_14 = new QFrame(tab2);
        frame_14->setObjectName(QString::fromUtf8("frame_14"));
        frame_14->setGeometry(QRect(10, 100, 781, 86));
        frame_14->setFrameShape(QFrame::StyledPanel);
        frame_14->setFrameShadow(QFrame::Raised);
        Ch10Time = new QPushButton(frame_14);
        Ch10Time->setObjectName(QString::fromUtf8("Ch10Time"));
        Ch10Time->setGeometry(QRect(550, 10, 221, 71));
        Ch10Time->setFont(font1);
        label1_7 = new QLabel(frame_14);
        label1_7->setObjectName(QString::fromUtf8("label1_7"));
        label1_7->setGeometry(QRect(10, 30, 531, 31));
        label1_7->setFont(font4);
        frame_15 = new QFrame(tab2);
        frame_15->setObjectName(QString::fromUtf8("frame_15"));
        frame_15->setGeometry(QRect(10, 190, 781, 86));
        frame_15->setFrameShape(QFrame::StyledPanel);
        frame_15->setFrameShadow(QFrame::Raised);
        Ch11Time = new QPushButton(frame_15);
        Ch11Time->setObjectName(QString::fromUtf8("Ch11Time"));
        Ch11Time->setGeometry(QRect(550, 10, 221, 71));
        Ch11Time->setFont(font1);
        label1_6 = new QLabel(frame_15);
        label1_6->setObjectName(QString::fromUtf8("label1_6"));
        label1_6->setGeometry(QRect(10, 30, 541, 31));
        label1_6->setFont(font4);
        frame_26 = new QFrame(tab2);
        frame_26->setObjectName(QString::fromUtf8("frame_26"));
        frame_26->setGeometry(QRect(10, 370, 781, 51));
        frame_26->setFrameShape(QFrame::StyledPanel);
        frame_26->setFrameShadow(QFrame::Raised);
        label2 = new QLabel(frame_26);
        label2->setObjectName(QString::fromUtf8("label2"));
        label2->setGeometry(QRect(20, 10, 751, 31));
        label2->setFont(font1);
        tabWidget->addTab(tab2, QString());
        tab3 = new QWidget();
        tab3->setObjectName(QString::fromUtf8("tab3"));
        frame_18 = new QFrame(tab3);
        frame_18->setObjectName(QString::fromUtf8("frame_18"));
        frame_18->setGeometry(QRect(410, 30, 376, 86));
        frame_18->setFrameShape(QFrame::StyledPanel);
        frame_18->setFrameShadow(QFrame::Raised);
        Ch20Time = new QPushButton(frame_18);
        Ch20Time->setObjectName(QString::fromUtf8("Ch20Time"));
        Ch20Time->setGeometry(QRect(160, 10, 211, 71));
        Ch20Time->setFont(font1);
        label1_12 = new QLabel(frame_18);
        label1_12->setObjectName(QString::fromUtf8("label1_12"));
        label1_12->setGeometry(QRect(10, 30, 141, 31));
        label1_12->setFont(font1);
        frame_19 = new QFrame(tab3);
        frame_19->setObjectName(QString::fromUtf8("frame_19"));
        frame_19->setGeometry(QRect(410, 150, 376, 86));
        frame_19->setFrameShape(QFrame::StyledPanel);
        frame_19->setFrameShadow(QFrame::Raised);
        Ch21Time = new QPushButton(frame_19);
        Ch21Time->setObjectName(QString::fromUtf8("Ch21Time"));
        Ch21Time->setGeometry(QRect(160, 10, 211, 71));
        Ch21Time->setFont(font1);
        label1_13 = new QLabel(frame_19);
        label1_13->setObjectName(QString::fromUtf8("label1_13"));
        label1_13->setGeometry(QRect(10, 30, 141, 31));
        label1_13->setFont(font1);
        frame_20 = new QFrame(tab3);
        frame_20->setObjectName(QString::fromUtf8("frame_20"));
        frame_20->setGeometry(QRect(10, 30, 376, 86));
        frame_20->setFrameShape(QFrame::StyledPanel);
        frame_20->setFrameShadow(QFrame::Raised);
        Ch17Time = new QPushButton(frame_20);
        Ch17Time->setObjectName(QString::fromUtf8("Ch17Time"));
        Ch17Time->setGeometry(QRect(180, 10, 191, 71));
        Ch17Time->setFont(font1);
        label1_9 = new QLabel(frame_20);
        label1_9->setObjectName(QString::fromUtf8("label1_9"));
        label1_9->setGeometry(QRect(10, 30, 141, 31));
        label1_9->setFont(font1);
        frame_22 = new QFrame(tab3);
        frame_22->setObjectName(QString::fromUtf8("frame_22"));
        frame_22->setGeometry(QRect(10, 150, 376, 86));
        frame_22->setFrameShape(QFrame::StyledPanel);
        frame_22->setFrameShadow(QFrame::Raised);
        Ch18Time = new QPushButton(frame_22);
        Ch18Time->setObjectName(QString::fromUtf8("Ch18Time"));
        Ch18Time->setGeometry(QRect(180, 10, 191, 71));
        Ch18Time->setFont(font1);
        label1_10 = new QLabel(frame_22);
        label1_10->setObjectName(QString::fromUtf8("label1_10"));
        label1_10->setGeometry(QRect(10, 30, 141, 31));
        label1_10->setFont(font1);
        frame_23 = new QFrame(tab3);
        frame_23->setObjectName(QString::fromUtf8("frame_23"));
        frame_23->setGeometry(QRect(10, 260, 376, 86));
        frame_23->setFrameShape(QFrame::StyledPanel);
        frame_23->setFrameShadow(QFrame::Raised);
        Ch19Time = new QPushButton(frame_23);
        Ch19Time->setObjectName(QString::fromUtf8("Ch19Time"));
        Ch19Time->setGeometry(QRect(180, 10, 191, 71));
        Ch19Time->setFont(font1);
        label1_11 = new QLabel(frame_23);
        label1_11->setObjectName(QString::fromUtf8("label1_11"));
        label1_11->setGeometry(QRect(10, 30, 141, 31));
        label1_11->setFont(font1);
        frame_24 = new QFrame(tab3);
        frame_24->setObjectName(QString::fromUtf8("frame_24"));
        frame_24->setGeometry(QRect(410, 260, 376, 86));
        frame_24->setFrameShape(QFrame::StyledPanel);
        frame_24->setFrameShadow(QFrame::Raised);
        Ch22Time = new QPushButton(frame_24);
        Ch22Time->setObjectName(QString::fromUtf8("Ch22Time"));
        Ch22Time->setGeometry(QRect(160, 10, 211, 71));
        Ch22Time->setFont(font1);
        label1_14 = new QLabel(frame_24);
        label1_14->setObjectName(QString::fromUtf8("label1_14"));
        label1_14->setGeometry(QRect(10, 30, 141, 31));
        label1_14->setFont(font1);
        frame_27 = new QFrame(tab3);
        frame_27->setObjectName(QString::fromUtf8("frame_27"));
        frame_27->setGeometry(QRect(10, 370, 781, 51));
        frame_27->setFrameShape(QFrame::StyledPanel);
        frame_27->setFrameShadow(QFrame::Raised);
        label3 = new QLabel(frame_27);
        label3->setObjectName(QString::fromUtf8("label3"));
        label3->setGeometry(QRect(20, 10, 751, 31));
        label3->setFont(font1);
        tabWidget->addTab(tab3, QString());

        verticalLayout->addWidget(tabWidget);

        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        Ch1Time->setText(QApplication::translate("MainWindow", "409.2 Gs", 0, QApplication::UnicodeUTF8));
        label1_3->setText(QApplication::translate("MainWindow", "B1", 0, QApplication::UnicodeUTF8));
        Ch2Time->setText(QApplication::translate("MainWindow", "354.3 Gs", 0, QApplication::UnicodeUTF8));
        label1_2->setText(QApplication::translate("MainWindow", "B2", 0, QApplication::UnicodeUTF8));
        label1->setText(QApplication::translate("MainWindow", "TextLabel", 0, QApplication::UnicodeUTF8));
        labelStErr->setText(QApplication::translate("MainWindow", "TextLabel", 0, QApplication::UnicodeUTF8));
        labelPulse->setText(QApplication::translate("MainWindow", "TextLabel", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab1), QApplication::translate("MainWindow", "  \320\237\321\200\320\270\320\262\321\217\320\267\320\272\320\260  ", 0, QApplication::UnicodeUTF8));
        Ch12Time->setText(QApplication::translate("MainWindow", "6100 Gs", 0, QApplication::UnicodeUTF8));
        label1_8->setText(QApplication::translate("MainWindow", "\320\230\320\274\320\277\321\203\320\273\321\214\321\201 \320\270\320\275\321\202\320\265\320\263\321\200\320\260\321\202\320\276\321\200\320\260 \320\276\321\201\320\275\320\276\320\262\320\275\320\276\320\263\320\276 \321\201\321\202\320\276\320\273\320\260", 0, QApplication::UnicodeUTF8));
        Ch9Time->setText(QApplication::translate("MainWindow", "5000 Gs", 0, QApplication::UnicodeUTF8));
        label1_4->setText(QApplication::translate("MainWindow", "B-\320\270\320\274\320\277\321\203\320\273\321\214\321\201 \320\277\321\200\320\276\320\274\320\265\320\266\321\203\321\202\320\276\321\207\320\275\320\276\320\263\320\276 \321\201\321\202\320\276\320\273\320\260", 0, QApplication::UnicodeUTF8));
        Ch10Time->setText(QApplication::translate("MainWindow", "5100 Gs", 0, QApplication::UnicodeUTF8));
        label1_7->setText(QApplication::translate("MainWindow", "\320\230\320\274\320\277\321\203\320\273\321\214\321\201 \320\270\320\275\321\202\320\265\320\263\321\200\320\260\321\202\320\276\321\200\320\260 \320\277\321\200\320\276\320\274\320\265\320\266\321\203\321\202\320\276\321\207\320\275\320\276\320\263\320\276 \321\201\321\202\320\276\320\273\320\260", 0, QApplication::UnicodeUTF8));
        Ch11Time->setText(QApplication::translate("MainWindow", "16000 Gs", 0, QApplication::UnicodeUTF8));
        label1_6->setText(QApplication::translate("MainWindow", "B-\320\270\320\274\320\277\321\203\320\273\321\214\321\201 \320\276\321\201\320\275\320\276\320\262\320\275\320\276\320\263\320\276 \321\201\321\202\320\276\320\273\320\260", 0, QApplication::UnicodeUTF8));
        label2->setText(QApplication::translate("MainWindow", "TextLabel", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab2), QApplication::translate("MainWindow", "\320\244\320\276\321\200\320\274\320\270\321\200\320\276\320\262\320\260\320\275\320\270\320\265 \321\201\321\202\320\276\320\273\320\276\320\262 ", 0, QApplication::UnicodeUTF8));
        Ch20Time->setText(QApplication::translate("MainWindow", "1000 Gs", 0, QApplication::UnicodeUTF8));
        label1_12->setText(QApplication::translate("MainWindow", "Ch4", 0, QApplication::UnicodeUTF8));
        Ch21Time->setText(QApplication::translate("MainWindow", "1000 Gs", 0, QApplication::UnicodeUTF8));
        label1_13->setText(QApplication::translate("MainWindow", "Ch5", 0, QApplication::UnicodeUTF8));
        Ch17Time->setText(QApplication::translate("MainWindow", "1000 Gs", 0, QApplication::UnicodeUTF8));
        label1_9->setText(QApplication::translate("MainWindow", "Ekp", 0, QApplication::UnicodeUTF8));
        Ch18Time->setText(QApplication::translate("MainWindow", "1000 Gs", 0, QApplication::UnicodeUTF8));
        label1_10->setText(QApplication::translate("MainWindow", "\320\221\320\260\320\274\320\277\321\213", 0, QApplication::UnicodeUTF8));
        Ch19Time->setText(QApplication::translate("MainWindow", "1000 Gs", 0, QApplication::UnicodeUTF8));
        label1_11->setText(QApplication::translate("MainWindow", "Ch3", 0, QApplication::UnicodeUTF8));
        Ch22Time->setText(QApplication::translate("MainWindow", "1000 Gs", 0, QApplication::UnicodeUTF8));
        label1_14->setText(QApplication::translate("MainWindow", "Ch6", 0, QApplication::UnicodeUTF8));
        label3->setText(QApplication::translate("MainWindow", "TextLabel", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab3), QApplication::translate("MainWindow", "  \320\222-\321\202\320\260\320\271\320\274\320\265\321\200", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
