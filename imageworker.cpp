#include "imageworker.h"
#include "library.h"
#include <chrono>

ImageWorker::ImageWorker(ViewState *state, QObject *parent): state(state), QThread(parent),
    generator(std::chrono::system_clock::now().time_since_epoch().count()) {
}

void ImageWorker::timeout() {
    auto _l = state->getReadLock();
    int c = state->lib->count();
    std::uniform_int_distribution<int> distribution(0, state->lib->count() - 1);
    auto i = distribution(generator);
    auto image = (*state->lib)[i];
    if (!image.isNull()) {
        loadImage(image);
    }
    timer.singleShot(state->setting.interval, this, &ImageWorker::timeout);
}

void ImageWorker::run() {
    timeout();
    exec();
}
