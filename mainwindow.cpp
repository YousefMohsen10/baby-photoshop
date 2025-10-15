#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include "Image_Class.h"
#include "filters.h"
#include <string>
#include <QMessageBox>
#include <QStyle>
#include <QEvent>
#include <QCursor>
#include <QTimer>
#include <QColor>
#include <QColorDialog>
#include <QInputDialog>
#include <QShortcut>
#include <QKeySequence>
#include "ImageLabel.h"
using namespace std;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // --- Save ---
    saveShortcut = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_S), this);
    connect(saveShortcut, &QShortcut::activated, this, &MainWindow::onSaveButtonClicked);

    // --- Delete ---
    deleteShortcut = new QShortcut(QKeySequence(Qt::Key_Delete), this);
    connect(deleteShortcut, &QShortcut::activated, this, &MainWindow::onDeleteButtonClicked);

    // --- Redo ---
    redoShortcut = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Y), this);
    connect(redoShortcut, &QShortcut::activated, this, &MainWindow::onRedoButtonClicked);

    // --- Undo ---
    undoShortcut = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Z), this);
    connect(undoShortcut, &QShortcut::activated, this, &MainWindow::onUndoButtonClicked);

    ui->imageLabel->setText("<span style='font-size:20px; color:white; font-weight:bold;'>No image loaded</span>");
    ui->imageLabel->setAlignment(Qt::AlignCenter);
    ui->imageLabel->setStyleSheet("background-color: #2e2e2e; border-radius: 10px;");

    saveShortcut->setEnabled(false);
    deleteShortcut->setEnabled(false);
    undoShortcut->setEnabled(false);
    redoShortcut->setEnabled(false);

    ui->imageLabel->setMouseTracking(true);
    ui->imageLabel->setAttribute(Qt::WA_Hover);
    ui->imageLabel->installEventFilter(this);
    // ui->flipButton->setMouseTracking(true);
    // ui->flipButton->setAttribute(Qt::WA_Hover);
    // ui->flipButton->installEventFilter(this);
    ui->flipHButton->setVisible(false);
    ui->flipVButton->setVisible(false);
    ui->brightnessWidget->hide();
    ui->resizeWidget->hide();
    ui->blurWidget->hide();
    ui->skewWidget->hide();
    ui->mergeWidget->hide();
    ui->frameWidget->hide();
    disableAll();

    this->setStyleSheet(
        "QMainWindow {"
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:1,"
        "stop:0 #0A0A0A, stop:1 #007A7A);"
        "}"
        );

    connect(ui->loadButton, &QPushButton::clicked, this, &MainWindow::onLoadImageClicked);
    connect(ui->saveButton, &QPushButton::clicked, this, &MainWindow::onSaveButtonClicked);
    connect(ui->clearButton, &QPushButton::clicked, this, &MainWindow::onClearButtonClicked);
    connect(ui->deleteButton, &QPushButton::clicked, this, &MainWindow::onDeleteButtonClicked);
    connect(ui->undoButton, &QPushButton::clicked, this, &MainWindow::onUndoButtonClicked);
    connect(ui->redoButton, &QPushButton::clicked, this, &MainWindow::onRedoButtonClicked);
    connect(ui->grayButton, &QPushButton::clicked, this, &MainWindow::onApplyGrayScaleClicked);
    connect(ui->blackwhitebutton, &QPushButton::clicked, this, &MainWindow::onApplyBlackWhiteClicked);
    connect(ui->invertbutton, &QPushButton::clicked, this, &MainWindow::onApplyInvertClicked);
    connect(ui->oldtvbutton, &QPushButton::clicked, this, &MainWindow::onApplyOldTVClicked);
    connect(ui->oilpaintButton, &QPushButton::clicked, this, &MainWindow::onApplyOilPaintClicked);
    connect(ui->edgesButton, &QPushButton::clicked, this, &MainWindow::onApplyEdgeDetectionClicked);
    connect(ui->infraredButton, &QPushButton::clicked, this, &MainWindow::onApplyInfraredClicked);
    connect(ui->purpleButton, &QPushButton::clicked, this, &MainWindow::onApplyPurpleScaleClicked);
    connect(ui->sunlightButton, &QPushButton::clicked, this, &MainWindow::onApplySunlightClicked);
    connect(ui->leftButton, &QPushButton::clicked, this, &MainWindow::onApplyLeftRotateClicked);
    connect(ui->rightButton, &QPushButton::clicked, this, &MainWindow::onApplyRightRotateClicked);
    connect(ui->flipButton, &QPushButton::clicked, this, &MainWindow::onflipButtonClicked);
    connect(ui->flipHButton, &QPushButton::clicked, this, &MainWindow::onApplyFlipHorizontalClicked);
    connect(ui->flipVButton, &QPushButton::clicked, this, &MainWindow::onApplyFlipVerticalClicked);
    connect(ui->neonButton, &QPushButton::clicked, this, &MainWindow::onApplyNeonClicked);
    connect(ui->waveButton, &QPushButton::clicked, this, &MainWindow::onApplyWaveClicked);
    connect(ui->frameApply, &QPushButton::clicked, this,&MainWindow::onApplyFrameClicked);
    connect(ui->shakeButton, &QPushButton::clicked, this, &MainWindow::onApplyShakeClicked);
    connect(ui->cropButton, &QPushButton::clicked, this, [this]() {
        hideAllFilterMenus(false);
        ui->imageLabel->setCropping(true);
    });

    connect(ui->imageLabel, &ImageLabel::cropRequested, this, &MainWindow::onCropRequested);
    connect(ui->brightnessButton, &QPushButton::clicked, this, [=](){
        hideAllFilterMenus(false);
        ui->brightnessWidget->show();
    });
    connect(ui->brightnessSlider, &QSlider::valueChanged, this, [=](int value){
        ui->brightnessValueLabel->setText(QString::number(value));
    });
    connect(ui->brightnessApply, &QPushButton::clicked, this, [=](){
        int sliderValue = ui->brightnessSlider->value();
        applyBrightnessSliderValue(sliderValue);
        hideAllFilterMenus(false);
    });
    connect (ui->resizeButton, &QPushButton::clicked, this, [=](){
        hideAllFilterMenus(false);
        ui->keepAspectCheckBox->setChecked(true);
        ui->resizeWidget->show();
    });
    connect(ui->widthBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::onWidthChanged);
    connect(ui->heightBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::onHeightChanged);
    connect(ui->resizeApply, &QPushButton::clicked, this, [=](){
        int widthValue = ui->widthBox->value();
        int heightValue = ui->heightBox->value();
        onApplyResizeClicked(widthValue, heightValue);
        hideAllFilterMenus(false);
    });
    connect(ui->blurSlider, &QSlider::valueChanged, this, [=](int value){
        QString blurPercent = QString("%1%").arg(value);
        ui->blurValueLabel->setText(blurPercent);
    });
    connect(ui->blurApply, &QPushButton::clicked, this, [=](){
        int sliderValue = ui->blurSlider->value();
        applyBlurSliderValue(sliderValue);
        hideAllFilterMenus(false);
    });
    connect(ui->blurButton, &QPushButton::clicked, this, [=](){
        hideAllFilterMenus(false);
        ui->blurWidget->show();
    });
    connect(ui->skewButton, &QPushButton::clicked, this, [=](){
        hideAllFilterMenus(false);
        ui->skewWidget->show();
    });
    connect(ui->skewApply, &QPushButton::clicked, this, [=](){
        int value = ui->skewBox->value();
        applySkewValue(value);
        hideAllFilterMenus(false);
    });
    connect(ui->mergeApply, &QPushButton::clicked, this, [=](){
        bool resize = ui->resizeRadioButton->isChecked();
        bool overlap = ui->overlapRadioButton->isChecked();
        onMergeTypeSelected(resize, overlap);
        hideAllFilterMenus(false);
    });

    connect(ui->mergeButton, &QPushButton::clicked, this, [=](){
        hideAllFilterMenus(false);
        QString filePath = QFileDialog::getOpenFileName(this, "Open Image", "C:\\Users\\Alaa\\Desktop\\Babyphotoshop data\\CS213-2025-2026-Assignemnt1-SampleImages\\CS213-2025-2026-Assignemnt1-SampleImages\\CS112-Winter2023-2024-Assignemnt3-SampleImages", "Images (*.png *.jpg *.bmp *.tga)");
        if (filePath.isEmpty()) return;
        string path = filePath.toStdString();
        mergeImage.loadNewImage(path);
        ui->mergeWidget->show();
    });

    connect(ui->frameButton,&QPushButton::clicked, this, [=](){
        hideAllFilterMenus(false);
        ui->frameWidget->show();
        // ui->frameApply->setEnabled(false);
    });







}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateImageLabel(const QImage &qimg)
{
    int labelWidth = 699;
    int labelHeight = 446;

    QPixmap pixmap = QPixmap::fromImage(qimg);
    QPixmap scaledPixmap = pixmap.scaled(labelWidth, labelHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    ui->imageLabel->setPixmap(scaledPixmap);
    ui->imageLabel->setFixedSize(labelWidth, labelHeight);
    ui->imageLabel->setAlignment(Qt::AlignCenter);
}

void MainWindow::onLoadImageClicked() {
    QString filePath = QFileDialog::getOpenFileName(this, "Open Image", "C:\\Users\\Alaa\\Desktop\\Babyphotoshop data\\CS213-2025-2026-Assignemnt1-SampleImages\\CS213-2025-2026-Assignemnt1-SampleImages\\CS112-Winter2023-2024-Assignemnt3-SampleImages", "Images (*.png *.jpg *.bmp *.tga)");
    if (filePath.isEmpty()) return;
    enableAll();
    resetAll();
    string path = filePath.toStdString();
    myImage.loadNewImage(path);
    originalImage = myImage;
    history.clear();
    history.push_back(myImage);
    current = 0;
    hideAllFilterMenus(false);
    QImage qimg = convertImageToQImage(myImage);
    updateImageLabel(qimg);
    updateSizeLabel(myImage.width, myImage.height);
    originalAspectRatio = static_cast<float>(myImage.width) / myImage.height;


}



QImage MainWindow::convertImageToQImage(const Image &img) {
    QImage qimg(img.width, img.height, QImage::Format_RGB888);
    for (int y = 0; y < img.height; ++y) {
        for (int x = 0; x < img.width; ++x) {
            auto r = img(x, y, 0);
            auto g = img(x, y, 1);
            auto b = img(x, y, 2);
            qimg.setPixelColor(x, y, QColor(r, g, b));
        }
    }
    return qimg;
}

Image MainWindow::convertQImageToImage(const QImage &qimg)
{
    int width = qimg.width();
    int height = qimg.height();
    Image img(width, height);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            QColor color = qimg.pixelColor(x, y);
            img(x, y, 0) = color.red();
            img(x, y, 1) = color.green();
            img(x, y, 2) = color.blue();
        }
    }
    return img;
}

