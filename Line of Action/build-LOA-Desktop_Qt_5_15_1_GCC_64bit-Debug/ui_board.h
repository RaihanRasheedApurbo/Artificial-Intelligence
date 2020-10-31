/********************************************************************************
** Form generated from reading UI file 'board.ui'
**
** Created by: Qt User Interface Compiler version 5.15.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BOARD_H
#define UI_BOARD_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>

QT_BEGIN_NAMESPACE

class Ui_Board
{
public:

    void setupUi(QDialog *Board)
    {
        if (Board->objectName().isEmpty())
            Board->setObjectName(QString::fromUtf8("Board"));
        Board->resize(918, 609);
        QFont font;
        font.setPointSize(11);
        Board->setFont(font);

        retranslateUi(Board);

        QMetaObject::connectSlotsByName(Board);
    } // setupUi

    void retranslateUi(QDialog *Board)
    {
        Board->setWindowTitle(QCoreApplication::translate("Board", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Board: public Ui_Board {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BOARD_H
