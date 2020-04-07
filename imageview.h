#pragma once
#include <QGraphicsView>
#include "viewstate.h"

QT_BEGIN_NAMESPACE
class QWidget;
class QMouseEvent;
class QGraphicsScene;
class QGraphicsPixmapItem;
QT_END_NAMESPACE

class ImageView : public QGraphicsView {
    Q_OBJECT
public:
    ImageView(ViewState* state, QWidget *parent);
    virtual ~ImageView();
    void mouseReleaseEvent(QMouseEvent *e);
    void setImage(QImage);
    void resized();

signals:
    void rightClicked();

private:
    const static int MAX_TEXTURE_SIZE = 4096 * 4096;
    ViewState *state;
    QGraphicsScene *scene;
    std::unique_ptr<QGraphicsPixmapItem> pixmapItem;
};