void MainWindow::onClearButtonClicked()
{
    myImage = originalImage;
    hideAllFilterMenus(false);
    history.clear();
    history.push_back(myImage);
    current = 0;
    updateSizeLabel(myImage.width, myImage.height);
    resetAll();
    QImage qimg = convertImageToQImage(myImage);
    updateImageLabel(qimg);
}


void MainWindow::onDeleteButtonClicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Delete Image", "Are you sure you want to delete the current image?", QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::No) return;
    myImage = Image();
    originalImage = Image();
    current = 0;
    history.clear();
    hideAllFilterMenus(false);
    ui->imageLabel->clear();
    ui->imageLabel->setText("<span style='font-size:20px; color:white; font-weight:bold;'>No image loaded</span>");
    ui->imageLabel->setAlignment(Qt::AlignCenter);
    ui->imageLabel->setStyleSheet("background-color: #2e2e2e; border-radius: 10px;");
    disableAll();

}

void MainWindow::onUndoButtonClicked(){
    hideAllFilterMenus(false);
    if (current > 0)
    {
        current--;
        myImage = history[current];
        QImage qimg = convertImageToQImage(myImage);
        updateImageLabel(qimg);
        updateSizeLabel(myImage.width, myImage.height);
    }
    else
    {
        QMessageBox::information(this, "Undo", "No more actions to undo!");
    }
}

