# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'PC_slider.ui'
#
# Created by: PyQt5 UI code generator 5.6
#
# WARNING! All changes made in this file will be lost!

from PyQt5 import QtCore, QtGui, QtWidgets

class Ui_Dialog(object):
    def setupUi(self, Dialog):
        Dialog.setObjectName("Dialog")
        Dialog.resize(515, 221)
        Dialog.setStyleSheet("background:white")
        self.pushButton_set = QtWidgets.QPushButton(Dialog)
        self.pushButton_set.setGeometry(QtCore.QRect(30, 190, 71, 22))
        self.pushButton_set.setStyleSheet("")
        self.pushButton_set.setObjectName("pushButton_set")
        self.widget = QtWidgets.QWidget(Dialog)
        self.widget.setGeometry(QtCore.QRect(30, 10, 71, 171))
        self.widget.setObjectName("widget")
        self.verticalLayout_3 = QtWidgets.QVBoxLayout(self.widget)
        self.verticalLayout_3.setContentsMargins(0, 0, 0, 0)
        self.verticalLayout_3.setObjectName("verticalLayout_3")
        self.lineEdit_pc1 = QtWidgets.QLineEdit(self.widget)
        self.lineEdit_pc1.setMinimumSize(QtCore.QSize(60, 0))
        self.lineEdit_pc1.setMaximumSize(QtCore.QSize(80, 16777215))
        self.lineEdit_pc1.setObjectName("lineEdit_pc1")
        self.verticalLayout_3.addWidget(self.lineEdit_pc1)
        self.lineEdit_pc2 = QtWidgets.QLineEdit(self.widget)
        self.lineEdit_pc2.setMinimumSize(QtCore.QSize(60, 0))
        self.lineEdit_pc2.setMaximumSize(QtCore.QSize(80, 16777215))
        self.lineEdit_pc2.setObjectName("lineEdit_pc2")
        self.verticalLayout_3.addWidget(self.lineEdit_pc2)
        self.lineEdit_pc3 = QtWidgets.QLineEdit(self.widget)
        self.lineEdit_pc3.setMinimumSize(QtCore.QSize(60, 0))
        self.lineEdit_pc3.setMaximumSize(QtCore.QSize(80, 16777215))
        self.lineEdit_pc3.setObjectName("lineEdit_pc3")
        self.verticalLayout_3.addWidget(self.lineEdit_pc3)
        self.widget1 = QtWidgets.QWidget(Dialog)
        self.widget1.setGeometry(QtCore.QRect(110, 10, 381, 171))
        self.widget1.setObjectName("widget1")
        self.horizontalLayout = QtWidgets.QHBoxLayout(self.widget1)
        self.horizontalLayout.setContentsMargins(0, 0, 0, 0)
        self.horizontalLayout.setObjectName("horizontalLayout")
        self.verticalLayout_2 = QtWidgets.QVBoxLayout()
        self.verticalLayout_2.setObjectName("verticalLayout_2")
        self.label = QtWidgets.QLabel(self.widget1)
        self.label.setObjectName("label")
        self.verticalLayout_2.addWidget(self.label)
        self.label_2 = QtWidgets.QLabel(self.widget1)
        self.label_2.setObjectName("label_2")
        self.verticalLayout_2.addWidget(self.label_2)
        self.label_3 = QtWidgets.QLabel(self.widget1)
        self.label_3.setObjectName("label_3")
        self.verticalLayout_2.addWidget(self.label_3)
        self.horizontalLayout.addLayout(self.verticalLayout_2)
        self.verticalLayout = QtWidgets.QVBoxLayout()
        self.verticalLayout.setObjectName("verticalLayout")
        self.PC1Slider = QtWidgets.QSlider(self.widget1)
        self.PC1Slider.setMinimum(-50)
        self.PC1Slider.setMaximum(50)
        self.PC1Slider.setProperty("value", 0)
        self.PC1Slider.setOrientation(QtCore.Qt.Horizontal)
        self.PC1Slider.setObjectName("PC1Slider")
        self.verticalLayout.addWidget(self.PC1Slider)
        self.PC2Slider = QtWidgets.QSlider(self.widget1)
        self.PC2Slider.setMinimum(-50)
        self.PC2Slider.setMaximum(50)
        self.PC2Slider.setOrientation(QtCore.Qt.Horizontal)
        self.PC2Slider.setObjectName("PC2Slider")
        self.verticalLayout.addWidget(self.PC2Slider)
        self.PC3Slider = QtWidgets.QSlider(self.widget1)
        self.PC3Slider.setMinimum(-50)
        self.PC3Slider.setMaximum(50)
        self.PC3Slider.setPageStep(10)
        self.PC3Slider.setOrientation(QtCore.Qt.Horizontal)
        self.PC3Slider.setObjectName("PC3Slider")
        self.verticalLayout.addWidget(self.PC3Slider)
        self.horizontalLayout.addLayout(self.verticalLayout)

        self.retranslateUi(Dialog)
        self.PC1Slider.valueChanged['int'].connect(Dialog.slide)
        self.PC2Slider.valueChanged['int'].connect(Dialog.slide)
        self.PC3Slider.valueChanged['int'].connect(Dialog.slide)
        self.pushButton_set.clicked.connect(Dialog.setPCValue)
        QtCore.QMetaObject.connectSlotsByName(Dialog)

    def retranslateUi(self, Dialog):
        _translate = QtCore.QCoreApplication.translate
        Dialog.setWindowTitle(_translate("Dialog", "Dialog"))
        self.pushButton_set.setText(_translate("Dialog", "Set"))
        self.lineEdit_pc1.setPlaceholderText(_translate("Dialog", "0"))
        self.lineEdit_pc2.setPlaceholderText(_translate("Dialog", "0"))
        self.lineEdit_pc3.setPlaceholderText(_translate("Dialog", "0"))
        self.label.setText(_translate("Dialog", "PC1"))
        self.label_2.setText(_translate("Dialog", "PC2"))
        self.label_3.setText(_translate("Dialog", "PC3"))

