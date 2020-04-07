#pragma once

#include <QMainWindow>
#include <QSize>
#include <QImage>
#include <memory>
#include "setting.h"
#include "viewstate.h"

QT_BEGIN_NAMESPACE
class QResizeEvent;
QT_END_NAMESPACE
class ImageWorker;
class ImageView;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
	MainWindow();
	virtual ~MainWindow();
    void resizeEvent(QResizeEvent* event) override;

private slots:
    void loadImage(QImage);
    void contextMenu();

private:
    ImageView* view;
    ImageWorker* worker;
    ViewState state;
    void resetLib();
};