void MainWindow::onRedoButtonClicked(){
    hideAllFilterMenus(false);
    if (current < (int)history.size() - 1)
    {
        current++;
        myImage = history[current];
        QImage qimg = convertImageToQImage(myImage);
        updateImageLabel(qimg);
        updateSizeLabel(myImage.width, myImage.height);
    }
    else
    {
        QMessageBox::information(this, "Redo", "No more actions to redo!");
    }
}

void MainWindow::onSaveButtonClicked()
{
    QString qpath = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                 "C:\\Users\\Alaa\\Desktop\\Babyphotoshop data\\CS213-2025-2026-Assignemnt1-SampleImages\\CS213-2025-2026-Assignemnt1-SampleImages\\CS112-Winter2023-2024-Assignemnt3-SampleImages\\",
                                                 tr("Images (*.png *.jpg *.bmp *.tga)"));
    hideAllFilterMenus(false);
    if(qpath.isEmpty()) return;
    string path = qpath.toStdString();
    myImage.saveImage(path);
}

void MainWindow::onApplyGrayScaleClicked() {
    if (current < (int)history.size() - 1)
        history.erase(history.begin() + current + 1, history.end());
    hideAllFilterMenus(false);
    gray_scale(myImage);

    history.push_back(myImage);
    current++;

    QImage qimg = convertImageToQImage(myImage);
    updateImageLabel(qimg);
}

