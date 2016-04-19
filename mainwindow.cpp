#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtCore>
#include <QCloseEvent>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    currentStatus = new QLabel();
    ui->statusBar->addWidget(currentStatus);


    //menu
    fileMenu = ui->menuBar->addMenu("File");
    openFile = fileMenu->addAction("Open File");


    timer = new QTimer();
    detector = new FaceDetector();
    thread = new QThread();

    connect(timer,SIGNAL(timeout()),detector,SLOT(processFrame()));
    connect(detector,SIGNAL(imageProcessed(QImage)),SLOT(updateImage(QImage)));
    connect(detector,SIGNAL(statusChanged(QString)),SLOT(updateStatus(QString)));

    connect(openFile,SIGNAL(triggered(bool)),SLOT(openCascadeClassifier()));


    //correct thread stopping
    connect(this,SIGNAL(KillThread()),timer,SLOT(stop()));
    connect(this,SIGNAL(KillThread()),detector,SLOT(exitThread()));
    connect(detector, SIGNAL(finished()), thread, SLOT(quit()));
    connect(detector, SIGNAL(finished()), detector, SLOT(deleteLater()));
    connect(thread,SIGNAL(finished()),thread,SLOT(deleteLater()));
    connect(thread,SIGNAL(finished()),SLOT(close()));

    timer->start(50); //20 fps

    timer->moveToThread(thread);
    detector->moveToThread(thread);
    thread->start();
}
void MainWindow::openCascadeClassifier()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open File"),QDir::currentPath(),tr("XML files(*.xml)"));
    if (!fileName.isEmpty()) {
        detector->loadCascadeClassifier(fileName);
        }
}
void MainWindow::updateStatus(QString status)
{
    currentStatus->setText(status);
}
void MainWindow::updateImage(QImage image)
{
    ui->camView->setPixmap(QPixmap::fromImage(image));
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (!thread->isFinished()) {
         emit KillThread();
         event->ignore();
    }
}
MainWindow::~MainWindow()
{
    delete ui;
}
