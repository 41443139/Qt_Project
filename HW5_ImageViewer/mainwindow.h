#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QToolBar>
#include <QAction>
#include <QLabel>
#include <QScrollArea>
#include <QImage>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void openImage();
    void saveImage();
    void aboutApp();

private:
    void createActions();
    void createMenus();
    void createToolBars();

    // Central Widget 相關元件
    QScrollArea *scrollArea;
    QLabel *imageLabel;
    QImage currentImage;

    // Menus
    QMenu *fileMenu;
    QMenu *imageMenu;
    QMenu *editMenu;

    // ToolBar
    QToolBar *mainToolBar;

    // Actions
    QAction *openAction;
    QAction *saveAction;
    QAction *exitAction;
    QAction *zoomInAction;
    QAction *zoomOutAction;
    QAction *resetAction;
};

#endif // MAINWINDOW_H