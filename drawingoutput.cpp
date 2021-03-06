#include "drawingoutput.h"

#include <iostream>

#include <QDebug>
#include <QColor>

using namespace std;

DrawingOutput::DrawingOutput(QTextEdit *text_box, QObject *parent)
    : AbstractOutput(parent)
    , text_box_(text_box)
    , pen_(Qt::red, 2)
{
    pen_.setStyle(Qt::SolidLine);
}

DrawingOutput::DrawingOutput(QTextEdit *text_box, QPen pen, QBrush brush, QObject *parent)
    : AbstractOutput(parent)
    , text_box_(text_box)
    , pen_(pen)
    , brush_(brush)
{
}

void DrawingOutput::output(const std::vector<std::pair<int, float> > &predictions) {
    if (!is_labels_) {
        for (auto pred : predictions) {
            string s("class: ");
            s.append(to_string(pred.first).c_str());
            s.append(", probability: ");
            s.append(to_string(pred.second).c_str());
            text_box_->append(s.c_str());
        }
        text_box_->append("");
    } else {
        for (auto pred : predictions) {
            string s("label: ");
            s.append(labels_[pred.first]);
            s.append(", probability: ");
            s.append(to_string(pred.second).c_str());
            text_box_->append(s.c_str());
        }
        text_box_->append("");
    }
}

void DrawingOutput::output(const std::vector<cv::Rect> &boxes) {
    boxes_.clear();
    for (cv::Rect box : boxes) {
        boxes_.push_back(toQRectF(box));
    }

    update();
}

//TODO: draw predictions
void DrawingOutput::output(const std::vector<cv::Rect> &boxes,
                              const std::vector<std::vector<std::pair<int, float> > > &predictions) {

    Q_ASSERT(boxes.size() == predictions.size());

    boxes_.clear();
    for (cv::Rect box : boxes) {
        boxes_.push_back(toQRectF(box));
    }

    update();
}

void DrawingOutput::update() {
    if (overlay_size_!= cv::Size()) {
        Q_ASSERT(!overlay_.isNull());

        p_.fillRect(overlay_.rect(), Qt::transparent);
        p_.drawRects(boxes_);

        emit outputReady(overlay_);
    }
}

void DrawingOutput::updateOutputSize(cv::Size overlay_size) {
    if (overlay_size != overlay_size_) {
        resetOverlay(overlay_size);
    }
}

void DrawingOutput::resetOverlay(cv::Size overlay_size) {
    overlay_size_ = overlay_size;
    p_.end();
    overlay_ = QPixmap( toQSize(overlay_size_) );
    overlay_.fill(Qt::transparent);
    p_.begin(&overlay_);
    p_.setPen(pen_);
    p_.setBrush(brush_);
    p_.setCompositionMode(QPainter::CompositionMode_Source);

    Q_ASSERT(overlay_.paintingActive());
}
