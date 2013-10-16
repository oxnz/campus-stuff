State(object):
	name
	do_action
	check_condition
	entry_action
	exit_action
StateMachine(object):
	states
	active_state
Entity(object):
	ID
	name
	description
	state
world:Entity():
	self.state = State()
