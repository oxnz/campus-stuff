#!/usr/bin/env python
#-*- encoding: utf-8 -*-
#
# Copyright (c) 1998, Yun Xinyi <yunxinyi@gmail.com>
# All rights reserved.
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
#     * Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above copyright
#       notice, this list of conditions and the following disclaimer in the
#       documentation and/or other materials provided with the distribution.
#     * Neither the name of the Yun Xinyi, <yunxinyi@gmail.com> nor the
#       names of its contributors may be used to endorse or promote products
#       derived from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY YUN XINYI AND CONTRIBUTORS "AS IS" AND ANY
# EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL YUN XINYI AND CONTRIBUTORS BE LIABLE FOR ANY
# DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


from PyQt4.QtCore import *
from PyQt4.QtGui import *
import sys
import platform

class Task(object):
	sections = ['进程号', '进程名', '运行时间', '优先级']
	sectionTips = ['全局唯一进程表示号', '进程名称', '剩余的运行时间', '当前的优先级, 可以选择单元格进行更改']
	priorMax = 100000
	priorMin = 1
	def __init__(self, uuid, name, life, prior):
		self._uuid = uuid
		self._name = name
		self._life = life
		self._prior = prior
		self._progress = 0
	@property
	def uuid(self):
		return self._uuid
	@uuid.setter
	def uuid(self, uuid):
		if not isinstance(uuid, int):
			raise TypeError('uuid must be integer')
		self._uuid = uuid
	@property
	def name(self):
		return self._name
	@name.setter
	def name(self, name):
		if isinstance(name, str) or isinstance(name, QString):
			self._name = name
		else:
			raise TypeError('name must be string or QString, but %s is supplied' % type(name))
	@property
	def life(self):
		return self._life
	@life.setter
	def life(self, life):
		if not isinstance(life, int):
			raise TypeError('life must be integer')
		self._life = life
	@property
	def prior(self):
		return self._prior
	@prior.setter
	def prior(self, prior):
		if not isinstance(prior, int):
			raise TypeError('prior must be integer, but %s is supplied' % type(prior))
		elif prior < 0:
			raise ValueError('prior must not be negtive')
		else:
			self._prior = prior
	@property
	def progress(self):
		return self._progress
	@progress.setter
	def progress(self, progress):
		if not isinstance(progress, int):
			raise TypeError('progress must be integer, but %s is supplied' % type(prior))
		elif progress < 0:
			print 'progress=', progress
			raise ValueError('progress must not be negtive')
		else:
			self._progress = progress
	
	def run(self):
		self._progress += 10
		if self._progress > 1000:
			self._progress = 0
			self._life -= 1
			self._prior -= 1

class TaskItemDelegate(QItemDelegate):
	def __init__(self, parent=None):
		super(TaskItemDelegate, self).__init__(parent)
	def createEditor(self, parent, option, index):
		row = index.row()
		col = index.column()
		if col == 3:
			editor = QSpinBox(parent)
			editor.setFrame(False)
			return editor
	def setEditorData(self, editor, index):
		row = index.row()
		col = index.column()
		if col == 3:
			lifeIndex = index.model().index(row, col-1, QModelIndex())
			editor.setMinimum(lifeIndex.model().data(lifeIndex, Qt.EditRole))
			editor.setMaximum(Task.priorMax)
			editor.setValue(index.model().data(index, Qt.EditRole))
	
	def setModelData(self, editor, model, index):
		editor.interpretText()
		model.setData(index, editor.value(), Qt.EditRole)
	def updateEditorGeometry(self, editor, option, index):
		editor.setGeometry(option.rect)

