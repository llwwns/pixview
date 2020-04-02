#pragma once
#include <QMutex>
#include <memory>
#include "library.h"
#include "setting.h"

struct ViewState {
    Setting setting;
    std::unique_ptr<Library> lib;
    QMutex mutex;
};
