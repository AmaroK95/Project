#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "facedetector.h"
#include <QMainWindow>
#include <QLabel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void closeEvent(QCloseEvent* event);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QLabel *currentStatus;
    QTimer *timer;
    FaceDetector *detector;
    QThread *thread;
    QMenu *fileMenu;
    QAction *openFile;

signals:
    void KillThread();

public slots:
    void updateImage(QImage image);
    void updateStatus(QString status);
    void openCascadeClassifier();
};

#endif // MAINWINDOW_H
