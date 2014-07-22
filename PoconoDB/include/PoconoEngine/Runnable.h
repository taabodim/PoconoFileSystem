/*
 * Runnable.h
 *
 *  Created on: Mar 10, 2014
 *      Author: mahmoudtaabodi
 */

#include <list>
#include <boost/thread.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>

#ifndef RUNNABLE_H_
#define RUNNABLE_H_

using namespace boost;

namespace PoconoDB {


//template<typename runnableType>
//class Runnable {
//private:
//	int taskId;
//	bool taskDone;
//	runnableType* runnable;
//public:
//	Runnable(int id) {
//		taskId = id;
//	}
//	Runnable& Runnbable(Runnable& r) {
//		return r;
//	}
//	Runnable& operator=(Runnable& r) {
//		return r;
//	}
//	bool operator==(Runnable& r) {
//		return taskId == r.taskId;
//	}
//	int getTaskId() {
//		return taskId;
//	}
//	bool isTaskDone() {
//		return taskDone;
//	}
//	void setTaskDone(bool done) {
//		taskDone = done;
//	}
//
//	void run() {
//		runnable->run();
//		setTaskDone(true);
//	}
//	virtual ~Runnable() {
//		mylogger << "Runnable object deleted by destructor." //<< std::endl;
//	}
//};

   
    class Runnable {
    private:
        int taskId;
        bool taskDone;
       
    public:
        Runnable() {
            taskId = -2;
            taskDone = false;
        }
        Runnable(int id) {
            taskId = id;
        }
        Runnable& Runnbable(Runnable& r) {
            return r;
        }
        Runnable& operator=(Runnable& r) {
            return r;
        }
        bool operator==(Runnable& r) {
            return taskId == r.taskId;
        }
        int getTaskId() {
            return taskId;
        }
        bool isTaskDone() {
            return taskDone;
        }
        void setTaskDone(bool done) {
            taskDone = done;
        }
        virtual void run() =0;
        virtual ~Runnable() {
        }
    };
    typedef std::shared_ptr<Runnable> taskType;
}

#endif /* RUNNABLE_H_ */
