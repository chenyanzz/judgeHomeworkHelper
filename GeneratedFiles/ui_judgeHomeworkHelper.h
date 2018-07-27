/********************************************************************************
** Form generated from reading UI file 'judgeHomeworkHelper.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_JUDGEHOMEWORKHELPER_H
#define UI_JUDGEHOMEWORKHELPER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_judgeHomeworkHelperClass
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QSpacerItem *horizontalSpacer;
    QPushButton *button_diff;
    QTextBrowser *text_rightAnswer;
    QLabel *label_2;
    QTextBrowser *text_stuAnswer;

    void setupUi(QMainWindow *judgeHomeworkHelperClass)
    {
        if (judgeHomeworkHelperClass->objectName().isEmpty())
            judgeHomeworkHelperClass->setObjectName(QStringLiteral("judgeHomeworkHelperClass"));
        judgeHomeworkHelperClass->resize(647, 420);
        centralWidget = new QWidget(judgeHomeworkHelperClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout_2 = new QVBoxLayout(centralWidget);
        verticalLayout_2->setSpacing(20);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(20, 20, 20, 20);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        button_diff = new QPushButton(centralWidget);
        button_diff->setObjectName(QStringLiteral("button_diff"));

        horizontalLayout->addWidget(button_diff);


        verticalLayout_2->addLayout(horizontalLayout);

        text_rightAnswer = new QTextBrowser(centralWidget);
        text_rightAnswer->setObjectName(QStringLiteral("text_rightAnswer"));

        verticalLayout_2->addWidget(text_rightAnswer);

        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));

        verticalLayout_2->addWidget(label_2);

        text_stuAnswer = new QTextBrowser(centralWidget);
        text_stuAnswer->setObjectName(QStringLiteral("text_stuAnswer"));

        verticalLayout_2->addWidget(text_stuAnswer);

        judgeHomeworkHelperClass->setCentralWidget(centralWidget);

        retranslateUi(judgeHomeworkHelperClass);

        QMetaObject::connectSlotsByName(judgeHomeworkHelperClass);
    } // setupUi

    void retranslateUi(QMainWindow *judgeHomeworkHelperClass)
    {
        judgeHomeworkHelperClass->setWindowTitle(QApplication::translate("judgeHomeworkHelperClass", "judgeHomeworkHelper", nullptr));
        label->setText(QApplication::translate("judgeHomeworkHelperClass", "\346\240\207\347\255\224\345\205\263\351\224\256\350\257\215", nullptr));
        button_diff->setText(QApplication::translate("judgeHomeworkHelperClass", "\345\257\271\346\257\224", nullptr));
        text_rightAnswer->setHtml(QApplication::translate("judgeHomeworkHelperClass", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'SimSun'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">A.\350\276\223\345\257\274\347\273\204\347\273\207\357\274\214B.<span style=\" color:#0055ff;\">\344\277\235\346\212\244</span>\347\273\204\347\273\207\357\274\214C.<span style=\" color:#00ff00;\">\345\210\206\347\224\237</span>\347\273\204\347\273\207\357\274\214D.<span style=\" color:#ffff00;\">\346\234\272\346\242\260</span>\347\273\204\347\273\207\357\274\214E.<span style=\" color:#bd0aa5;\">\350\220\245\345\205\273</span>\347\273\204\347\273\207</p></body></html>", nullptr));
        label_2->setText(QApplication::translate("judgeHomeworkHelperClass", "\345\255\246\347\224\237\347\255\224\346\241\210", nullptr));
        text_stuAnswer->setHtml(QApplication::translate("judgeHomeworkHelperClass", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'SimSun'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">A<span style=\" color:#ffff00;\">\346\234\272\346\242\260</span>\347\273\204\347\273\207\343\200\201B<span style=\" color:#0000ff;\">\344\277\235\346\212\244</span>\347\273\204\347\273\207\343\200\201C<span style=\" color:#00ff00;\">\345\210\206\347\224\237</span>\347\273\204\347\273\207\357\274\214D <span style=\" color:#ff0000;\">\350\276\223\345\257\274</span>\347\273\204\347\273\207\357\274\214E\343\200\201<span style=\" color:#d905c4;\">\350\220\245\345\205\273</span>\347\273\204\347\273\207</p></body></html>", nullptr));
    } // retranslateUi

};

namespace Ui {
    class judgeHomeworkHelperClass: public Ui_judgeHomeworkHelperClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_JUDGEHOMEWORKHELPER_H
