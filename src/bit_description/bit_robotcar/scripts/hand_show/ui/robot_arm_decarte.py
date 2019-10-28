# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'robot_arm_decarte.ui'
#
# Created by: PyQt5 UI code generator 5.6
#
# WARNING! All changes made in this file will be lost!

from PyQt5 import QtCore, QtGui, QtWidgets

class Ui_Dialog(object):
    def setupUi(self, Dialog):
        Dialog.setObjectName("Dialog")
        Dialog.resize(517, 273)
        Dialog.setStyleSheet("background:white")
        self.horizontalLayout_2 = QtWidgets.QHBoxLayout(Dialog)
        self.horizontalLayout_2.setContentsMargins(0, 0, 0, 0)
        self.horizontalLayout_2.setObjectName("horizontalLayout_2")
        self.widget = QtWidgets.QWidget(Dialog)
        self.widget.setStyleSheet("")
        self.widget.setObjectName("widget")
        self.gridLayout_2 = QtWidgets.QGridLayout(self.widget)
        self.gridLayout_2.setContentsMargins(9, 0, 9, 12)
        self.gridLayout_2.setObjectName("gridLayout_2")
        self.gridLayout_3 = QtWidgets.QGridLayout()
        self.gridLayout_3.setObjectName("gridLayout_3")
        self.pushButton_z_down = QtWidgets.QPushButton(self.widget)
        self.pushButton_z_down.setMinimumSize(QtCore.QSize(22, 22))
        self.pushButton_z_down.setStyleSheet("QPushButton{background:#F76677;border-radius:5px;}\n"
"QPushButton:hover{font-size:18px;}")
        self.pushButton_z_down.setDefault(False)
        self.pushButton_z_down.setObjectName("pushButton_z_down")
        self.gridLayout_3.addWidget(self.pushButton_z_down, 3, 4, 1, 1)
        self.pushButton_x_up = QtWidgets.QPushButton(self.widget)
        self.pushButton_x_up.setMinimumSize(QtCore.QSize(22, 22))
        self.pushButton_x_up.setToolTip("")
        self.pushButton_x_up.setWhatsThis("")
        self.pushButton_x_up.setStyleSheet("QPushButton{background:#6DDF6D;border-radius:5px;}\n"
"QPushButton:hover{font-size:18px;}")
        self.pushButton_x_up.setCheckable(False)
        self.pushButton_x_up.setObjectName("pushButton_x_up")
        self.gridLayout_3.addWidget(self.pushButton_x_up, 2, 0, 1, 1)
        self.label_7 = QtWidgets.QLabel(self.widget)
        self.label_7.setMaximumSize(QtCore.QSize(16777215, 20))
        self.label_7.setStyleSheet("\n"
"font: 16pt \"Sans Serif\";")
        self.label_7.setAlignment(QtCore.Qt.AlignCenter)
        self.label_7.setObjectName("label_7")
        self.gridLayout_3.addWidget(self.label_7, 0, 2, 1, 1)
        self.dial_z = QtWidgets.QDial(self.widget)
        self.dial_z.setMaximumSize(QtCore.QSize(116, 116))
        self.dial_z.setStyleSheet("background:#F7D674;border-radius:5px;")
        self.dial_z.setMaximum(90)
        self.dial_z.setObjectName("dial_z")
        self.gridLayout_3.addWidget(self.dial_z, 1, 4, 1, 1)
        self.pushButton_y_down = QtWidgets.QPushButton(self.widget)
        self.pushButton_y_down.setMinimumSize(QtCore.QSize(22, 22))
        self.pushButton_y_down.setStyleSheet("QPushButton{background:#F76677;border-radius:5px;}\n"
"QPushButton:hover{font-size:18px;}")
        self.pushButton_y_down.setDefault(False)
        self.pushButton_y_down.setObjectName("pushButton_y_down")
        self.gridLayout_3.addWidget(self.pushButton_y_down, 3, 2, 1, 1)
        self.pushButton_x_down = QtWidgets.QPushButton(self.widget)
        self.pushButton_x_down.setMinimumSize(QtCore.QSize(22, 22))
        self.pushButton_x_down.setStyleSheet("QPushButton{background:#F76677;border-radius:5px;}\n"
"QPushButton:hover{font-size:18px;}")
        self.pushButton_x_down.setDefault(False)
        self.pushButton_x_down.setObjectName("pushButton_x_down")
        self.gridLayout_3.addWidget(self.pushButton_x_down, 3, 0, 1, 1)
        self.dial_y = QtWidgets.QDial(self.widget)
        self.dial_y.setStyleSheet("background:#F7D674;border-radius:5px;")
        self.dial_y.setMaximum(90)
        self.dial_y.setObjectName("dial_y")
        self.gridLayout_3.addWidget(self.dial_y, 1, 2, 1, 1)
        self.label_6 = QtWidgets.QLabel(self.widget)
        self.label_6.setMaximumSize(QtCore.QSize(16777215, 20))
        self.label_6.setStyleSheet("\n"
"font: 16pt \"Sans Serif\";")
        self.label_6.setAlignment(QtCore.Qt.AlignCenter)
        self.label_6.setObjectName("label_6")
        self.gridLayout_3.addWidget(self.label_6, 0, 0, 1, 1)
        self.label_8 = QtWidgets.QLabel(self.widget)
        self.label_8.setMaximumSize(QtCore.QSize(16777215, 20))
        self.label_8.setStyleSheet("\n"
"font: 16pt \"Sans Serif\";")
        self.label_8.setAlignment(QtCore.Qt.AlignCenter)
        self.label_8.setObjectName("label_8")
        self.gridLayout_3.addWidget(self.label_8, 0, 4, 1, 1)
        self.pushButton_y_up = QtWidgets.QPushButton(self.widget)
        self.pushButton_y_up.setMinimumSize(QtCore.QSize(22, 22))
        self.pushButton_y_up.setToolTip("")
        self.pushButton_y_up.setWhatsThis("")
        self.pushButton_y_up.setStyleSheet("QPushButton{background:#6DDF6D;border-radius:5px;}\n"
"QPushButton:hover{font-size:18px;}")
        self.pushButton_y_up.setCheckable(False)
        self.pushButton_y_up.setObjectName("pushButton_y_up")
        self.gridLayout_3.addWidget(self.pushButton_y_up, 2, 2, 1, 1)
        self.dial_x = QtWidgets.QDial(self.widget)
        self.dial_x.setMaximumSize(QtCore.QSize(100, 100))
        self.dial_x.setStyleSheet("background:#F7D674;border-radius:5px;")
        self.dial_x.setMaximum(90)
        self.dial_x.setObjectName("dial_x")
        self.gridLayout_3.addWidget(self.dial_x, 1, 0, 1, 1)
        self.pushButton_z_up = QtWidgets.QPushButton(self.widget)
        self.pushButton_z_up.setMinimumSize(QtCore.QSize(22, 22))
        self.pushButton_z_up.setToolTip("")
        self.pushButton_z_up.setWhatsThis("")
        self.pushButton_z_up.setStyleSheet("QPushButton{background:#6DDF6D;border-radius:5px;}\n"
"QPushButton:hover{font-size:18px;}")
        self.pushButton_z_up.setCheckable(False)
        self.pushButton_z_up.setObjectName("pushButton_z_up")
        self.gridLayout_3.addWidget(self.pushButton_z_up, 2, 4, 1, 1)
        self.pushButton_x_up_angle = QtWidgets.QPushButton(self.widget)
        self.pushButton_x_up_angle.setMinimumSize(QtCore.QSize(22, 22))
        self.pushButton_x_up_angle.setMaximumSize(QtCore.QSize(50, 16777215))
        self.pushButton_x_up_angle.setToolTip("")
        self.pushButton_x_up_angle.setWhatsThis("")
        self.pushButton_x_up_angle.setStyleSheet("QPushButton{background:#6DDF6D;border-radius:5px;}\n"
"QPushButton:hover{font-size:18px;}")
        self.pushButton_x_up_angle.setCheckable(False)
        self.pushButton_x_up_angle.setObjectName("pushButton_x_up_angle")
        self.gridLayout_3.addWidget(self.pushButton_x_up_angle, 0, 1, 1, 1)
        self.pushButton_y_down_angle = QtWidgets.QPushButton(self.widget)
        self.pushButton_y_down_angle.setMinimumSize(QtCore.QSize(22, 22))
        self.pushButton_y_down_angle.setStyleSheet("QPushButton{background:#F76677;border-radius:5px;}\n"
"QPushButton:hover{font-size:18px;}")
        self.pushButton_y_down_angle.setDefault(False)
        self.pushButton_y_down_angle.setObjectName("pushButton_y_down_angle")
        self.gridLayout_3.addWidget(self.pushButton_y_down_angle, 1, 3, 1, 1)
        self.pushButton_y_up_angle = QtWidgets.QPushButton(self.widget)
        self.pushButton_y_up_angle.setMinimumSize(QtCore.QSize(22, 22))
        self.pushButton_y_up_angle.setMaximumSize(QtCore.QSize(50, 16777215))
        self.pushButton_y_up_angle.setToolTip("")
        self.pushButton_y_up_angle.setWhatsThis("")
        self.pushButton_y_up_angle.setStyleSheet("QPushButton{background:#6DDF6D;border-radius:5px;}\n"
"QPushButton:hover{font-size:18px;}")
        self.pushButton_y_up_angle.setCheckable(False)
        self.pushButton_y_up_angle.setObjectName("pushButton_y_up_angle")
        self.gridLayout_3.addWidget(self.pushButton_y_up_angle, 0, 3, 1, 1)
        self.pushButton_x_down_angle = QtWidgets.QPushButton(self.widget)
        self.pushButton_x_down_angle.setMinimumSize(QtCore.QSize(22, 22))
        self.pushButton_x_down_angle.setStyleSheet("QPushButton{background:#F76677;border-radius:5px;}\n"
"QPushButton:hover{font-size:18px;}")
        self.pushButton_x_down_angle.setDefault(False)
        self.pushButton_x_down_angle.setObjectName("pushButton_x_down_angle")
        self.gridLayout_3.addWidget(self.pushButton_x_down_angle, 1, 1, 1, 1)
        self.pushButton_z_up_angle = QtWidgets.QPushButton(self.widget)
        self.pushButton_z_up_angle.setMinimumSize(QtCore.QSize(22, 22))
        self.pushButton_z_up_angle.setMaximumSize(QtCore.QSize(50, 16777215))
        self.pushButton_z_up_angle.setToolTip("")
        self.pushButton_z_up_angle.setWhatsThis("")
        self.pushButton_z_up_angle.setStyleSheet("QPushButton{background:#6DDF6D;border-radius:5px;}\n"
"QPushButton:hover{font-size:18px;}")
        self.pushButton_z_up_angle.setCheckable(False)
        self.pushButton_z_up_angle.setObjectName("pushButton_z_up_angle")
        self.gridLayout_3.addWidget(self.pushButton_z_up_angle, 0, 5, 1, 1)
        self.pushButton_z_down_angle = QtWidgets.QPushButton(self.widget)
        self.pushButton_z_down_angle.setMinimumSize(QtCore.QSize(22, 22))
        self.pushButton_z_down_angle.setStyleSheet("QPushButton{background:#F76677;border-radius:5px;}\n"
"QPushButton:hover{font-size:18px;}")
        self.pushButton_z_down_angle.setDefault(False)
        self.pushButton_z_down_angle.setObjectName("pushButton_z_down_angle")
        self.gridLayout_3.addWidget(self.pushButton_z_down_angle, 1, 5, 1, 1)
        self.gridLayout_2.addLayout(self.gridLayout_3, 0, 0, 1, 1)
        self.horizontalLayout = QtWidgets.QHBoxLayout()
        self.horizontalLayout.setObjectName("horizontalLayout")
        self.pushButton_set_angle = QtWidgets.QPushButton(self.widget)
        self.pushButton_set_angle.setObjectName("pushButton_set_angle")
        self.horizontalLayout.addWidget(self.pushButton_set_angle)
        self.lineEdit_delta = QtWidgets.QLineEdit(self.widget)
        self.lineEdit_delta.setObjectName("lineEdit_delta")
        self.horizontalLayout.addWidget(self.lineEdit_delta)
        self.lineEdit_theta = QtWidgets.QLineEdit(self.widget)
        self.lineEdit_theta.setObjectName("lineEdit_theta")
        self.horizontalLayout.addWidget(self.lineEdit_theta)
        self.gridLayout_2.addLayout(self.horizontalLayout, 1, 0, 1, 1)
        self.horizontalLayout_2.addWidget(self.widget)

        self.retranslateUi(Dialog)
        self.pushButton_set_angle.clicked.connect(Dialog.set_angle)
        self.pushButton_x_up.clicked.connect(Dialog.set_xyz)
        self.pushButton_y_up.clicked.connect(Dialog.set_xyz)
        self.pushButton_z_up.clicked.connect(Dialog.set_xyz)
        self.pushButton_x_down.clicked.connect(Dialog.set_xyz)
        self.pushButton_y_down.clicked.connect(Dialog.set_xyz)
        self.pushButton_z_down.clicked.connect(Dialog.set_xyz)
        self.pushButton_x_up_angle.clicked.connect(Dialog.increase_angle)
        self.pushButton_y_up_angle.clicked.connect(Dialog.increase_angle)
        self.pushButton_z_up_angle.clicked.connect(Dialog.increase_angle)
        self.pushButton_x_down_angle.clicked.connect(Dialog.increase_angle)
        self.pushButton_y_down_angle.clicked.connect(Dialog.increase_angle)
        self.pushButton_z_down_angle.clicked.connect(Dialog.increase_angle)
        QtCore.QMetaObject.connectSlotsByName(Dialog)

    def retranslateUi(self, Dialog):
        _translate = QtCore.QCoreApplication.translate
        Dialog.setWindowTitle(_translate("Dialog", "Robot Arm"))
        self.pushButton_z_down.setText(_translate("Dialog", "-"))
        self.pushButton_x_up.setText(_translate("Dialog", "+"))
        self.label_7.setText(_translate("Dialog", "y"))
        self.pushButton_y_down.setText(_translate("Dialog", "-"))
        self.pushButton_x_down.setText(_translate("Dialog", "-"))
        self.label_6.setText(_translate("Dialog", "x"))
        self.label_8.setText(_translate("Dialog", "z"))
        self.pushButton_y_up.setText(_translate("Dialog", "+"))
        self.pushButton_z_up.setText(_translate("Dialog", "+"))
        self.pushButton_x_up_angle.setText(_translate("Dialog", "+"))
        self.pushButton_y_down_angle.setText(_translate("Dialog", "-"))
        self.pushButton_y_up_angle.setText(_translate("Dialog", "+"))
        self.pushButton_x_down_angle.setText(_translate("Dialog", "-"))
        self.pushButton_z_up_angle.setText(_translate("Dialog", "+"))
        self.pushButton_z_down_angle.setText(_translate("Dialog", "-"))
        self.pushButton_set_angle.setText(_translate("Dialog", "SetAngle"))
        self.lineEdit_delta.setPlaceholderText(_translate("Dialog", "delta"))
        self.lineEdit_theta.setPlaceholderText(_translate("Dialog", "theta"))

