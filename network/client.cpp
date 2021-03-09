#include "client.h"

Client::Client(QWidget *pwgt) : QWidget(pwgt), _next_block_size(0) {
    _name = QInputDialog::getText(this, "Client configuration", "Enter your name");
    _host = QInputDialog::getText(this, "Client configuration", "Enter server IP");
    QHBoxLayout *main_layout = new QHBoxLayout;
    _board_widget = new BoardWidget;

    connect (_board_widget, SIGNAL(signalPlayCard(int)),              this, SLOT(slotPlayCard(int)));
    connect (_board_widget, SIGNAL(signalDumpCard(int)),              this, SLOT(slotDumpCard(int)));
    connect (_board_widget, SIGNAL(signalHintColor(int, CARD_COLOR)), this, SLOT(slotHintColor(int, CARD_COLOR)));
    connect (_board_widget, SIGNAL(signalHintRank(int, uint8_t)),     this, SLOT(slotHintRank(int, uint8_t)));

    main_layout->addWidget(_board_widget);

    _txt_info = new QTextEdit;
    _txt_input = new QLineEdit;

    _txt_info->setReadOnly(true);
    QPushButton *pcmd = new QPushButton("&Send");
    connect(pcmd, SIGNAL(clicked()), SLOT(slotSendMessageToServer()));
    connect(_txt_input, SIGNAL(returnPressed()), this, SLOT(slotSendMessageToServer()));

    QVBoxLayout *pvbxLayout = new QVBoxLayout;
    pvbxLayout->addWidget(new QLabel("<H1>Client</H1>"));
    pvbxLayout->addWidget(_txt_info);
    pvbxLayout->addWidget(_txt_input);
    pvbxLayout->addWidget(pcmd);
    main_layout->addLayout(pvbxLayout);
    setLayout(main_layout);

    _tcp_socket = new QTcpSocket(this);
    _tcp_socket->connectToHost(_host, SERVER_PORT);
    connect(_tcp_socket, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
    slotSendToServer(Command::NAME, _name);
}

void Client::SetHost(const QString& host) {
    _host = host;
}

void Client::SetName(const QString& name) {
    _name = name;
}

void Client::slotReadyRead() {
    QDataStream in(_tcp_socket);
    in.setVersion(QDataStream::Qt_5_3);
    for (;;) {
        if (!_next_block_size) {
            if (_tcp_socket->bytesAvailable() < static_cast<qint64>(sizeof(quint16))) {
                break;
            }
            in >> _next_block_size;
        }
        if (_tcp_socket->bytesAvailable() < _next_block_size)
        break;
        QTime time;
        QString str;
        quint8 command;
        in >> time >> command >> str;
        if (static_cast<Command>(command) == Command::MESSAGE) {
            _txt_info->append(time.toString() + " " + str);
        } else if (static_cast<Command>(command) == Command::BOARD) {
            QByteArray byte_array = str.toUtf8();
            byte_array = byte_array.fromBase64(byte_array);
            std::vector<uint8_t> data(byte_array.begin(), byte_array.end());
            auto gamer_no = data.back();
            data.pop_back();
            Board board(data);
            _board_widget->Refresh(board, gamer_no);
        }
        _next_block_size = 0;
    }
}

void Client::slotSendToServer(Command command, const QString& str) {
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_3);
    out << quint16(0) << QTime::currentTime() << quint8(command) << str;
    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));
    _tcp_socket->write(arrBlock);
}

void Client::slotSendMessageToServer() {
    slotSendToServer(Command::MESSAGE, _txt_input->text());
    _txt_input->setText("");
}

void Client::slotPlayCard(int card_no) {
    QByteArray arr;
    arr.push_back(card_no);
    slotSendToServer(Command::PLAY, arr.toBase64());
}

void Client::slotDumpCard(int card_no) {
    QByteArray arr;
    arr.push_back(card_no);
    slotSendToServer(Command::DUMP, arr.toBase64());
}

void Client::slotHintColor(int gamer_no, CARD_COLOR color) {
    QByteArray arr;
    arr.push_back(gamer_no);
    arr.push_back(static_cast<quint8>(color));
    slotSendToServer(Command::HINT_COLOR, arr.toBase64());
}

void Client::slotHintRank(int gamer_no, uint8_t color) {
    QByteArray arr;
    arr.push_back(gamer_no);
    arr.push_back(static_cast<quint8>(color));
    slotSendToServer(Command::HINT_RANK, arr.toBase64());
}

// void Client::slotSendNameToServer(const QString& name) {
//     QByteArray arrBlock;
//     QDataStream out(&arrBlock, QIODevice::WriteOnly);
//     out.setVersion(QDataStream::Qt_5_3);
//     out << quint16(0) << QTime::currentTime() << quint8(Command::NAME) << name /*m_ptxtInput->text()*/;
//
//     out.device()->seek(0);
//     out << quint16(arrBlock.size() - sizeof(quint16));
//
//     m_pTcpSocket->write(arrBlock);
//     // m_ptxtInput->setText("");
// }

