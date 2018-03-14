/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QScrollBar *scrollBarWidthImg;
    QLineEdit *lineEditWidthImage;
    QWidget *verticalLayoutWidget_2;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_2;
    QScrollBar *scrollBarHeightImg;
    QLineEdit *lineEditHeightImage;
    QGraphicsView *graphicsView;
    QPushButton *btnGrab;
    QWidget *verticalLayoutWidget_3;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_3;
    QScrollBar *scrollBarOffsetFile;
    QLineEdit *lineEditOffsetFile;
    QPushButton *btnGrayPal;
    QWidget *verticalLayoutWidget_4;
    QVBoxLayout *verticalLayout_4;
    QLabel *label_4;
    QScrollBar *scrollBarOffsetImg;
    QLineEdit *lineEditOffsetImg;
    QWidget *verticalLayoutWidget_5;
    QVBoxLayout *verticalLayout_5;
    QLabel *label_5;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *btnNextPalPicture;
    QPushButton *btnNextPicture;
    QPushButton *btnSaveImg;
    QWidget *verticalLayoutWidget_6;
    QVBoxLayout *verticalLayout_6;
    QLabel *label_7;
    QHBoxLayout *horizontalLayout;
    QPushButton *btnLoadedImg;
    QPushButton *btnActualImg;
    QPushButton *btnRepleace;
    QWidget *verticalLayoutWidget_7;
    QVBoxLayout *verticalLayout_7;
    QLabel *label_6;
    QHBoxLayout *horizontalLayout_3;
    QLineEdit *lineEditLoadImg;
    QPushButton *btnLoad;
    QStatusBar *statusbar;
    QMenuBar *menubar;
    QMenu *menuGrabger;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(861, 546);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        verticalLayoutWidget = new QWidget(centralwidget);
        verticalLayoutWidget->setObjectName(QStringLiteral("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(680, 0, 160, 66));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(verticalLayoutWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label);

        scrollBarWidthImg = new QScrollBar(verticalLayoutWidget);
        scrollBarWidthImg->setObjectName(QStringLiteral("scrollBarWidthImg"));
        scrollBarWidthImg->setOrientation(Qt::Horizontal);

        verticalLayout->addWidget(scrollBarWidthImg);

        lineEditWidthImage = new QLineEdit(verticalLayoutWidget);
        lineEditWidthImage->setObjectName(QStringLiteral("lineEditWidthImage"));

        verticalLayout->addWidget(lineEditWidthImage);

        verticalLayoutWidget_2 = new QWidget(centralwidget);
        verticalLayoutWidget_2->setObjectName(QStringLiteral("verticalLayoutWidget_2"));
        verticalLayoutWidget_2->setGeometry(QRect(680, 80, 160, 66));
        verticalLayout_2 = new QVBoxLayout(verticalLayoutWidget_2);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(verticalLayoutWidget_2);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(label_2);

        scrollBarHeightImg = new QScrollBar(verticalLayoutWidget_2);
        scrollBarHeightImg->setObjectName(QStringLiteral("scrollBarHeightImg"));
        scrollBarHeightImg->setOrientation(Qt::Horizontal);

        verticalLayout_2->addWidget(scrollBarHeightImg);

        lineEditHeightImage = new QLineEdit(verticalLayoutWidget_2);
        lineEditHeightImage->setObjectName(QStringLiteral("lineEditHeightImage"));

        verticalLayout_2->addWidget(lineEditHeightImage);

        graphicsView = new QGraphicsView(centralwidget);
        graphicsView->setObjectName(QStringLiteral("graphicsView"));
        graphicsView->setGeometry(QRect(0, 0, 646, 406));
        btnGrab = new QPushButton(centralwidget);
        btnGrab->setObjectName(QStringLiteral("btnGrab"));
        btnGrab->setGeometry(QRect(720, 320, 80, 23));
        verticalLayoutWidget_3 = new QWidget(centralwidget);
        verticalLayoutWidget_3->setObjectName(QStringLiteral("verticalLayoutWidget_3"));
        verticalLayoutWidget_3->setGeometry(QRect(680, 240, 160, 66));
        verticalLayout_3 = new QVBoxLayout(verticalLayoutWidget_3);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        label_3 = new QLabel(verticalLayoutWidget_3);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(label_3);

        scrollBarOffsetFile = new QScrollBar(verticalLayoutWidget_3);
        scrollBarOffsetFile->setObjectName(QStringLiteral("scrollBarOffsetFile"));
        scrollBarOffsetFile->setOrientation(Qt::Horizontal);

        verticalLayout_3->addWidget(scrollBarOffsetFile);

        lineEditOffsetFile = new QLineEdit(verticalLayoutWidget_3);
        lineEditOffsetFile->setObjectName(QStringLiteral("lineEditOffsetFile"));

        verticalLayout_3->addWidget(lineEditOffsetFile);

        btnGrayPal = new QPushButton(centralwidget);
        btnGrayPal->setObjectName(QStringLiteral("btnGrayPal"));
        btnGrayPal->setGeometry(QRect(680, 350, 151, 23));
        verticalLayoutWidget_4 = new QWidget(centralwidget);
        verticalLayoutWidget_4->setObjectName(QStringLiteral("verticalLayoutWidget_4"));
        verticalLayoutWidget_4->setGeometry(QRect(680, 160, 160, 66));
        verticalLayout_4 = new QVBoxLayout(verticalLayoutWidget_4);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        label_4 = new QLabel(verticalLayoutWidget_4);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setAlignment(Qt::AlignCenter);

        verticalLayout_4->addWidget(label_4);

        scrollBarOffsetImg = new QScrollBar(verticalLayoutWidget_4);
        scrollBarOffsetImg->setObjectName(QStringLiteral("scrollBarOffsetImg"));
        scrollBarOffsetImg->setOrientation(Qt::Horizontal);

        verticalLayout_4->addWidget(scrollBarOffsetImg);

        lineEditOffsetImg = new QLineEdit(verticalLayoutWidget_4);
        lineEditOffsetImg->setObjectName(QStringLiteral("lineEditOffsetImg"));

        verticalLayout_4->addWidget(lineEditOffsetImg);

        verticalLayoutWidget_5 = new QWidget(centralwidget);
        verticalLayoutWidget_5->setObjectName(QStringLiteral("verticalLayoutWidget_5"));
        verticalLayoutWidget_5->setGeometry(QRect(650, 380, 201, 109));
        verticalLayout_5 = new QVBoxLayout(verticalLayoutWidget_5);
        verticalLayout_5->setSpacing(0);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        verticalLayout_5->setSizeConstraint(QLayout::SetMinimumSize);
        verticalLayout_5->setContentsMargins(0, 0, 0, 0);
        label_5 = new QLabel(verticalLayoutWidget_5);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setAlignment(Qt::AlignCenter);

        verticalLayout_5->addWidget(label_5);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setSizeConstraint(QLayout::SetDefaultConstraint);
        horizontalLayout_2->setContentsMargins(0, 0, -1, 7);
        btnNextPalPicture = new QPushButton(verticalLayoutWidget_5);
        btnNextPalPicture->setObjectName(QStringLiteral("btnNextPalPicture"));
        btnNextPalPicture->setEnabled(true);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(btnNextPalPicture->sizePolicy().hasHeightForWidth());
        btnNextPalPicture->setSizePolicy(sizePolicy);
        btnNextPalPicture->setMinimumSize(QSize(91, 46));
        btnNextPalPicture->setMaximumSize(QSize(120, 46));
        QFont font;
        font.setPointSize(12);
        font.setBold(true);
        font.setWeight(75);
        btnNextPalPicture->setFont(font);
        btnNextPalPicture->setLayoutDirection(Qt::LeftToRight);
        btnNextPalPicture->setAutoFillBackground(false);

        horizontalLayout_2->addWidget(btnNextPalPicture);

        btnNextPicture = new QPushButton(verticalLayoutWidget_5);
        btnNextPicture->setObjectName(QStringLiteral("btnNextPicture"));
        btnNextPicture->setEnabled(true);
        sizePolicy.setHeightForWidth(btnNextPicture->sizePolicy().hasHeightForWidth());
        btnNextPicture->setSizePolicy(sizePolicy);
        btnNextPicture->setMinimumSize(QSize(91, 46));
        btnNextPicture->setMaximumSize(QSize(120, 46));
        btnNextPicture->setFont(font);
        btnNextPicture->setLayoutDirection(Qt::LeftToRight);
        btnNextPicture->setAutoFillBackground(false);

        horizontalLayout_2->addWidget(btnNextPicture);


        verticalLayout_5->addLayout(horizontalLayout_2);

        btnSaveImg = new QPushButton(verticalLayoutWidget_5);
        btnSaveImg->setObjectName(QStringLiteral("btnSaveImg"));
        btnSaveImg->setFont(font);

        verticalLayout_5->addWidget(btnSaveImg);

        verticalLayoutWidget_6 = new QWidget(centralwidget);
        verticalLayoutWidget_6->setObjectName(QStringLiteral("verticalLayoutWidget_6"));
        verticalLayoutWidget_6->setGeometry(QRect(30, 410, 221, 89));
        verticalLayout_6 = new QVBoxLayout(verticalLayoutWidget_6);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        verticalLayout_6->setContentsMargins(0, 0, 0, 0);
        label_7 = new QLabel(verticalLayoutWidget_6);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setFont(font);
        label_7->setAlignment(Qt::AlignCenter);

        verticalLayout_6->addWidget(label_7);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        btnLoadedImg = new QPushButton(verticalLayoutWidget_6);
        btnLoadedImg->setObjectName(QStringLiteral("btnLoadedImg"));
        btnLoadedImg->setFont(font);

        horizontalLayout->addWidget(btnLoadedImg);

        btnActualImg = new QPushButton(verticalLayoutWidget_6);
        btnActualImg->setObjectName(QStringLiteral("btnActualImg"));
        QFont font1;
        font1.setPointSize(11);
        font1.setBold(true);
        font1.setWeight(75);
        btnActualImg->setFont(font1);

        horizontalLayout->addWidget(btnActualImg);


        verticalLayout_6->addLayout(horizontalLayout);

        btnRepleace = new QPushButton(verticalLayoutWidget_6);
        btnRepleace->setObjectName(QStringLiteral("btnRepleace"));
        btnRepleace->setFont(font);

        verticalLayout_6->addWidget(btnRepleace);

        verticalLayoutWidget_7 = new QWidget(centralwidget);
        verticalLayoutWidget_7->setObjectName(QStringLiteral("verticalLayoutWidget_7"));
        verticalLayoutWidget_7->setGeometry(QRect(270, 410, 311, 46));
        verticalLayout_7 = new QVBoxLayout(verticalLayoutWidget_7);
        verticalLayout_7->setSpacing(0);
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        verticalLayout_7->setContentsMargins(0, 0, 0, 0);
        label_6 = new QLabel(verticalLayoutWidget_7);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setAlignment(Qt::AlignCenter);

        verticalLayout_7->addWidget(label_6);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        lineEditLoadImg = new QLineEdit(verticalLayoutWidget_7);
        lineEditLoadImg->setObjectName(QStringLiteral("lineEditLoadImg"));

        horizontalLayout_3->addWidget(lineEditLoadImg);

        btnLoad = new QPushButton(verticalLayoutWidget_7);
        btnLoad->setObjectName(QStringLiteral("btnLoad"));
        btnLoad->setFont(font);

        horizontalLayout_3->addWidget(btnLoad);


        verticalLayout_7->addLayout(horizontalLayout_3);

        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        MainWindow->setStatusBar(statusbar);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 861, 20));
        menuGrabger = new QMenu(menubar);
        menuGrabger->setObjectName(QStringLiteral("menuGrabger"));
        MainWindow->setMenuBar(menubar);

        menubar->addAction(menuGrabger->menuAction());

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Grabber Lotus 3", 0));
        label->setText(QApplication::translate("MainWindow", "Size Image", 0));
        label_2->setText(QApplication::translate("MainWindow", "Heigh Image", 0));
        btnGrab->setText(QApplication::translate("MainWindow", "Grab", 0));
        label_3->setText(QApplication::translate("MainWindow", "offset file", 0));
        btnGrayPal->setText(QApplication::translate("MainWindow", "Gray Scalle Pallete", 0));
        label_4->setText(QApplication::translate("MainWindow", "Offset Image", 0));
        label_5->setText(QApplication::translate("MainWindow", "automatic find", 0));
#ifndef QT_NO_ACCESSIBILITY
        btnNextPalPicture->setAccessibleDescription(QString());
#endif // QT_NO_ACCESSIBILITY
        btnNextPalPicture->setText(QApplication::translate("MainWindow", "Next Palete \n"
"Picture", 0));
#ifndef QT_NO_ACCESSIBILITY
        btnNextPicture->setAccessibleDescription(QString());
#endif // QT_NO_ACCESSIBILITY
        btnNextPicture->setText(QApplication::translate("MainWindow", "Next\n"
" Picture", 0));
        btnSaveImg->setText(QApplication::translate("MainWindow", "Save Grabed Image", 0));
        label_7->setText(QApplication::translate("MainWindow", "Show Image", 0));
        btnLoadedImg->setText(QApplication::translate("MainWindow", "Loaded", 0));
        btnActualImg->setText(QApplication::translate("MainWindow", "Actual", 0));
        btnRepleace->setText(QApplication::translate("MainWindow", "Repleace", 0));
        label_6->setText(QApplication::translate("MainWindow", "path to loaded Image", 0));
        btnLoad->setText(QApplication::translate("MainWindow", "Load", 0));
        menuGrabger->setTitle(QApplication::translate("MainWindow", "Grabger", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
