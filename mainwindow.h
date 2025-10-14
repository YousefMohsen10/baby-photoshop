#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Image_Class.h"
#include <QImage>
#include <QPixmap>
#include "ImageLabel.h"
#include <QPushButton>
using namespace std;
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onLoadImageClicked();
    void onApplyGrayScaleClicked();
    void onApplyBlackWhiteClicked();

    void onApplyInvertClicked();

    void onApplyOldTVClicked();

    void onSaveButtonClicked();

    void onApplyOilPaintClicked();

    void onApplyEdgeDetectionClicked();

    void onClearButtonClicked();

    void onDeleteButtonClicked();

    void onUndoButtonClicked();

    void onRedoButtonClicked();

    void onApplyInfraredClicked();

    void onApplyPurpleScaleClicked();

    void onApplySunlightClicked();

    void onApplyLeftRotateClicked();
    void onApplyRightRotateClicked();

    void onApplyFlipHorizontalClicked();
    void onApplyFlipVerticalClicked();

    void onflipButtonClicked();

    // void onApplyCropModeClicked();
    // void onCropFinished(QRect cropRect);
    void onCropRequested(const QRect &rect);

    void applyBrightnessSliderValue(int value);

    void onApplyResizeClicked(int width, int height);

    void updateSizeLabel(int width, int height);

    void applyBlurSliderValue(int value);

    void applySkewValue(int value);

    void hideAllFilterMenus(bool visible);

    void onMergeTypeSelected(bool resize, bool overlap);

    void onApplyNeonClicked();

    void onApplyWaveClicked();

    void onApplyFrameClicked();

    void onApplyShakeClicked();

    void updateImageLabel(const QImage &qimg);

    void disableAll();
    void enableAll();
    void resetAll();

    void onWidthChanged(int newWidth);
    void onHeightChanged(int newHeight);

    bool eventFilter(QObject *obj, QEvent *event);



private:
    Ui::MainWindow *ui;
    Image myImage;
    Image originalImage;
    Image mergeImage;
    vector<Image> history;
    int current = -1;
    int labelWidth, labelHeight;
    float originalAspectRatio = 1.0f;
    bool isUpdatingDimensions = false;

    QShortcut *saveShortcut = nullptr;
    QShortcut *deleteShortcut = nullptr;
    QShortcut *undoShortcut = nullptr;
    QShortcut *redoShortcut = nullptr;

    QImage convertImageToQImage(const Image &img);
    Image convertQImageToImage(const QImage &qimg);

    bool flipOptionsVisible = false;
    void toggleFlipOptions(bool visible);

    bool cropMode = false;
    QPoint cropStart, cropEnd;
};
#endif
