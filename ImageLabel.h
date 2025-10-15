#ifndef IMAGELABEL_H
#define IMAGELABEL_H

#include <QLabel>
#include <QMouseEvent>
#include <QPainter>
#include <QRect>

class ImageLabel : public QLabel {
    Q_OBJECT

public:
    explicit ImageLabel(QWidget *parent = nullptr)
        : QLabel(parent), cropping(false) {}

    void setCropping(bool value) {
        cropping = value;
        readyToDraw = false; // wait for the first click
        setCursor(value ? Qt::CrossCursor : Qt::ArrowCursor);
    }
    bool getCroppingStatus(){return cropping;}

protected:
    void mousePressEvent(QMouseEvent *event) override {
        if (!cropping) return;
        if (!readyToDraw) {
            startPoint = event->pos();
            endPoint = startPoint;
            readyToDraw = true;
        } else {
            // second click finalizes
            endPoint = event->pos();
            cropping = false;
            readyToDraw = false;
            update();

            QRect rect = getSelectionRect();
            if (rect.isValid())
                emit cropRequested(rect);

            setCursor(Qt::ArrowCursor);
        }
        update();
    }

    void mouseMoveEvent(QMouseEvent *event) override {
        if (cropping && readyToDraw) {
            endPoint = event->pos();
            update();
        }
    }

    void mouseReleaseEvent(QMouseEvent *event) override {
        if (!cropping) return;
        endPoint = event->pos();
        cropping = false;
        update();

        QRect rect = getSelectionRect();
        if (rect.isValid()) {
            emit cropRequested(rect);
        }
    }

    void paintEvent(QPaintEvent *event) override {
        QLabel::paintEvent(event);
        if (cropping && readyToDraw) {
            QPainter painter(this);
            painter.setPen(QPen(Qt::white, 2, Qt::DashLine));
            painter.drawRect(getSelectionRect());
        }
    }

private:
    QPoint startPoint;
    QPoint endPoint;
    bool cropping;
    bool readyToDraw;

    QRect getSelectionRect() const {
        return QRect(startPoint, endPoint).normalized();
    }

signals:
    void cropRequested(const QRect &rect);
};

#endif // IMAGELABEL_H
