#pragma once

#include <QMainWindow>
#include <QSize>
#include <QImage>
#include <memory>
#include "setting.h"
#include "viewstate.h"

QT_BEGIN_NAMESPACE
class QResizeEvent;
class QGraphicsView;
class QGraphicsPixmapItem;
class QGraphicsScene;
class QImage;
QT_END_NAMESPACE
class ImageWorker;

class ImageView : public QMainWindow
{
    Q_OBJECT

public:
	ImageView();
	virtual ~ImageView();
    void resizeEvent(QResizeEvent* event) override;

private slots:
    void loadImage(QImage);
    void contextMenu();

private:
    std::unique_ptr<QGraphicsPixmapItem> pixmapItem;
    QGraphicsScene *scene;
    QSize imageSize;
    QGraphicsView* view;
    ImageWorker* worker;
    const static int MAX_TEXTURE_SIZE = 4096 * 4096;
    ViewState state;
    void resetLib();
};
