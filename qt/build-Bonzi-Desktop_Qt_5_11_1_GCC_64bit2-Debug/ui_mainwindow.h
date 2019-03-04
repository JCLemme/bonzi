/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QTextEdit *bubble;
    QLineEdit *command;
    QPushButton *go;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QLabel *viewer;
    QSpacerItem *horizontalSpacer_2;
    QComboBox *animationBox;
    QFrame *line;
    QPushButton *demo;
    QPushButton *play;
    QPushButton *stop;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(642, 251);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        bubble = new QTextEdit(centralWidget);
        bubble->setObjectName(QStringLiteral("bubble"));
        bubble->setGeometry(QRect(5, 5, 391, 31));
        command = new QLineEdit(centralWidget);
        command->setObjectName(QStringLiteral("command"));
        command->setGeometry(QRect(5, 220, 311, 25));
        go = new QPushButton(centralWidget);
        go->setObjectName(QStringLiteral("go"));
        go->setGeometry(QRect(318, 220, 81, 25));
        layoutWidget = new QWidget(centralWidget);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(5, 40, 391, 176));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        viewer = new QLabel(layoutWidget);
        viewer->setObjectName(QStringLiteral("viewer"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(viewer->sizePolicy().hasHeightForWidth());
        viewer->setSizePolicy(sizePolicy);
        viewer->setMinimumSize(QSize(200, 160));
        viewer->setMaximumSize(QSize(200, 160));

        horizontalLayout->addWidget(viewer);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        animationBox = new QComboBox(centralWidget);
        animationBox->setObjectName(QStringLiteral("animationBox"));
        animationBox->setGeometry(QRect(420, 10, 216, 25));
        line = new QFrame(centralWidget);
        line->setObjectName(QStringLiteral("line"));
        line->setGeometry(QRect(400, 5, 16, 241));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);
        demo = new QPushButton(centralWidget);
        demo->setObjectName(QStringLiteral("demo"));
        demo->setGeometry(QRect(420, 40, 56, 25));
        play = new QPushButton(centralWidget);
        play->setObjectName(QStringLiteral("play"));
        play->setGeometry(QRect(480, 40, 46, 25));
        stop = new QPushButton(centralWidget);
        stop->setObjectName(QStringLiteral("stop"));
        stop->setGeometry(QRect(545, 40, 89, 25));
        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        go->setText(QApplication::translate("MainWindow", "Go", nullptr));
        viewer->setText(QString());
        demo->setText(QApplication::translate("MainWindow", "Demo", nullptr));
        play->setText(QApplication::translate("MainWindow", "Play", nullptr));
        stop->setText(QApplication::translate("MainWindow", "Stop", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
