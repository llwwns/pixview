#pragma once
#include <QGraphicsView>
#include <QWidget>
#include <QMouseEvent>

class MainView : public QGraphicsView {
    Q_OBJECT
public:
    MainView(QWidget *parent): QGraphicsView(parent){}
    void mouseReleaseEvent(QMouseEvent *e) override {
        if (e->button() == Qt::RightButton) {
            rightClicked();
        }
    }
signals:
    void rightClicked();
};

