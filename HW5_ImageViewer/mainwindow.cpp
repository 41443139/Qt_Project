#include "mainwindow.h"
#include <QMenuBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QStyle>
#include <QStatusBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Desktop Image Processor - HW5");
    resize(800, 600);

    // 1. 設定 CentralWidget
    imageLabel = new QLabel(this);
    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setText("請點擊 'Open Image' 開啟圖片");

    scrollArea = new QScrollArea(this);
    scrollArea->setWidget(imageLabel);
    scrollArea->setWidgetResizable(true);
    setCentralWidget(scrollArea);

    // 2. 初始化 Actions, Menus, ToolBars
    createActions();
    createMenus();
    createToolBars();

    statusBar()->showMessage("就緒");
}

MainWindow::~MainWindow()
{
}

void MainWindow::createActions()
{
    // Open Image Action (共用給 File Menu 與 ToolBar)
    openAction = new QAction(style()->standardIcon(QStyle::SP_DialogOpenButton), "Open Image", this);
    openAction->setShortcut(QKeySequence::Open); // Ctrl+O
    openAction->setStatusTip("開啟影像檔案");
    connect(openAction, &QAction::triggered, this, &MainWindow::openImage);

    // Save Action (指定 Ctrl+S 快捷鍵)
    saveAction = new QAction(style()->standardIcon(QStyle::SP_DialogSaveButton), "Save", this);
    saveAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_S)); // Ctrl+S
    saveAction->setStatusTip("儲存目前影像");
    connect(saveAction, &QAction::triggered, this, &MainWindow::saveImage);

    // Exit Action
    exitAction = new QAction(style()->standardIcon(QStyle::SP_DialogCloseButton), "Exit", this);
    exitAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Q));
    exitAction->setStatusTip("離開應用程式");
    connect(exitAction, &QAction::triggered, this, &QWidget::close);

    // Image / Edit 預留示範用 Actions
    zoomInAction = new QAction(style()->standardIcon(QStyle::SP_ArrowUp), "Zoom In", this);
    zoomInAction->setShortcut(QKeySequence::ZoomIn);
    connect(zoomInAction, &QAction::triggered, this, [this]() {
        if (!currentImage.isNull()) {
            QPixmap pix = imageLabel->pixmap();
            imageLabel->setPixmap(pix.scaled(pix.size() * 1.2, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }
    });

    zoomOutAction = new QAction(style()->standardIcon(QStyle::SP_ArrowDown), "Zoom Out", this);
    zoomOutAction->setShortcut(QKeySequence::ZoomOut);
    connect(zoomOutAction, &QAction::triggered, this, [this]() {
        if (!currentImage.isNull()) {
            QPixmap pix = imageLabel->pixmap();
            imageLabel->setPixmap(pix.scaled(pix.size() * 0.8, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }
    });

    resetAction = new QAction(style()->standardIcon(QStyle::SP_BrowserReload), "Reset Image", this);
    resetAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_R));
    connect(resetAction, &QAction::triggered, this, [this]() {
        if (!currentImage.isNull()) {
            imageLabel->setPixmap(QPixmap::fromImage(currentImage));
        }
    });
}

void MainWindow::createMenus()
{
    // File Menu
    fileMenu = menuBar()->addMenu("&File");
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    // Image Menu
    imageMenu = menuBar()->addMenu("&Image");
    imageMenu->addAction(zoomInAction);
    imageMenu->addAction(zoomOutAction);

    // Edit Menu
    editMenu = menuBar()->addMenu("&Edit");
    editMenu->addAction(resetAction);
}

void MainWindow::createToolBars()
{
    mainToolBar = addToolBar("Main Toolbar");
    mainToolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon); // 同時顯示圖示與文字

    // 將同一個 openAction 與 saveAction 加入 ToolBar
    mainToolBar->addAction(openAction);
    mainToolBar->addAction(saveAction);
    mainToolBar->addSeparator();
    mainToolBar->addAction(zoomInAction);
    mainToolBar->addAction(zoomOutAction);
    mainToolBar->addAction(resetAction);
}

void MainWindow::openImage()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "開啟圖片",
        "",
        "Images (*.png *.jpg *.jpeg *.bmp *.gif)"
        );

    if (!fileName.isEmpty()) {
        if (currentImage.load(fileName)) {
            imageLabel->setPixmap(QPixmap::fromImage(currentImage));
            imageLabel->adjustSize();
            statusBar()->showMessage("已載入: " + fileName);
        } else {
            QMessageBox::warning(this, "錯誤", "無法載入該圖片檔案！");
        }
    }
}

void MainWindow::saveImage()
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
        // 儲存目前畫面上顯示的狀態（若未縮放則存原圖）
        QPixmap currentPix = imageLabel->pixmap();
        if (!currentPix.isNull() && currentPix.save(fileName)) {
            statusBar()->showMessage("已儲存至: " + fileName);
        } else {
            QMessageBox::warning(this, "錯誤", "儲存失敗！");
        }
    }
}

void MainWindow::aboutApp()
{
    QMessageBox::about(this, "關於", "Qt MainWindow 影像處理工具作業五");
}