void MainWindow::onApplyBlackWhiteClicked()
{
    if (current < (int)history.size() - 1)
        history.erase(history.begin() + current + 1, history.end());
    hideAllFilterMenus(false);
    Black_White(myImage);
    history.push_back(myImage);
    current++;
    QImage qimg = convertImageToQImage(myImage);
    updateImageLabel(qimg);
}


void MainWindow::onApplyInvertClicked()
{
    if (current < (int)history.size() - 1)
        history.erase(history.begin() + current + 1, history.end());
    hideAllFilterMenus(false);
    invert(myImage);
    history.push_back(myImage);
    current++;
    QImage qimg = convertImageToQImage(myImage);
    updateImageLabel(qimg);
}


void MainWindow::onApplyOldTVClicked()
{
    if (current < (int)history.size() - 1)
        history.erase(history.begin() + current + 1, history.end());
    hideAllFilterMenus(false);
    oldTVFilter(myImage);
    history.push_back(myImage);
    current++;
    QImage qimg = convertImageToQImage(myImage);
    updateImageLabel(qimg);
}


void MainWindow::onApplyOilPaintClicked()
{
    if (current < (int)history.size() - 1)
        history.erase(history.begin() + current + 1, history.end());
    hideAllFilterMenus(false);
    myImage = oil_painting(myImage);
    history.push_back(myImage);
    current++;
    QImage qimg = convertImageToQImage(myImage);
    updateImageLabel(qimg);
}


void MainWindow::onApplyEdgeDetectionClicked()
{
    if (current < (int)history.size() - 1)
        history.erase(history.begin() + current + 1, history.end());
    hideAllFilterMenus(false);
    myImage = edge_detection(myImage);
    history.push_back(myImage);
    current++;
    QImage qimg = convertImageToQImage(myImage);
    updateImageLabel(qimg);
}




