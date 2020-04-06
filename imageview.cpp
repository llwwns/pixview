#include "imageview.h"
#include "mainview.h"
#include "library.h"
#include "imageworker.h"

#include <memory>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QPalette>
#include <QImage>
#include <QResizeEvent>
#include <QStandardPaths>
#include <QMenu>
#include <QAction>

ImageView::ImageView(): pixmapItem(std::make_unique<QGraphicsPixmapItem>())
{
	scene = new QGraphicsScene(this);
    scene->addItem(pixmapItem.get());
    view = new MainView(this);
    view->setScene(scene);
    view->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::LosslessImageRendering);
	QPalette p = view->viewport()->palette();
	p.setColor(QPalette::Window,QColor(0, 0, 0));
    view->viewport()->setPalette(p);
	view->viewport()->setBackgroundRole(QPalette::Window);
    view->setScene(scene);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    view->fitInView(pixmapItem.get(), Qt::KeepAspectRatio);
    setCentralWidget(view);
    state.setting = Setting::load();
    if (state.setting.fullscreen) {
       setWindowFlag(Qt::FramelessWindowHint, true);
       showMaximized();
    }
    resetLib();
    worker = new ImageWorker(&state, this);
    connect(worker, &ImageWorker::loadImage, this, &ImageView::loadImage);
    worker->start();
    connect(static_cast<MainView*>(view), &MainView::rightClicked, this, &ImageView::contextMenu);
}

void ImageView::resetLib() {
    auto _l = state.getWriteLock();
    std::unique_ptr<Library> lib;
    for (auto l: state.setting.libraries) {
        if (l.name == state.setting.defaultLibrary) {
            lib = std::unique_ptr<Library>(new LibraryList(l.paths));
        }
    }
    if (lib == nullptr) {
        lib = std::unique_ptr<Library>(new LibraryList(QStandardPaths::standardLocations(QStandardPaths::PicturesLocation)));
    }
    state.lib = std::move(lib);
}

ImageView::~ImageView()
{
    worker->quit();
    worker->wait();
}

void ImageView::resizeEvent(QResizeEvent* event) {
    view->fitInView(pixmapItem.get(), Qt::KeepAspectRatio);
}

void ImageView::loadImage(QImage image) {
    auto size = image.size();
    auto s = size.width() * size.height();
    if (s > MAX_TEXTURE_SIZE) {
        auto w = int64_t(size.width()) * MAX_TEXTURE_SIZE / s;
        auto h = int64_t(size.height()) * MAX_TEXTURE_SIZE / s;
        image = image.scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        auto size = image.size();
    }
    pixmapItem->setPixmap(QPixmap::fromImage(image));
    pixmapItem->resetTransform();
    static bool rotated = false;
    auto _r = state.getReadLock();
    if (state.setting.autoRotate && ((view->size().width() > view->size().height()) != (image.width() > image.height()))) {
        if (!rotated) {
            view->rotate(90);
            rotated = true;
        }
    } else if (rotated) {
        view->rotate(-90);
        rotated = false;
    }
    scene->setSceneRect(0, 0, image.width(), image.height());
    resizeEvent(nullptr);
}

void ImageView::contextMenu() {
    QMenu menu;
    for (auto lib: state.setting.libraries) {
        auto act = menu.addAction(QString::fromStdString(lib.name));
        if (lib.name != state.setting.defaultLibrary) {
            connect(act, &QAction::triggered, this, [=](){
                auto _l = state.getWriteLock();
                this->state.setting.defaultLibrary = lib.name;
                resetLib();
            });
        } else {
            act->setCheckable(true);
            act->setChecked(true);
        }
    }
    menu.addSeparator();
    if (state.setting.fullscreen) {
        auto act = menu.addAction("normal");
        connect(act, &QAction::triggered, this, [=]() {
            auto _l = state.getWriteLock();
            setWindowFlag(Qt::FramelessWindowHint, false);
            showNormal();
           state.setting.fullscreen = false;
        });
    } else {
        auto act = menu.addAction("fullscreen");
        connect(act, &QAction::triggered, this, [=]() {
            auto _l = state.getWriteLock();
           setWindowFlag(Qt::FramelessWindowHint, true);
           setWindowFlag(Qt::WindowSystemMenuHint, true);
           showMaximized();
           state.setting.fullscreen = true;
        });
    }
	menu.exec(QCursor::pos()+QPoint(1,1));
}
