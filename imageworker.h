#include <QThread>
#include "library.h"
#include <atomic>
#include <QTimer>
#include <QMutex>
#include <random>
#include "viewstate.h"

QT_BEGIN_NAMESPACE
class QImage;
QT_END_NAMESPACE

class ImageWorker : public QThread {
Q_OBJECT
public:
    ImageWorker(ViewState *state, QObject *parent = nullptr);
    void pause();
    void isPaused();
    void resume();

signals:
    void loadImage(QImage);

private slots:
    void timeout();

private:
    QTimer timer;
    void run() override;
    const static int INTERVAL = 3000;
    std::mt19937 generator;
    QMutex mutex;
    ViewState* state;
};