class TaskPool(QAbstractTableModel):
	def __init__(self, parent=None):
		super(TaskPool, self).__init__(parent)
		self.__uuid = 0
		self.__tasks = []
	@property
	def uuid(self):
		self.__uuid += 1
		return self.__uuid
	@uuid.setter
	def uuid(self, uuid):
		if not isinstance(uuid, int):
			raise TypeError('uuid must be integer')
		self.___uuid = uuid
	@property
	def currentTask(self):
		try:
			return self.__tasks[0]
		except IndexError:
			print 'No more task exists'
			return None
		except Exception, e:
			print e
			raise RuntimeError('unexpected error')
	def isEmpty(self):
		return len(self.__tasks) == 0
	def addTask(self, task):
		if not isinstance(task, Task):
			raise TypeError('task is not a instance of Task')
		self.__tasks.append(task)
		self.reset()
	def delTask(self, task):
		self.__tasks.remove(task)
		self.reset()
	def reset(self):
		QAbstractTableModel.reset(self)
	def sort(self, column = 3, order = Qt.DescendingOrder):
		r = True
		k = lambda x: x.prior
		if order == Qt.AscendingOrder:
			r = False
		else:
			r = True
		if column == 0:
			k = lambda x:x.uuid
		elif column == 1:
			k = lambda x:x.name
		elif column == 2:
			k = lambda x:x.life
		elif column == 3:
			k = lambda x:x.prior
		self.__tasks.sort(key=k, reverse=r)
		self.reset()
	def columnCount(self, index):
		return 4
	def rowCount(self, index):
		return len(self.__tasks)
	def headerData(self, section, orientation, role):
		results = {Qt.Vertical :{
				Qt.DisplayRole: lambda: QVariant(),
				Qt.DecorationRole: lambda: QVariant(),
				Qt.ToolTipRole: lambda: ['已就绪', '正在运行'][bool(section)],
				Qt.TextColorRole: lambda: QColor(Qt.red),
				Qt.BackgroundColorRole: lambda: QColor(Qt.blue),
			},
			Qt.Horizontal:{
				Qt.DisplayRole: lambda: Task.sections[section],
				Qt.DecorationRole: lambda: QVariant(),
				Qt.ToolTipRole: Task.sectionTips[section],
				Qt.TextColorRole: lambda: QColor(Qt.blue),
				Qt.BackgroundColorRole: lambda: QColor(Qt.green),
			}
		}
		try:
			return results[orientation][role]()
		except:
			return QAbstractTableModel.headerData(self, section, orientation, role)

	def flags(self, index):
		flags = QAbstractTableModel.flags(self, index)
		if index.column() == 3:
			flags |= Qt.ItemIsEditable
		return flags
	def data(self, index, role):
		if not index.isValid():
			return QVariant()
		row = index.row()
		col = index.column()
		values = {
			0: self.__tasks[row].uuid,
			1: self.__tasks[row].name,
			2: self.__tasks[row].life,
			3: self.__tasks[row].prior,
		}
		results = {
				Qt.DisplayRole: lambda: values[col],
				Qt.EditRole: lambda: values[col],
				Qt.TextAlignmentRole: lambda: Qt.AlignRight | Qt.AlighCenter,
				Qt.TextColorRole: lambda: QVariant(),
				Qt.BackgroundColorRole: lambda: QVariant(),
				Qt.ToolTipRole: lambda: QVariant(),
				Qt.StatusTipRole: lambda: QVariant(),
				Qt.FontRole: lambda: QVariant(),
				Qt.WhatsThisRole: lambda: QVariant(),
		}
		try:
			return results[role]()
		except:
			return QVariant()
	def setData(self, index, value, role):
		if not index.isValid():
			return False
		if value == QVariant(''):
			return False
		row = index.row()
		col = index.column()
		if col == 3:
			# set new prior
			if not value < self.__tasks[row].life:
				self.__tasks[row].prior = value
				self.sort()
				self.reset()
				return True
			else:
				raise TypeError('Prior must be integer or int-convertable and should be greater than task life')
				return False
		else:
			raise  IndexError('Task dose not have an index of (%d,%d)' % (row, column))

class TaskManager(QMainWindow):
	def __init__(self, parent=None):
		super(TaskManager, self).__init__(parent)
		self.status = False

		self.timer = QTimer()
		self.timer.setInterval(1000)
		self.timer.timeout.connect(self.schedule)

		self.taskTimer = QTimer()
		self.taskTimer.setInterval(10)
		self.taskTimer.timeout.connect(self.execTask)

		self.taskTable = QTableView()
		self.taskTable.sortByColumn(3, Qt.DescendingOrder)
		self.taskTable.setSortingEnabled(True)
		self.taskTable.setAlternatingRowColors(True)
		self.taskTable.verticalHeader().setVisible(False)
		self.taskTable.setEditTriggers(QAbstractItemView.DoubleClicked | QAbstractItemView.SelectedClicked)
