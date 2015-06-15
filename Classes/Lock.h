/*
* Lock.h
*
*  Created on: 2015年5月25日
*      Author: kerlw
*/

#ifndef LOCK_H_
#define LOCK_H_

class Lock
{
public:

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	Lock() {
	}
	~Lock() {
	}
	bool lock() {
		EnterCriticalSection(&m_cs);
		return true;
	}
	void unlock() {
		LeaveCriticalSection(&m_cs);
	}
private:
	CRITICAL_SECTION m_cs;
#else
	Lock() {
		pthread_mutex_init(&_mutex, NULL);
	}
	~Lock() {
		pthread_mutex_destroy(&_mutex);
	}
	bool lock() {
		return (pthread_mutex_lock(&_mutex) == 0);
	}
	void unlock() {
		pthread_mutex_unlock(&_mutex);
	}

private:
	pthread_mutex_t _mutex;
#endif

};

class AutoLock
{
public:
	AutoLock(Lock& lock) :
		_lock(lock) {
		_lock.lock();
	}

	~AutoLock() {
		_lock.unlock();
	}

private:
	Lock& _lock;
};

#endif /* LOCK_H_ */
