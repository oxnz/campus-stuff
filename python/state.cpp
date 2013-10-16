#include <iostream>
#include <queue>

class DoorFSM{
	public:
		enum States{Closed, Locked, Unlocked, Opened};
		enum Event{Lock, Unlock, Open, Close};
	private:
		States __Y;
		std::queue<Event> __events;
		void __processEvent(Event e);

	protected:
		virtual void enterOpened() = 0;
		virtual void enterClosed() = 0;
		virtual void enterLocked() = 0;
		virtual void enterUnlocked() = 0;
		
	public:

		DoorFSM() {
			__Y = Opened;
		}

		virtual ~DoorFSM() {}

		States currentState() {
			return __Y;
		}

		void A(Event e);
};

void DoorFSM::__processEvent(Event e)
{
	States yOld = __Y;
	bool pass = false;
	switch (__Y) {
		case Closed:
			if (e == Open) {
				__Y = Opened;
				pass = true;
			}
			else if (e == Lock) {
				__Y = Locked;
				pass = true;
			}
			break;
		case Unlocked:
			if (e == Lock) {
				__Y = Locked;
				pass = true;
			}
			else if (e == Open) {
				__Y = Opened;
				pass = true;
			}
			break;
		case Locked:
			if (e == Unlock) {
				__Y = Unlocked;
				pass = true;
			}
			break;
		case Opened:
			if (e == Close) {
				__Y = Closed;
				pass = true;
			}
			break;
	}

	if (yOld == __Y && !pass)
	{
		return;
	}

	switch (__Y) {
		case Closed:
			enterClosed();
			break;
		case Unlocked:
			enterUnlocked();
			break;
		case Locked:
			enterLocked();
			break;
		case Opened:
			enterOpened();
			break;
	}
}

void DoorFSM::A(Event e) {
	bool __empty = __events.empty();
	__events.push(e);
//	if (__empty) {
		while (!__events.empty()) {
			__processEvent(__events.front());
			__events.pop();
		}
//	}
}

class DoorFSMLogic:public DoorFSM
{
	protected:
		virtual void enterOpened() {
			std::cout << "Enter Opened state." << std::endl;
		}
		virtual void enterLocked() {
			std::cout << "Enter Closed state." << std::endl;
		}
		virtual void enterUnlocked() {
			std::cout << "Enter Locked state." << std::endl;
		}
		virtual void enterClosed() {
			std::cout << "Enter Unlocked state." << std::endl;
		}
};

int main() {
	DoorFSMLogic door;
	door.A(DoorFSM::Close);
	door.A(DoorFSM::Lock);
	door.A(DoorFSM::Unlock);
	door.A(DoorFSM::Open);
	door.A(DoorFSM::Close);
	door.A(DoorFSM::Open);
}
