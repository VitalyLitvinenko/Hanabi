#pragma once

#include "network/protocol.h"

#include <QTcpSocket>
#include <QtWidgets>
#include <QInputDialog>

#include "widgets/boardwidget.h"

class Client : public QWidget {
    Q_OBJECT

private:
    QString _host;
    QString _name;
    QTcpSocket *_tcp_socket;

    QTextEdit *_txt_info;
    QLineEdit *_txt_input;
    quint16 _next_block_size;

    BoardWidget *_board_widget;

public:
    explicit Client(QWidget *pwgt = nullptr);

    void SetHost(const QString& host);
    void SetName(const QString& name);
    void SetTcpSocket(QTcpSocket *tcp_socket);

private slots:
    void slotReadyRead();
    void slotSendToServer(Command command, const QString& str);

    void slotSendMessageToServer();
    void slotPlayCard(int card_no);
    void slotDumpCard(int card_no);
    void slotHintColor(int gamer_no, CARD_COLOR color);
    void slotHintRank(int gamer_no, uint8_t color);
};
