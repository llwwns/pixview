#include "imageview.h"
#include <QWidget>
#include <QMouseEvent>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>

ImageView::ImageView(ViewState* state, QWidget *parent): QGraphicsView(parent),
    state(state), pixmapItem(std::make_unique<QGraphicsPixmapItem>())  {
	scene = new QGraphicsScene(this);
    scene->addItem(pixmapItem.get());
    this->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::LosslessImageRendering);
	QPalette p = this->viewport()->palette();
	p.setColor(QPalette::Window,QColor(0, 0, 0));
    this->viewport()->setPalette(p);
	this->viewport()->setBackgroundRole(QPalette::Window);
    this->setScene(scene);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    this->fitInView(pixmapItem.get(), Qt::KeepAspectRatio);
}

void ImageView::mouseReleaseEvent(QMouseEvent *e) {
    if (e->button() == Qt::RightButton) {
        rightClicked();
    }
}

void ImageView::resized() {
    this->fitInView(pixmapItem.get(), Qt::KeepAspectRatio);
}

void ImageView::setImage(QImage image) {
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
    auto _r = state->getReadLock();
    if (state->setting.autoRotate && ((this->size().width() > this->size().height()) != (image.width() > image.height()))) {
        if (!rotated) {
            this->rotate(90);
            rotated = true;
        }
    } else if (rotated) {
        this->rotate(-90);
        rotated = false;
    }
    scene->setSceneRect(0, 0, image.width(), image.height());
    resized();
}

ImageView::~ImageView() {
}
