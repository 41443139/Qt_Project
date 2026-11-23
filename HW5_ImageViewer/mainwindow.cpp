#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QStyle>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 補上 Qt 內建系統圖示
    ui->actionOpen_Image->setIcon(style()->standardIcon(QStyle::SP_DialogOpenButton));
    ui->actionSave->setIcon(style()->standardIcon(QStyle::SP_DialogSaveButton));
    ui->actionExit->setIcon(style()->standardIcon(QStyle::SP_DialogCloseButton));
    ui->actionZoom_In->setIcon(style()->standardIcon(QStyle::SP_ArrowUp));
    ui->actionZoom_Out->setIcon(style()->standardIcon(QStyle::SP_ArrowDown));
    ui->actionReset_Image->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));

    // 讓 Toolbar 同時顯示圖示與文字
    ui->toolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    ui->label->setText("請點擊 'Open Image' 開啟圖片");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_Image_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "開啟圖片",
        "",
        "Images (*.png *.jpg *.jpeg *.bmp *.gif)"
        );

    if (!fileName.isEmpty()) {
        if (currentImage.load(fileName)) {
            ui->label->setPixmap(QPixmap::fromImage(currentImage));
            ui->label->adjustSize();
            statusBar()->showMessage("已載入: " + fileName);
        } else {
            QMessageBox::warning(this, "錯誤", "無法載入該圖片檔案！");
        }
    }
}

void MainWindow::on_actionSave_triggered()
{
    if (currentImage.isNull()) {
        QMessageBox::information(this, "提示", "目前沒有可儲存的圖片！");
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(
        this,
        "儲存圖片",
        "output.png",
        "PNG (*.png);;JPEG (*.jpg *.jpeg);;BMP (*.bmp)"
        );

    if (!fileName.isEmpty()) {
        QPixmap currentPix = ui->label->pixmap();
        if (!currentPix.isNull() && currentPix.save(fileName)) {
            statusBar()->showMessage("已儲存至: " + fileName);
        } else {
            QMessageBox::warning(this, "錯誤", "儲存失敗！");
        }
    }
}

void MainWindow::on_actionExit_triggered()
{
    close();
}

void MainWindow::on_actionZoom_In_triggered()
{
    if (!currentImage.isNull()) {
        QPixmap pix = ui->label->pixmap();
        ui->label->setPixmap(pix.scaled(pix.size() * 1.2, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}

void MainWindow::on_actionZoom_Out_triggered()
{
    if (!currentImage.isNull()) {
        QPixmap pix = ui->label->pixmap();
        ui->label->setPixmap(pix.scaled(pix.size() * 0.8, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}

void MainWindow::on_actionReset_Image_triggered()
{
    if (!currentImage.isNull()) {
        ui->label->setPixmap(QPixmap::fromImage(currentImage));
    }
}