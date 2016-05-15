#pragma once

#include <vector>

#include <QObject>
#include <QMutex>
#include <QReadWriteLock>

#include <opencv2/opencv.hpp>

#include "abstractclassifierbuilder.h"
#include "abstractdetectorbuilder.h"
#include "abstractclassifier.h"
#include "abstractdetector.h"
#include "abstractoutput.h"

class Processor : public QObject
{
    Q_OBJECT
public:
    Processor(AbstractOutput *output, QObject *parent = 0);
    ~Processor();

public slots:
    void receiveFrame(const cv::Mat &frame);

    void changeStateProcessing();
    void setProcessing(bool is_processing);
    void setOneshot(bool is_oneshot);

    void initClassifier();
    void initDetector();

private:
    AbstractClassifierBuilder *builder_cl_;
    AbstractDetectorBuilder *builder_dt_;
    AbstractClassifier *cl_;
    AbstractDetector *dt_;
    AbstractOutput *out_;

    bool is_ready_;
    bool is_processing_;
    bool is_oneshot_;

    QReadWriteLock mutex_;

    /*dbg*/int i = 0;

    void process(const cv::Mat &img);
    const std::vector<cv::Mat> cropImages(const cv::Mat &img, const std::vector<cv::Rect> &boxes);

    /* TODO: this method is just for scratch */
    void loadLabels(const std::string &labels_file);
};
