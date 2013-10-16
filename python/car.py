#!/usr/bin/env python

import random
import time

class base_fsm(object):
	def enter_state(self, obj):
		raise NotImplementedError()

	def exec_state(self, obj):
		raise NotImplementedError()

	def exit_state(self, obj):
		raise NotImplementedError()

class Car(object):
	def attach_fsm(self, state, fsm):
		self.fsm = fsm
		self.curr_state = state
	def change_state(self, new_state, new_fsm):
		self.curr_state = new_state
		self.fsm.exit_state(self)
		self.fsm = new_fsm
		self.fsm.enter_state(self)
		self.fsm.exec_state(self)
	def keep_state(self):
		self.fsm.exec_state(self)
	def stop(self):
		print "Stop!!!"
	def go(self):
		print "Gooooooooo!!!!"

class stop_fsm(base_fsm):
	def enter_state(self, obj):
		print "Car%s enter stop state!"%(id(obj))

	def exec_state(self, obj):
		print "Car%s in top state!"%(id(obj))
		obj.stop()
	
	def exit_state(self, obj):
		print "Car%s exit stop state!"%(id(obj))

class run_fsm(base_fsm):
	def enter_state(self, obj):
		print "Car%s enter run state!"%(id(obj))
	
	def exec_state(self, obj):
		print "Car%s in run state!"%(id(obj))
		obj.go()
	
	def exit_state(self, obj):
		print "Car%s exit run state!"%(id(obj))

class broken_fsm(base_fsm):
	def enter_state(self, obj):
		print "Car%s enter broken state!"%(id(obj))
	
	def exec_state(self, obj):
		print "Car%s in broken state!"%(id(obj))

	def exit_state(self, obj):
		print "Car%s exit broken state!"%(id(obj))


class fsm_mgr(object):
	def __init__(self):
		self._fsms = {}
		self._fsms[0] = stop_fsm()
		self._fsms[1] = run_fsm()
		self._fsms[2] = broken_fsm()
	def get_fsm(self, state):
		return self._fsms[state]
	def frame(self, objs, state):
		for obj in objs:
			if state == obj.curr_state:
				obj.keep_state()
			else:
				obj.change_state(state, self._fsms[state])
	
class World(object):
	def __init__(self):
		self._cars = []
		self._fsm_mgr = fsm_mgr()
		self.__init_car()
	def __init_car(self):
		for i in xrange(1):
			tmp = Car()
			tmp.attach_fsm(0, self._fsm_mgr.get_fsm(0))
			self._cars.append(tmp)
	def _frame(self):
		self._fsm_mgr.frame(self._cars, state_factory())
	
	def run(self):
		while True:
			self._frame()
			time.sleep(0.5)
def state_factory():
	return random.randint(0, 2)

if __name__ == "__main__":
	world = World()
	world.run()
