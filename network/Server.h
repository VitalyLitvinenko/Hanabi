#pragma once

#include "network/protocol.h"
#include "core/board.h"

#include <QtWidgets>
#include <QTcpServer>
#include <QTcpSocket>
#include <QNetworkInterface>

//====================================================================

class Server : public QWidget {
    Q_OBJECT

private:
    QTcpServer *m_ptcpServer;
    QTextEdit* m_ptxt;
    quint16 m_nNextBlockSize;
    QMap<QTcpSocket*, QPair<QString, size_t>> clients;
    QPushButton *_start_btn;

    Board board;

private:
    void sendToClient(Command command, QTcpSocket *pSocket, const QString& str);
    void sendToAllClients(const QString& str);

    static QString GetLocalIP();

    void SendNewBoard();

public:
    explicit Server(QWidget *pwgt=nullptr);

public slots:
    void slotNewConnection();
    void slotDisconnected();
    void slotReadClient();

    void slotStartGame();
};
