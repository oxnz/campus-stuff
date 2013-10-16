#!/usr/bin/env python
#-*- encoding: UTF-8 -*-

class StateMachine:
	def __init__(self):
		self.handlers = {}
		self.startState = None
		self.endStates = []
	
	def addState(self, name, handler, endState=0):
		name = name
		self.handlers[name] = handler
		if endState:
			self.endStates.append(name)

	def setStartState(self, name):
		self.startState = name

	def run(self):
		try:
			handler = self.handlers[self.startState]
		except:
			raise "initializationError", "must call .set_start() before .run()"
		if not self.endStates:
			raise "InitializationError", "at least one state must be endstate"
		while 1:
			newState = handler()
			if newState in self.endStates:
				break;
			else:
				handler = self.handlers[newState]

def born():
	print "Born ..."
	return "sing"

def sing():
	print "Sing ..."
	return "sleep"

def sleep():
	print "Sleep ..."
	return "die"

def die():
	print "Die ..."

if __name__ == "__main__":
	m = StateMachine()
	m.addState("born", born)
	m.addState("sing", sing)
	m.addState("sleep", sleep)
	m.addState("die", die, endState=1)
	m.setStartState("born")
	m.run()
