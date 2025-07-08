/********************************************************************************
** Form generated from reading UI file 'inttoolgui.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INTTOOLGUI_H
#define UI_INTTOOLGUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_IntToolGUI
{
public:
    QWidget *centralwidget;
    QPushButton *ExportButton;
    QPushButton *ImportButton;
    QLabel *label;

    void setupUi(QMainWindow *IntToolGUI)
    {
        if (IntToolGUI->objectName().isEmpty())
            IntToolGUI->setObjectName("IntToolGUI");
        IntToolGUI->resize(450, 40);
        centralwidget = new QWidget(IntToolGUI);
        centralwidget->setObjectName("centralwidget");
        ExportButton = new QPushButton(centralwidget);
        ExportButton->setObjectName("ExportButton");
        ExportButton->setGeometry(QRect(90, 10, 81, 24));
        ImportButton = new QPushButton(centralwidget);
        ImportButton->setObjectName("ImportButton");
        ImportButton->setGeometry(QRect(0, 10, 81, 24));
        label = new QLabel(centralwidget);
        label->setObjectName("label");
        label->setGeometry(QRect(170, 14, 281, 16));
        IntToolGUI->setCentralWidget(centralwidget);

        retranslateUi(IntToolGUI);

        QMetaObject::connectSlotsByName(IntToolGUI);
    } // setupUi

    void retranslateUi(QMainWindow *IntToolGUI)
    {
        IntToolGUI->setWindowTitle(QCoreApplication::translate("IntToolGUI", "IntToolGUI", nullptr));
        ExportButton->setText(QCoreApplication::translate("IntToolGUI", "Export", nullptr));
        ImportButton->setText(QCoreApplication::translate("IntToolGUI", "Import", nullptr));
        label->setText(QCoreApplication::translate("IntToolGUI", "Original Tool By CuckyDev, GUI Tool By Maximos Ve", nullptr));
    } // retranslateUi

};

namespace Ui {
    class IntToolGUI: public Ui_IntToolGUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INTTOOLGUI_H
