#include "dialog.h"

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
{
    serverProgressBar = new QProgressBar;
    serverStatusLabel = new QLabel(tr("伺服器端就緒"));
    startButton = new QPushButton(tr("接收"));
    quitButton = new QPushButton(tr("退出"));

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(startButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);

    connect(startButton, &QPushButton::clicked, this, &Dialog::start);
    connect(quitButton, &QPushButton::clicked, this, &Dialog::close);
    connect(&tcpServer, &QTcpServer::newConnection, this, &Dialog::acceptConnection);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(serverProgressBar);
    mainLayout->addWidget(serverStatusLabel);
    mainLayout->addStretch();
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);

    setWindowTitle(tr("接收檔案"));
}

Dialog::~Dialog()
{
}

void Dialog::start()
{
    startButton->setEnabled(false);

    while (!tcpServer.isListening() && !tcpServer.listen(QHostAddress::Any, 16998)) {
        QMessageBox::StandardButton ret = QMessageBox::critical(
            this,
            tr("迴圈"),
            tr("無法啟動伺服器: %1.").arg(tcpServer.errorString()),
            QMessageBox::Retry | QMessageBox::Cancel
            );
        if (ret == QMessageBox::Cancel)
            return;
    }
    serverStatusLabel->setText(tr("監聽中..."));
}

void Dialog::acceptConnection()
{
    tcpServerConnection = tcpServer.nextPendingConnection();
    connect(tcpServerConnection, &QIODevice::readyRead, this, &Dialog::updateServerProgress);
    connect(tcpServerConnection, &QAbstractSocket::errorOccurred, this, &Dialog::displayError);

    serverStatusLabel->setText(tr("接受連線"));
    tcpServer.close();
}

void Dialog::updateServerProgress()
{
}

void Dialog::displayError(QAbstractSocket::SocketError socketError)
{
    if (socketError == QTcpSocket::RemoteHostClosedError)
        return;

    QMessageBox::information(this, tr("網路錯誤"),
                             tr("產生如下錯誤: %1.").arg(tcpServerConnection->errorString()));
    tcpServerConnection->close();
    serverProgressBar->reset();
    serverStatusLabel->setText(tr("伺服器端就緒"));
    startButton->setEnabled(true);
}