#		self.taskTable.setSelectionBehavior(QTableView.SelectRows)
		self.taskTable.setSelectionMode(QTableView.SingleSelection)
		self.taskTable.horizontalHeader().setResizeMode(QHeaderView.Stretch)
		self.model = TaskPool(self)
		self.taskTable.setModel(self.model)
		self.taskTable.setItemDelegate(TaskItemDelegate(self))

		self.setWindowTitle("任务管理器")
		layout = QHBoxLayout()
		widget = QWidget(self)
		widget.setLayout(layout)
		self.setCentralWidget(widget)
		
		grid = QGridLayout()
		label = QLabel("当前任务:")
		grid.addWidget(label, 0, 0)
		self.taskName = QLabel()
		grid.addWidget(self.taskName, 0, 1)
		label = QLabel("运行进度:")
		grid.addWidget(label, 1, 0)
		self.taskProgBar = QProgressBar()
		self.taskProgBar.setMinimum(0)
		self.taskProgBar.setMaximum(1000)
		grid.addWidget(self.taskProgBar, 1, 1)
		label = QLabel("名称:")
		grid.addWidget(label, 2, 0)
		self.taskNameEdit  = QLineEdit("test")
		self.taskNameEdit.setValidator(QRegExpValidator(QRegExp('^[A-Za-z_]{1}[0-9A-Za-z_-]{0,9}$')))
		grid.addWidget(self.taskNameEdit, 2, 1)
		label = QLabel("运行时间:")
		grid.addWidget(label, 3, 0)
		self.taskLife = QSpinBox()
		self.taskLife.setMinimum(1)
		grid.addWidget(self.taskLife, 3, 1)
		label = QLabel("优先级:")
		grid.addWidget(label, 4, 0)
		self.taskPrior = QSpinBox()
		self.taskPrior.setMinimum(Task.priorMin)
		self.taskPrior.setMaximum(Task.priorMax)
		self.taskLife.valueChanged.connect(lambda: self.taskPrior.setValue(self.taskLife.value()))
		grid.addWidget(self.taskPrior, 4, 1)
		vbox = QVBoxLayout()
		vbox.addLayout(grid)
		label = QLabel('')
		import os
		label.setPixmap(QPixmap(os.path.join(os.path.abspath(os.path.dirname(__file__)), './xx.png')))
#		label.setPixmap(QPixmap('./xx.png'))
		vbox.addWidget(label)
		
		vbox.addStretch()
		grid = QGridLayout()
		button = QPushButton("提交")
		button.clicked.connect(self.addTask)
		grid.addWidget(button, 0, 0)
		self.controlButton = QPushButton("开始")
		self.controlButton.clicked.connect(self.control)
		grid.addWidget(self.controlButton, 0, 1)
		button = QPushButton("帮助")
		button.clicked.connect(self.info)
		grid.addWidget(button, 1, 0)
		button = QPushButton("退出")
		button.clicked.connect(lambda: sys.exit(0))
		grid.addWidget(button, 1, 1)
		vbox.addLayout(grid)

		layout.setAlignment(Qt.AlignHCenter)
		layout.setAlignment(Qt.AlignVCenter)

		groupBox = QGroupBox("任务列表")
		groupBoxLayout = QHBoxLayout()
		groupBoxLayout.addWidget(self.taskTable)
		groupBox.setLayout(groupBoxLayout)
		layout.addWidget(groupBox)
#		layout.addWidget(self.taskTable)
		layout.setStretch(0, 3)
		layout.setStretch(1, 1)
		groupBox = QGroupBox("任务控制")
		groupBox.setLayout(vbox)
		layout.addWidget(groupBox)
