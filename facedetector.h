#ifndef FACEDETECTOR
#define FACEDETECTOR

//open cv
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/objdetect/objdetect.hpp>

#include <QObject>
#include <QElapsedTimer>
#include <QImage>

class FaceDetector: public QObject
{
Q_OBJECT

public:
    FaceDetector(QObject *parent = 0);
    void detect();
    void loadCascadeClassifier(QString path);
protected:

signals:
    void imageProcessed(QImage image);
    void statusChanged(QString text);
    void finished();

public slots:
    void processFrame();
    void exitThread();


private:
    int frames;

    cv::VideoCapture webcam;
    cv::Mat inputFrame;
    cv::Mat processedFrame;
    cv::Mat outputFrame;
    cv::CascadeClassifier faceDetector;

    QImage outputImage;
    QElapsedTimer *processTime;


};

#endif // FACEDETECTOR