bool MainWindow::eventFilter(QObject *obj, QEvent *event){
    if(obj == ui->imageLabel){
        if(event->type() == QEvent::MouseButtonPress && !ui->imageLabel->getCroppingStatus()){
            QImage qimg = convertImageToQImage(originalImage);
            updateImageLabel(qimg);
        }
        else if (event->type() == QEvent::MouseButtonRelease && !ui->imageLabel->getCroppingStatus())
        {
            QImage qimg = convertImageToQImage(myImage);
            updateImageLabel(qimg);
        }
    }
    return QObject::eventFilter(obj, event);
}



void MainWindow::onApplyInfraredClicked(){
    if (current < (int)history.size() - 1)
        history.erase(history.begin() + current + 1, history.end());
    hideAllFilterMenus(false);
    Infra_Red(myImage);
    history.push_back(myImage);
    current++;
    QImage qimg = convertImageToQImage(myImage);
    updateImageLabel(qimg);
}

void MainWindow::onApplyPurpleScaleClicked(){
    if (current < (int)history.size() - 1)
        history.erase(history.begin() + current + 1, history.end());
    hideAllFilterMenus(false);
    purple_scale(myImage);
    history.push_back(myImage);
    current++;
    QImage qimg = convertImageToQImage(myImage);
    updateImageLabel(qimg);
}

void MainWindow::onApplySunlightClicked(){
    if (current < (int)history.size() - 1)
        history.erase(history.begin() + current + 1, history.end());
    hideAllFilterMenus(false);
    sunlight_glow(myImage);
    history.push_back(myImage);
    current++;
    QImage qimg = convertImageToQImage(myImage);
    updateImageLabel(qimg);
}

void MainWindow::onApplyLeftRotateClicked(){
    if (current < (int)history.size() - 1)
        history.erase(history.begin() + current + 1, history.end());
    hideAllFilterMenus(false);
    myImage = rotate(myImage, 270);
    history.push_back(myImage);
    current++;
    updateSizeLabel(myImage.width, myImage.height);
    QImage qimg = convertImageToQImage(myImage);
    updateImageLabel(qimg);
}

void MainWindow::onApplyRightRotateClicked(){
    if (current < (int)history.size() - 1)
        history.erase(history.begin() + current + 1, history.end());
    hideAllFilterMenus(false);
    myImage = rotate(myImage, 90);
    history.push_back(myImage);
    current++;
    updateSizeLabel(myImage.width, myImage.height);
    QImage qimg = convertImageToQImage(myImage);
    updateImageLabel(qimg);
}

void MainWindow::onApplyFlipVerticalClicked(){
    if (current < (int)history.size() - 1)
        history.erase(history.begin() + current + 1, history.end());
    hideAllFilterMenus(false);
    flip(myImage, 1);
    history.push_back(myImage);
    current++;
    QImage qimg = convertImageToQImage(myImage);
    updateImageLabel(qimg);
}

void MainWindow::onApplyFlipHorizontalClicked(){
    if (current < (int)history.size() - 1)
        history.erase(history.begin() + current + 1, history.end());
    hideAllFilterMenus(false);
    flip(myImage, 2);
    history.push_back(myImage);
    current++;
    QImage qimg = convertImageToQImage(myImage);
    updateImageLabel(qimg);
}

void MainWindow::toggleFlipOptions(bool visible)
{
    flipOptionsVisible = visible;
    ui->flipVButton->setVisible(flipOptionsVisible);
    ui->flipHButton->setVisible(flipOptionsVisible);
    ui->flipButton->setVisible(!flipOptionsVisible);
    if(flipOptionsVisible){
        ui->flipVButton->raise();
        ui->flipHButton->raise();
    }
}

void MainWindow::onflipButtonClicked()
{
    hideAllFilterMenus(true);
}

