/********************************************************************************
** Form generated from reading UI file 'browseSectionWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BROWSESECTIONWINDOW_H
#define UI_BROWSESECTIONWINDOW_H

#include <QtCore/QVariant>
#include <QtWebEngineWidgets/QWebEngineView>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_browseSectionWindow
{
public:
    QVBoxLayout *verticalLayout_2;
    QLabel *label_title;
    QLabel *label;
    QWebEngineView *webview_rawHtml;
    QLabel *label_2;
    QWebEngineView *webview_correctAnswer;
    QLabel *label_3;
    QPlainTextEdit *text_keywords;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QLabel *label_process;
    QPushButton *btn_prev;
    QPushButton *btn_next;
    QPushButton *btn_cancel;
    QPushButton *btn_save;

    void setupUi(QWidget *browseSectionWindow)
    {
        if (browseSectionWindow->objectName().isEmpty())
            browseSectionWindow->setObjectName(QStringLiteral("browseSectionWindow"));
        browseSectionWindow->resize(740, 572);
        QFont font;
        font.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        browseSectionWindow->setFont(font);
        browseSectionWindow->setAutoFillBackground(true);
        verticalLayout_2 = new QVBoxLayout(browseSectionWindow);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        label_title = new QLabel(browseSectionWindow);
        label_title->setObjectName(QStringLiteral("label_title"));
        label_title->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(label_title);

        label = new QLabel(browseSectionWindow);
        label->setObjectName(QStringLiteral("label"));

        verticalLayout_2->addWidget(label);

        webview_rawHtml = new QWebEngineView(browseSectionWindow);
        webview_rawHtml->setObjectName(QStringLiteral("webview_rawHtml"));
        webview_rawHtml->setUrl(QUrl(QStringLiteral("about:blank")));

        verticalLayout_2->addWidget(webview_rawHtml);

        label_2 = new QLabel(browseSectionWindow);
        label_2->setObjectName(QStringLiteral("label_2"));

        verticalLayout_2->addWidget(label_2);

        webview_correctAnswer = new QWebEngineView(browseSectionWindow);
        webview_correctAnswer->setObjectName(QStringLiteral("webview_correctAnswer"));
        webview_correctAnswer->setUrl(QUrl(QStringLiteral("about:blank")));

        verticalLayout_2->addWidget(webview_correctAnswer);

        label_3 = new QLabel(browseSectionWindow);
        label_3->setObjectName(QStringLiteral("label_3"));

        verticalLayout_2->addWidget(label_3);

        text_keywords = new QPlainTextEdit(browseSectionWindow);
        text_keywords->setObjectName(QStringLiteral("text_keywords"));

        verticalLayout_2->addWidget(text_keywords);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        label_process = new QLabel(browseSectionWindow);
        label_process->setObjectName(QStringLiteral("label_process"));

        horizontalLayout_2->addWidget(label_process);

        btn_prev = new QPushButton(browseSectionWindow);
        btn_prev->setObjectName(QStringLiteral("btn_prev"));

        horizontalLayout_2->addWidget(btn_prev);

        btn_next = new QPushButton(browseSectionWindow);
        btn_next->setObjectName(QStringLiteral("btn_next"));

        horizontalLayout_2->addWidget(btn_next);

        btn_cancel = new QPushButton(browseSectionWindow);
        btn_cancel->setObjectName(QStringLiteral("btn_cancel"));

        horizontalLayout_2->addWidget(btn_cancel);

        btn_save = new QPushButton(browseSectionWindow);
        btn_save->setObjectName(QStringLiteral("btn_save"));

        horizontalLayout_2->addWidget(btn_save);


        verticalLayout_2->addLayout(horizontalLayout_2);


        retranslateUi(browseSectionWindow);

        QMetaObject::connectSlotsByName(browseSectionWindow);
    } // setupUi

    void retranslateUi(QWidget *browseSectionWindow)
    {
        browseSectionWindow->setWindowTitle(QApplication::translate("browseSectionWindow", "Form", nullptr));
        label_title->setText(QApplication::translate("browseSectionWindow", "TextLabel", nullptr));
        label->setText(QApplication::translate("browseSectionWindow", "\345\216\237\351\242\230\357\274\232", nullptr));
        label_2->setText(QApplication::translate("browseSectionWindow", "\346\240\207\345\207\206\347\255\224\346\241\210\357\274\232", nullptr));
        label_3->setText(QApplication::translate("browseSectionWindow", "\350\257\267\350\276\223\345\205\245\345\205\263\351\224\256\350\257\215\357\274\210\347\251\272\346\240\274\345\210\206\351\232\224\357\274\211\357\274\232", nullptr));
        label_process->setText(QApplication::translate("browseSectionWindow", "TextLabel", nullptr));
        btn_prev->setText(QApplication::translate("browseSectionWindow", "\344\270\212\344\270\200\351\242\230", nullptr));
        btn_next->setText(QApplication::translate("browseSectionWindow", "\344\270\213\344\270\200\351\242\230", nullptr));
        btn_cancel->setText(QApplication::translate("browseSectionWindow", "\345\217\226\346\266\210", nullptr));
        btn_save->setText(QApplication::translate("browseSectionWindow", "\347\241\256\345\256\232", nullptr));
    } // retranslateUi

};

namespace Ui {
    class browseSectionWindow: public Ui_browseSectionWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BROWSESECTIONWINDOW_H
