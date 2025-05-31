#ifndef Spinlock_h
#define Spinlock_h

#include <atomic>

namespace chloride {

	class Spinlock {
	private:
		std::atomic_flag& flag;
		bool locked = false;

	public:

		~Spinlock() {
			if (locked) flag.clear();
		}

		Spinlock(std::atomic_flag& flag, bool deferLock = false) 
		: flag(flag) {
			if (!deferLock) lock();
		}

		bool tryLock() {
			return flag.test_and_set();
		}

		void lock() {
			while (!flag.test_and_set());
			locked = true;
		}

	};

};

#endif
