#include "server.h"

Server::Server(QWidget *pwgt) : QWidget(pwgt), m_nNextBlockSize(0) {
    m_ptcpServer = new QTcpServer(this);
    if (!m_ptcpServer->listen(QHostAddress::Any, SERVER_PORT)) {
        QMessageBox::critical(nullptr, "ServerError",
                               "Unable to start the server: "
                               + m_ptcpServer->errorString());
        m_ptcpServer->close();
        return;
    }
    connect(m_ptcpServer, SIGNAL(newConnection()), this, SLOT(slotNewConnection()));
    m_ptxt = new QTextEdit;
    m_ptxt->setReadOnly(true);

    QVBoxLayout *pvbxLayoyt = new QVBoxLayout;
    pvbxLayoyt->addWidget(new QLabel("<H1>Hanabi server</H1>"));
    pvbxLayoyt->addWidget(new QLabel(QString("<H1>IP: ") + GetLocalIP() + "</H1>"));
    pvbxLayoyt->addWidget(m_ptxt);

    _start_btn = new QPushButton("Start game");
    pvbxLayoyt->addWidget(_start_btn);
    connect(_start_btn, SIGNAL(clicked()), this, SLOT(slotStartGame()));

    setLayout(pvbxLayoyt);
}

void Server::slotNewConnection() {
    QTcpSocket *pClientSocket = m_ptcpServer->nextPendingConnection();
    m_ptxt->append("Client " + QString::number(pClientSocket->peerPort()) + " connected");
    connect(pClientSocket, SIGNAL(disconnected()), this, SLOT(slotDisconnected()));
    connect(pClientSocket, SIGNAL(readyRead()),this,SLOT(slotReadClient()));
    clients[pClientSocket] = {QString::number(pClientSocket->peerPort()), clients.size()};
    sendToClient(Command::MESSAGE, pClientSocket, "Connected!");
}

void Server::slotDisconnected() {
    QTcpSocket *pClientSocket = (QTcpSocket*)sender();
    m_ptxt->append("Client " + clients[pClientSocket].first + " disconnected");
    clients.remove(pClientSocket);
    emit pClientSocket->deleteLater();
}

void Server::slotReadClient() {
    QTcpSocket *pClientSocket = (QTcpSocket*)sender();
    QDataStream in(pClientSocket);
    in.setVersion(QDataStream::Qt_5_3);
    for (;;) {
        if (!m_nNextBlockSize) {
            if (pClientSocket->bytesAvailable() < static_cast<qint64>(sizeof(quint16))) {
                break;
            }
            in >> m_nNextBlockSize;
        }
        if (pClientSocket->bytesAvailable() < m_nNextBlockSize) {
            break;
        }
        QTime time;
        QString str;
        quint8 command;
        QString strMessage;
        QString buf;
        QString info;
        QByteArray byte_array;
        in >> time >> command >> str;
        switch (static_cast<Command>(command)) {
        case Command::NAME:
            buf = clients[pClientSocket].first;
            clients[pClientSocket].first = str;

            if (board.AddGamer(clients[pClientSocket].first.toStdString())) {
                info = clients[pClientSocket].first + " added to the game";
            } else {
                info = clients[pClientSocket].first + " can not be added to the game";
            }

            m_ptxt->append(info);
            sendToAllClients(info);

            m_ptxt->append("Rename " + buf + " -> " + str);

            break;
        case Command::MESSAGE:
            strMessage = time.toString() + " client " +  clients[pClientSocket].first + " print: " + str;
            m_ptxt->append(strMessage);
            sendToAllClients(clients[pClientSocket].first + ": " + str);
            break;
        case Command::PLAY:
            byte_array = str.toUtf8();
            byte_array = byte_array.fromBase64(byte_array);
            board.PlayCard(byte_array.front());
            SendNewBoard();
            m_ptxt->append("Card played");
            break;
        case Command::DUMP:
            byte_array = str.toUtf8();
            byte_array = byte_array.fromBase64(byte_array);
            board.DumpCard(byte_array.front());
            SendNewBoard();
            m_ptxt->append("Card dumped");
            break;
        case Command::HINT_COLOR:
            byte_array = str.toUtf8();
            byte_array = byte_array.fromBase64(byte_array);
            board.PromptColor(byte_array.at(0), static_cast<CARD_COLOR>(byte_array.at(1)));
            m_ptxt->append("Color hinted");
            SendNewBoard();
            break;
        case Command::HINT_RANK:
            byte_array = str.toUtf8();
            byte_array = byte_array.fromBase64(byte_array);
            board.PromptRank(byte_array.at(0), byte_array.at(1));
            SendNewBoard();
            m_ptxt->append("Rank hinted");
            break;
        default:
            break;
        }
        m_nNextBlockSize = 0;
    }
}

void Server::sendToClient(Command command, QTcpSocket *pSocket, const QString& str) {
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_3);
    out << quint16(0) << QTime::currentTime() << quint8(command) << str;

    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));
    pSocket->write(arrBlock);
}

void Server::sendToAllClients(const QString& str) {
    for(auto p_soc_with_name : clients.toStdMap()) {
        sendToClient(Command::MESSAGE, p_soc_with_name.first, str);
    };
}

QString Server::GetLocalIP() {
    const QHostAddress& localhost = QHostAddress(QHostAddress::LocalHost);
    for (const QHostAddress& address : QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != localhost) {
            return address.toString();
        }
    }
    return "unknown";
}

void Server::slotStartGame() {
    if (board.TryToStartGame()) {
        m_ptxt->append("Game started");
        _start_btn->setDisabled(true);
        SendNewBoard();
    } else {
        m_ptxt->append("Game not started");
    }
}

void Server::SendNewBoard() {
    auto data = board.ToByteArray();
    for (auto client : clients.toStdMap()) {
        auto client_data = data;
        client_data.push_back(client.second.second);
        QByteArray byte_array((char*)client_data.data(), client_data.size());
        sendToClient(Command::BOARD, client.first, byte_array.toBase64());
    }
}