#		layout.addLayout(vbox)
		self.resize(850, 500)

		menuBar = self.menuBar()
		fileMenuBar = menuBar.addMenu("文件")
		quitAction = QAction('退出', self)
		quitAction.triggered.connect(lambda: sys.exit(0))
		fileMenuBar.addAction(quitAction)
		helpMenuBar = menuBar.addMenu("帮助")
		helpAction = QAction('任务管理器帮助', self)
		helpAction.triggered.connect(self.info)
		helpMenuBar.addAction(helpAction)
		aboutAction = QAction('关于任务管理器', self)
		aboutAction.triggered.connect(self.about)
		helpMenuBar.addAction(aboutAction)

		self.show()

	def schedule(self):
		if not self.model.isEmpty():
			self.model.sort()
			if self.status:
				self.taskTimer.start()
	def control(self):
		if not self.status:
			self.timer.start()
			self.status = True
			self.controlButton.setText("暂停")
			if not self.model.isEmpty():
				self.taskTimer.start()
		else:
			self.timer.stop()
			self.taskTimer.stop()
			self.status = False
			self.controlButton.setText("继续")
		self.schedule()

	def addTask(self):
		name = self.taskNameEdit.text()
		if name.isEmpty():
			QMessageBox.warning(self, "警告", "任务名不能为空!")
			return
		life = self.taskLife.value()
		prior = self.taskPrior.value()
		self.model.addTask(Task(self.model.uuid, name, life, prior))
		self.schedule()
	
	def execTask(self):
		task = self.model.currentTask
		task.run()
		if task.life > 0:
			self.taskName.setText(task.name)
			if task.progress == 0:
				self.model.reset()
			self.taskProgBar.setValue(task.progress)
		elif task.life == 0:
			self.taskProgBar.setValue(0)
			self.taskTimer.stop()
			self.dropTask(task)
			self.schedule()
	
	def dropTask(self, task):
		self.model.delTask(task)

	def about(self):
		QMessageBox.about(self, "关于", '''
<html>
	<h3>关于</h3>
	<table width=300>
		<caption>个人信息:</caption>
		<hr />
		<tr><td>姓名:</td><td></td>张盼</tr>
		<tr><td>学号:</td><td>2011302530064</td></tr>
		<tr><td>班级:</td><td>信息安全&lt;3&gt;班</td></tr>
	</table>
</html>
			''')

	def info(self):
		help = QDialog(self)
		vbox = QVBoxLayout()
		help.setLayout(vbox)
		help.setWindowTitle("帮助")
		helpContent = QLabel('''
<html>
	<style type="text/css">
		li {
			font-size: 14pt;
		}
		p {
			font-size: 11pt;
		}
	</style>
	<h3>任务管理器--优先级调度</h3>
	<ol>
		<li>添加任务</li>
			<p>任务名不能为空，名称必须以字幕或者下划线开头，长度为10个字符，优先级和运行时间大于0，且优先级大于运行时间(否则可能会没有足够的优先级来结束任务)</p>
		<li>动态调整</li>
			<p>在运行过程中可以动态改变优先级，此操作在表中点击需要更改的优先级单元格，然后输入新的优先级</p>
		<li>开始/暂停/继续</li>
			<p>点击开始，则开始以一秒为周期进行任务调度，每个任务时间片为1秒，暂停会停止一切任务和调度，继续则恢复运行和调度</p>
		<li>帮助</li>
			<p>显示此消息</p>
		<li>退出</li>
			<p>退出程序</p>
	</ol>
</html>
			''')
		vbox.addWidget(helpContent)
		buttonBox = QDialogButtonBox()
		buttonBox.addButton("知道了", QDialogButtonBox.AcceptRole)
		buttonBox.accepted.connect(help.destroy)
		vbox.addWidget(buttonBox)
		help.open()
#		help.exec_()


if __name__ == '__main__':
	if not platform.system() == 'Darwin':
		codec = QTextCodec.codecForName('utf-8')
		QTextCodec.setCodecForCStrings(codec)
		QTextCodec.setCodecForTr(codec)

	app = QApplication(sys.argv)
	taskManager = TaskManager()
	sys.exit(app.exec_())