void MainWindow::onCropRequested(const QRect &rect){
    if (!ui->imageLabel->pixmap()) return;

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirm Crop", "Do you want to crop this area?",
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply != QMessageBox::Yes) return;

    const QPixmap pixmap = ui->imageLabel->pixmap(Qt::ReturnByValue);
    QSize pixSize = pixmap.size();
    QSize lblSize = ui->imageLabel->size();

    double scaleX = double(pixSize.width()) / lblSize.width();
    double scaleY = double(pixSize.height()) / lblSize.height();

    double xOffset = 0, yOffset = 0;
    double scaledW = lblSize.width();
    double scaledH = lblSize.height();

    double imgAspect = double(pixSize.width()) / pixSize.height();
    double lblAspect = double(lblSize.width()) / lblSize.height();

    if (imgAspect > lblAspect) {
        scaledH = lblSize.width() / imgAspect;
        yOffset = (lblSize.height() - scaledH) / 2.0;
    } else {
        scaledW = lblSize.height() * imgAspect;
        xOffset = (lblSize.width() - scaledW) / 2.0;
    }

    QRectF cropRect(
        (rect.x() - xOffset) * (pixSize.width() / scaledW),
        (rect.y() - yOffset) * (pixSize.height() / scaledH),
        rect.width() * (pixSize.width() / scaledW),
        rect.height() * (pixSize.height() / scaledH)
        );
    QRect imageRect(0, 0, pixSize.width(), pixSize.height());
    QRect finalRect = cropRect.toRect().intersected(imageRect);


    if (current < (int)history.size() - 1)
        history.erase(history.begin() + current + 1, history.end());
    QPixmap cropped = pixmap.copy(finalRect);
    QImage cropped_qimage = cropped.toImage();
    myImage = convertQImageToImage(cropped_qimage);
    history.push_back(myImage);
    current++;
    updateSizeLabel(myImage.width, myImage.height);
    updateImageLabel(cropped_qimage);
}

void MainWindow::applyBrightnessSliderValue(int value){
    if (current < (int)history.size() - 1)
        history.erase(history.begin() + current + 1, history.end());
    darken_and_lighten(myImage, value);
    history.push_back(myImage);
    current++;
    hideAllFilterMenus(false);
    QImage qimg = convertImageToQImage(myImage);
    updateImageLabel(qimg);
}

void MainWindow::onApplyResizeClicked(int width, int height){
    if (current < (int)history.size() - 1)
        history.erase(history.begin() + current + 1, history.end());
    myImage = resize_image(myImage, width, height);
    history.push_back(myImage);
    current++;
    hideAllFilterMenus(false);
    QImage qimg = convertImageToQImage(myImage);
    updateImageLabel(qimg);
    updateSizeLabel(width, height);
}

void MainWindow::onWidthChanged(int newWidth)
{
    if (isUpdatingDimensions || !ui->keepAspectCheckBox->isChecked())
        return;

    isUpdatingDimensions = true;

    int newHeight = static_cast<int>(newWidth / originalAspectRatio);
    ui->heightBox->setValue(newHeight);

    isUpdatingDimensions = false;
}

void MainWindow::onHeightChanged(int newHeight)
{
    if (isUpdatingDimensions || !ui->keepAspectCheckBox->isChecked())
        return;

    isUpdatingDimensions = true;

    int newWidth = static_cast<int>(newHeight * originalAspectRatio);
    ui->widthBox->setValue(newWidth);

    isUpdatingDimensions = false;
}


void MainWindow::updateSizeLabel(int width, int height){
    ui->sizeLabel->show();
    QString size = QString("%1×%2").arg(width).arg(height);
    ui->sizeLabel->setText(size);
}

void MainWindow::hideAllFilterMenus(bool visible){
    ui->resizeWidget->hide();
    ui->brightnessWidget->hide();
    ui->blurWidget->hide();
    ui->skewWidget->hide();
    ui->mergeWidget->hide();
    ui->frameWidget->hide();
    ui->imageLabel->setCropping(false);
    toggleFlipOptions(visible);

}

