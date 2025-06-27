/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QLabel *lbTitle;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout;
    QLabel *lbEncryptedPlane;
    QPlainTextEdit *ptEncryptedInput;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QLabel *lbProcessList;
    QLineEdit *leProcessSearch;
    QListWidget *lwProcess;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pbRefrsh;
    QPushButton *pbAttach;
    QPushButton *pbDetach;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName("Widget");
        Widget->resize(800, 600);
        gridLayout_2 = new QGridLayout(Widget);
        gridLayout_2->setObjectName("gridLayout_2");
        gridLayout = new QGridLayout();
        gridLayout->setObjectName("gridLayout");
        lbTitle = new QLabel(Widget);
        lbTitle->setObjectName("lbTitle");
        QFont font;
        font.setPointSize(24);
        font.setBold(true);
        lbTitle->setFont(font);

        gridLayout->addWidget(lbTitle, 0, 0, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        lbEncryptedPlane = new QLabel(Widget);
        lbEncryptedPlane->setObjectName("lbEncryptedPlane");
        lbEncryptedPlane->setEnabled(false);
        QFont font1;
        font1.setBold(true);
        lbEncryptedPlane->setFont(font1);

        verticalLayout->addWidget(lbEncryptedPlane);

        ptEncryptedInput = new QPlainTextEdit(Widget);
        ptEncryptedInput->setObjectName("ptEncryptedInput");
        ptEncryptedInput->setEnabled(false);
        ptEncryptedInput->setReadOnly(true);

        verticalLayout->addWidget(ptEncryptedInput);


        horizontalLayout_3->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        lbProcessList = new QLabel(Widget);
        lbProcessList->setObjectName("lbProcessList");
        lbProcessList->setFont(font1);

        horizontalLayout->addWidget(lbProcessList);

        leProcessSearch = new QLineEdit(Widget);
        leProcessSearch->setObjectName("leProcessSearch");

        horizontalLayout->addWidget(leProcessSearch);


        verticalLayout_2->addLayout(horizontalLayout);

        lwProcess = new QListWidget(Widget);
        lwProcess->setObjectName("lwProcess");

        verticalLayout_2->addWidget(lwProcess);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        pbRefrsh = new QPushButton(Widget);
        pbRefrsh->setObjectName("pbRefrsh");

        horizontalLayout_2->addWidget(pbRefrsh);

        pbAttach = new QPushButton(Widget);
        pbAttach->setObjectName("pbAttach");
        pbAttach->setEnabled(true);

        horizontalLayout_2->addWidget(pbAttach);

        pbDetach = new QPushButton(Widget);
        pbDetach->setObjectName("pbDetach");

        horizontalLayout_2->addWidget(pbDetach);


        verticalLayout_2->addLayout(horizontalLayout_2);


        horizontalLayout_3->addLayout(verticalLayout_2);


        gridLayout->addLayout(horizontalLayout_3, 1, 0, 1, 1);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);


        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "Widget", nullptr));
        lbTitle->setText(QCoreApplication::translate("Widget", "\342\214\250\357\270\217 Wizbz Keyboard Security", nullptr));
        lbEncryptedPlane->setText(QCoreApplication::translate("Widget", "encrypted plane", nullptr));
        lbProcessList->setText(QCoreApplication::translate("Widget", "Process List", nullptr));
        pbRefrsh->setText(QCoreApplication::translate("Widget", "Refresh", nullptr));
        pbAttach->setText(QCoreApplication::translate("Widget", "Attach", nullptr));
        pbDetach->setText(QCoreApplication::translate("Widget", "Detach", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
