#include "mainwindow.h"
#include "imageview.h"
#include "library.h"
#include "imageworker.h"

#include <memory>
#include <QPalette>
#include <QImage>
#include <QResizeEvent>
#include <QStandardPaths>
#include <QMenu>
#include <QAction>

MainWindow::MainWindow()
{
    view = new ImageView(&state, this);
    setCentralWidget(view);
    state.setting = Setting::load();
    if (state.setting.fullscreen) {
       setWindowFlag(Qt::FramelessWindowHint, true);
       showMaximized();
    }
    resetLib();
    worker = new ImageWorker(&state, this);
    connect(worker, &ImageWorker::loadImage, this, &MainWindow::loadImage);
    worker->start();
    connect(static_cast<ImageView*>(view), &ImageView::rightClicked, this, &MainWindow::contextMenu);
}

void MainWindow::resetLib() {
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

MainWindow::~MainWindow()
{
    worker->quit();
    worker->wait();
}

void MainWindow::resizeEvent(QResizeEvent* event) {
    view->resized();
}

void MainWindow::loadImage(QImage image) {
    view->setImage(image);
}

void MainWindow::contextMenu() {
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
