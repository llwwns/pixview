#pragma once
#include <QReadWriteLock>
#include <memory>
#include "library.h"
#include "setting.h"

class Lock {
    QReadWriteLock* lock;
public:
    Lock(QReadWriteLock* l): lock(l) { }
    ~Lock() {lock->unlock();}
};

struct ViewState {
    Setting setting;
    std::unique_ptr<Library> lib;
    QReadWriteLock mutex;
    Lock getReadLock() {
        mutex.lockForRead();
        return Lock(&mutex);
    }
    Lock getWriteLock() {
        mutex.lockForRead();
        return Lock(&mutex);
    }
};