void MainWindow::applyBlurSliderValue(int value){
    if (current < (int)history.size() - 1)
        history.erase(history.begin() + current + 1, history.end());
    value = max(1, min(100, value));
    double ratio = value / 100.0;
    int radius = max(1, int(min(myImage.width, myImage.height) * ratio / 80.0));
    Image temp = blur_image(myImage, radius, true);
    myImage = blur_image(temp, radius, false);
    history.push_back(myImage);
    current++;
    hideAllFilterMenus(false);
    QImage qimg = convertImageToQImage(myImage);
    updateImageLabel(qimg);
}

void MainWindow::applySkewValue(int value){
    if (current < (int)history.size() - 1)
        history.erase(history.begin() + current + 1, history.end());
    skewImage(myImage, value);
    history.push_back(myImage);
    current++;
    hideAllFilterMenus(false);
    QImage qimg = convertImageToQImage(myImage);
    updateImageLabel(qimg);
}

void MainWindow::onMergeTypeSelected(bool resize, bool overlap){
    if (current < (int)history.size() - 1)
        history.erase(history.begin() + current + 1, history.end());
    string mergeOption;
    if(resize)
        mergeOption = "resize";
    else if(overlap) mergeOption = "overlap";

    myImage = merge_image(myImage, mergeImage, mergeOption);
    history.push_back(myImage);
    current++;
    hideAllFilterMenus(false);
    QImage qimg = convertImageToQImage(myImage);
    updateImageLabel(qimg);
}

void MainWindow::onApplyNeonClicked(){
    if (current < (int)history.size() - 1)
        history.erase(history.begin() + current + 1, history.end());

    myImage = neon_filter(myImage);
    history.push_back(myImage);
    current++;
    hideAllFilterMenus(false);
    QImage qimg = convertImageToQImage(myImage);
    updateImageLabel(qimg);
}

void MainWindow::onApplyWaveClicked(){
    if (current < (int)history.size() - 1)
        history.erase(history.begin() + current + 1, history.end());

    myImage = water_wave(myImage);
    history.push_back(myImage);
    current++;
    hideAllFilterMenus(false);
    QImage qimg = convertImageToQImage(myImage);
    updateImageLabel(qimg);
}

void MainWindow::onApplyFrameClicked(){
    if (current < (int)history.size() - 1)
        history.erase(history.begin() + current + 1, history.end());

    bool simple = ui->simpleRadioButton->isChecked();
    bool dbl = ui->doubleRadioButton->isChecked();
    bool fancy = ui->fancyRadioButton->isChecked();
    unsigned char r, g, b, wr, wg, wb;
    bool ok{};
    int thickness, innerThickness=-1;
    if(simple){
        thickness = QInputDialog::getInt(this, tr("Enter thickness:"), tr("Input"));
        QColor color = QColorDialog::getColor(Qt::white, this, "Choose Color");
        r = color.red();
        g = color.green();
        b = color.blue();
    }
    else if(fancy){
        thickness = QInputDialog::getInt(this, tr("Enter thickness:"), tr("Input"));
        QColor color = QColorDialog::getColor(Qt::white, this, "Choose Color");
        r = color.red();
        g = color.green();
        b = color.blue();
    }
    else if(dbl){
        thickness = QInputDialog::getInt(this, tr("Enter Outer Thickness:"), tr("Input"));
        QColor color = QColorDialog::getColor(Qt::white, this, "Choose Outer Color");
        r = color.red();
        g = color.green();
        b = color.blue();
        innerThickness = QInputDialog::getInt(this, tr("Enter Inner Thickness:"), tr("Input"));
        QColor color2 = QColorDialog::getColor(Qt::white, this, "Choose Inner Color");
        wr = color2.red();
        wg = color2.green();
        wb = color2.blue();

    }
    else return;
    frame(myImage, thickness, (innerThickness==-1?1:innerThickness), (fancy?2:1), dbl, r, g, b, wr, wb, wg);
    history.push_back(myImage);
    current++;
    hideAllFilterMenus(false);
    QImage qimg = convertImageToQImage(myImage);
    updateImageLabel(qimg);
}

