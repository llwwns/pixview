#include <QThread>
#include "library.h"
#include <atomic>

QT_BEGIN_NAMESPACE
class QImage;
QT_END_NAMESPACE

enum class WorkerState {
    waiting,
    loading,
    paused
};


class ImageWorker : public QThread {
Q_OBJECT
public:
    ImageWorker(std::unique_ptr<Library> lib, QObject *parent = nullptr);
    void pause();
    void isPaused();
    void resume();

signals:
    void loadImage(QImage);

private:
    void run() override;
    std::unique_ptr<Library> lib;
    const static int INTERVAL = 3000;
    std::atomic<WorkerState> state;
};
