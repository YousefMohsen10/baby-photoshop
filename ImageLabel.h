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

    void setCropping(bool value) { cropping = value; }
    bool getCroppingStatus(){return cropping;}

protected:
    void mousePressEvent(QMouseEvent *event) override {
        if (!cropping) return;
        startPoint = event->pos();
        endPoint = startPoint;
        update();
    }

    void mouseMoveEvent(QMouseEvent *event) override {
        if (!cropping) return;
        endPoint = event->pos();
        update();
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
        if (cropping) {
            QPainter painter(this);

            painter.setPen(QPen(Qt::white, 2, Qt::DashLine));
            painter.drawRect(getSelectionRect());
        }
    }

private:
    QPoint startPoint;
    QPoint endPoint;
    bool cropping;

    QRect getSelectionRect() const {
        return QRect(startPoint, endPoint).normalized();
    }

signals:
    void cropRequested(const QRect &rect);
};

#endif // IMAGELABEL_H