void MainWindow::onApplyShakeClicked(){
    if (current < (int)history.size() - 1)
        history.erase(history.begin() + current + 1, history.end());

    shakeFilter(myImage);
    history.push_back(myImage);
    current++;
    hideAllFilterMenus(false);
    QImage qimg = convertImageToQImage(myImage);
    updateImageLabel(qimg);
}

void MainWindow::disableAll(){
    ui->clearButton->setEnabled(false);
    ui->deleteButton->setEnabled(false);
    ui->saveButton->setEnabled(false);
    ui->undoButton->setEnabled(false);
    ui->redoButton->setEnabled(false);
    ui->blackwhitebutton->setEnabled(false);
    ui->cropButton->setEnabled(false);
    ui->blurButton->setEnabled(false);
    ui->brightnessButton->setEnabled(false);
    ui->edgesButton->setEnabled(false);
    ui->grayButton->setEnabled(false);
    ui->infraredButton->setEnabled(false);
    ui->flipButton->setEnabled(false);
    ui->frameButton->setEnabled(false);
    ui->leftButton->setEnabled(false);
    ui->rightButton->setEnabled(false);
    ui->purpleButton->setEnabled(false);
    ui->waveButton->setEnabled(false);
    ui->neonButton->setEnabled(false);
    ui->oldtvbutton->setEnabled(false);
    ui->mergeButton->setEnabled(false);
    ui->invertbutton->setEnabled(false);
    ui->sunlightButton->setEnabled(false);
    ui->oilpaintButton->setEnabled(false);
    ui->resizeButton->setEnabled(false);
    ui->skewButton->setEnabled(false);
    ui->shakeButton->setEnabled(false);
    ui->sizeLabel->hide();
    saveShortcut->setEnabled(false);
    deleteShortcut->setEnabled(false);
    undoShortcut->setEnabled(false);
    redoShortcut->setEnabled(false);
}

void MainWindow::enableAll(){
    ui->clearButton->setEnabled(true);
    ui->deleteButton->setEnabled(true);
    ui->saveButton->setEnabled(true);
    ui->undoButton->setEnabled(true);
    ui->redoButton->setEnabled(true);
    ui->blackwhitebutton->setEnabled(true);
    ui->cropButton->setEnabled(true);
    ui->blurButton->setEnabled(true);
    ui->brightnessButton->setEnabled(true);
    ui->edgesButton->setEnabled(true);
    ui->grayButton->setEnabled(true);
    ui->infraredButton->setEnabled(true);
    ui->flipButton->setEnabled(true);
    ui->frameButton->setEnabled(true);
    ui->leftButton->setEnabled(true);
    ui->rightButton->setEnabled(true);
    ui->purpleButton->setEnabled(true);
    ui->waveButton->setEnabled(true);
    ui->neonButton->setEnabled(true);
    ui->oldtvbutton->setEnabled(true);
    ui->mergeButton->setEnabled(true);
    ui->invertbutton->setEnabled(true);
    ui->sunlightButton->setEnabled(true);
    ui->oilpaintButton->setEnabled(true);
    ui->resizeButton->setEnabled(true);
    ui->skewButton->setEnabled(true);
    ui->shakeButton->setEnabled(true);
    saveShortcut->setEnabled(true);
    deleteShortcut->setEnabled(true);
    undoShortcut->setEnabled(true);
    redoShortcut->setEnabled(true);
}

void MainWindow::resetAll(){
    ui->blurSlider->setValue(0);
    ui->blurValueLabel->setText(QString("0%"));
    ui->brightnessSlider->setValue(0);
    ui->brightnessValueLabel->setText(QString("0%"));
    ui->overlapRadioButton->clicked(false);
    ui->resizeRadioButton->clicked(false);
    ui->skewBox->setValue(0);
    ui->simpleRadioButton->clicked(false);
    ui->doubleRadioButton->clicked(false);
    ui->fancyRadioButton->clicked(false);
    ui->heightBox->setValue(0);
    ui->widthBox->setValue(0);
}
