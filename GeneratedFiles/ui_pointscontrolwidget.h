/********************************************************************************
** Form generated from reading UI file 'pointscontrolwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_POINTSCONTROLWIDGET_H
#define UI_POINTSCONTROLWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PointsControlWidget
{
public:
    QGridLayout *gridLayout;
    QGroupBox *groupBox;
    QPushButton *pushButtonCalc;
    QPushButton *pushButtonCalcDC;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_2;
    QLabel *label_2;
    QSlider *horizontalSliderLen;
    QSpinBox *spinBoxLen;
    QLabel *label_3;
    QSlider *horizontalSliderPeriod;
    QSpinBox *spinBoxPeriod;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QSpinBox *spinBoxNum;
    QPushButton *pushButtonGenerateRandom;
    QPushButton *pushButtonGenerateBlueNoise;
    QPushButton *pushButtonGenerateMulticlassBlueNoise;
    QPushButton *pushButtonGenerateNormal;
    QPushButton *pushButtonGenerateBlueNoiseNormal;
    QPushButton *pushButtonGenerateMultiNormal;
    QPushButton *pushButtonGenerateSequence;
    QPushButton *pushButtonGenerateSequence2D;
    QPushButton *pushButtonGenerateSpiral;
    QCheckBox *checkBoxShowBackground;
    QPushButton *pushButtonUpdateLayer;
    QCheckBox *checkBoxPicking;
    QTextEdit *textEditResult;

    void setupUi(QWidget *PointsControlWidget)
    {
        if (PointsControlWidget->objectName().isEmpty())
            PointsControlWidget->setObjectName(QStringLiteral("PointsControlWidget"));
        PointsControlWidget->resize(734, 771);
        gridLayout = new QGridLayout(PointsControlWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        groupBox = new QGroupBox(PointsControlWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        pushButtonCalc = new QPushButton(groupBox);
        pushButtonCalc->setObjectName(QStringLiteral("pushButtonCalc"));
        pushButtonCalc->setGeometry(QRect(10, 22, 75, 23));
        pushButtonCalcDC = new QPushButton(groupBox);
        pushButtonCalcDC->setObjectName(QStringLiteral("pushButtonCalcDC"));
        pushButtonCalcDC->setGeometry(QRect(10, 51, 75, 23));

        gridLayout->addWidget(groupBox, 0, 0, 1, 2);

        groupBox_3 = new QGroupBox(PointsControlWidget);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        gridLayout_2 = new QGridLayout(groupBox_3);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        label_2 = new QLabel(groupBox_3);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout_2->addWidget(label_2, 0, 0, 1, 1);

        horizontalSliderLen = new QSlider(groupBox_3);
        horizontalSliderLen->setObjectName(QStringLiteral("horizontalSliderLen"));
        horizontalSliderLen->setMinimum(5);
        horizontalSliderLen->setMaximum(100);
        horizontalSliderLen->setSingleStep(5);
        horizontalSliderLen->setValue(10);
        horizontalSliderLen->setOrientation(Qt::Horizontal);

        gridLayout_2->addWidget(horizontalSliderLen, 0, 1, 1, 1);

        spinBoxLen = new QSpinBox(groupBox_3);
        spinBoxLen->setObjectName(QStringLiteral("spinBoxLen"));
        spinBoxLen->setMinimum(5);
        spinBoxLen->setMaximum(100);
        spinBoxLen->setSingleStep(5);
        spinBoxLen->setValue(10);

        gridLayout_2->addWidget(spinBoxLen, 0, 2, 1, 1);

        label_3 = new QLabel(groupBox_3);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout_2->addWidget(label_3, 1, 0, 1, 1);

        horizontalSliderPeriod = new QSlider(groupBox_3);
        horizontalSliderPeriod->setObjectName(QStringLiteral("horizontalSliderPeriod"));
        horizontalSliderPeriod->setMinimum(1);
        horizontalSliderPeriod->setMaximum(10);
        horizontalSliderPeriod->setOrientation(Qt::Horizontal);

        gridLayout_2->addWidget(horizontalSliderPeriod, 1, 1, 1, 1);

        spinBoxPeriod = new QSpinBox(groupBox_3);
        spinBoxPeriod->setObjectName(QStringLiteral("spinBoxPeriod"));
        spinBoxPeriod->setMinimum(1);
        spinBoxPeriod->setMaximum(4);

        gridLayout_2->addWidget(spinBoxPeriod, 1, 2, 1, 1);


        gridLayout->addWidget(groupBox_3, 1, 0, 1, 2);

        groupBox_2 = new QGroupBox(PointsControlWidget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        verticalLayout_2 = new QVBoxLayout(groupBox_2);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(groupBox_2);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        spinBoxNum = new QSpinBox(groupBox_2);
        spinBoxNum->setObjectName(QStringLiteral("spinBoxNum"));

        horizontalLayout->addWidget(spinBoxNum);


        verticalLayout_2->addLayout(horizontalLayout);

        pushButtonGenerateRandom = new QPushButton(groupBox_2);
        pushButtonGenerateRandom->setObjectName(QStringLiteral("pushButtonGenerateRandom"));

        verticalLayout_2->addWidget(pushButtonGenerateRandom);

        pushButtonGenerateBlueNoise = new QPushButton(groupBox_2);
        pushButtonGenerateBlueNoise->setObjectName(QStringLiteral("pushButtonGenerateBlueNoise"));

        verticalLayout_2->addWidget(pushButtonGenerateBlueNoise);

        pushButtonGenerateMulticlassBlueNoise = new QPushButton(groupBox_2);
        pushButtonGenerateMulticlassBlueNoise->setObjectName(QStringLiteral("pushButtonGenerateMulticlassBlueNoise"));

        verticalLayout_2->addWidget(pushButtonGenerateMulticlassBlueNoise);

        pushButtonGenerateNormal = new QPushButton(groupBox_2);
        pushButtonGenerateNormal->setObjectName(QStringLiteral("pushButtonGenerateNormal"));

        verticalLayout_2->addWidget(pushButtonGenerateNormal);

        pushButtonGenerateBlueNoiseNormal = new QPushButton(groupBox_2);
        pushButtonGenerateBlueNoiseNormal->setObjectName(QStringLiteral("pushButtonGenerateBlueNoiseNormal"));

        verticalLayout_2->addWidget(pushButtonGenerateBlueNoiseNormal);

        pushButtonGenerateMultiNormal = new QPushButton(groupBox_2);
        pushButtonGenerateMultiNormal->setObjectName(QStringLiteral("pushButtonGenerateMultiNormal"));

        verticalLayout_2->addWidget(pushButtonGenerateMultiNormal);

        pushButtonGenerateSequence = new QPushButton(groupBox_2);
        pushButtonGenerateSequence->setObjectName(QStringLiteral("pushButtonGenerateSequence"));

        verticalLayout_2->addWidget(pushButtonGenerateSequence);

        pushButtonGenerateSequence2D = new QPushButton(groupBox_2);
        pushButtonGenerateSequence2D->setObjectName(QStringLiteral("pushButtonGenerateSequence2D"));

        verticalLayout_2->addWidget(pushButtonGenerateSequence2D);

        pushButtonGenerateSpiral = new QPushButton(groupBox_2);
        pushButtonGenerateSpiral->setObjectName(QStringLiteral("pushButtonGenerateSpiral"));

        verticalLayout_2->addWidget(pushButtonGenerateSpiral);


        gridLayout->addWidget(groupBox_2, 2, 0, 1, 2);

        checkBoxShowBackground = new QCheckBox(PointsControlWidget);
        checkBoxShowBackground->setObjectName(QStringLiteral("checkBoxShowBackground"));

        gridLayout->addWidget(checkBoxShowBackground, 3, 0, 1, 1);

        pushButtonUpdateLayer = new QPushButton(PointsControlWidget);
        pushButtonUpdateLayer->setObjectName(QStringLiteral("pushButtonUpdateLayer"));

        gridLayout->addWidget(pushButtonUpdateLayer, 3, 1, 2, 1);

        checkBoxPicking = new QCheckBox(PointsControlWidget);
        checkBoxPicking->setObjectName(QStringLiteral("checkBoxPicking"));

        gridLayout->addWidget(checkBoxPicking, 4, 0, 1, 1);

        textEditResult = new QTextEdit(PointsControlWidget);
        textEditResult->setObjectName(QStringLiteral("textEditResult"));
        textEditResult->setEnabled(false);

        gridLayout->addWidget(textEditResult, 5, 0, 1, 2);


        retranslateUi(PointsControlWidget);

        QMetaObject::connectSlotsByName(PointsControlWidget);
    } // setupUi

    void retranslateUi(QWidget *PointsControlWidget)
    {
        PointsControlWidget->setWindowTitle(QApplication::translate("PointsControlWidget", "PointsControlWidget", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("PointsControlWidget", "\350\256\241\347\256\227\346\234\200\345\260\217\350\267\235\347\246\273\347\202\271\345\257\271", Q_NULLPTR));
        pushButtonCalc->setText(QApplication::translate("PointsControlWidget", "\345\270\270\350\247\204\346\226\271\346\263\225", Q_NULLPTR));
        pushButtonCalcDC->setText(QApplication::translate("PointsControlWidget", "\345\210\206\346\262\273\346\226\271\346\263\225", Q_NULLPTR));
        groupBox_3->setTitle(QApplication::translate("PointsControlWidget", "\351\207\207\346\240\267\350\256\276\347\275\256", Q_NULLPTR));
        label_2->setText(QApplication::translate("PointsControlWidget", "\351\207\207\346\240\267\347\202\271\346\225\260\357\274\232", Q_NULLPTR));
        label_3->setText(QApplication::translate("PointsControlWidget", "\345\207\275\346\225\260\345\221\250\346\234\237\357\274\232", Q_NULLPTR));
        groupBox_2->setTitle(QApplication::translate("PointsControlWidget", "\347\224\237\346\210\220\351\232\217\346\234\272\347\202\271", Q_NULLPTR));
        label->setText(QApplication::translate("PointsControlWidget", "\351\232\217\346\234\272\347\202\271\346\225\260\351\207\217\357\274\232", Q_NULLPTR));
        pushButtonGenerateRandom->setText(QApplication::translate("PointsControlWidget", "\347\224\237\346\210\220\351\232\217\346\234\272\347\202\271", Q_NULLPTR));
        pushButtonGenerateBlueNoise->setText(QApplication::translate("PointsControlWidget", "\347\224\237\346\210\220\350\223\235\345\231\252\345\243\260", Q_NULLPTR));
        pushButtonGenerateMulticlassBlueNoise->setText(QApplication::translate("PointsControlWidget", "\347\224\237\346\210\220\345\244\232\347\273\204\350\223\235\345\231\252\345\243\260", Q_NULLPTR));
        pushButtonGenerateNormal->setText(QApplication::translate("PointsControlWidget", "\347\224\237\346\210\220\346\255\243\346\200\201\345\210\206\345\270\203", Q_NULLPTR));
        pushButtonGenerateBlueNoiseNormal->setText(QApplication::translate("PointsControlWidget", "\347\224\237\346\210\220\350\223\235\345\231\252\345\243\260\346\255\243\346\200\201\345\210\206\345\270\203", Q_NULLPTR));
        pushButtonGenerateMultiNormal->setText(QApplication::translate("PointsControlWidget", "\347\224\237\346\210\220\345\244\232\344\270\252\346\255\243\346\200\201\345\210\206\345\270\203", Q_NULLPTR));
        pushButtonGenerateSequence->setText(QApplication::translate("PointsControlWidget", "\347\224\237\346\210\220\345\272\217\345\210\227", Q_NULLPTR));
        pushButtonGenerateSequence2D->setText(QApplication::translate("PointsControlWidget", "\347\224\237\346\210\220\344\272\214\347\273\264\345\272\217\345\210\227", Q_NULLPTR));
        pushButtonGenerateSpiral->setText(QApplication::translate("PointsControlWidget", "\347\224\237\346\210\220\350\236\272\346\227\213\347\202\271", Q_NULLPTR));
        checkBoxShowBackground->setText(QApplication::translate("PointsControlWidget", "\346\230\276\347\244\272\350\203\214\346\231\257", Q_NULLPTR));
        pushButtonUpdateLayer->setText(QApplication::translate("PointsControlWidget", "\346\233\264\346\226\260\345\275\223\345\211\215\345\261\202", Q_NULLPTR));
        checkBoxPicking->setText(QApplication::translate("PointsControlWidget", "\344\272\244\344\272\222\351\200\211\347\202\271", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class PointsControlWidget: public Ui_PointsControlWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_POINTSCONTROLWIDGET_H
