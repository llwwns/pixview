#include "imageworker.h"
#include "library.h"
#include <random>
#include <chrono>

ImageWorker::ImageWorker(std::unique_ptr<Library> lib, QObject *parent): lib(std::move(lib)), QThread(parent) {
}

void ImageWorker::run() {
    if (lib == nullptr) {
        return;
    }
    std::random_device device;
    std::mt19937 generator(device());
    std::uniform_int_distribution<int> distribution(0, lib->count() - 1);
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::knuth_b g(seed);
    for (;;) {
        int c = lib->count();
        if (c != 0) {
            auto i = distribution(generator);
            auto image = (*lib)[i];
            if (image.isNull()) {
                continue;
            }
            loadImage(image);
        }
        msleep(INTERVAL);
    }
}
