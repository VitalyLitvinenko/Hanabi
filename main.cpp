#include <QApplication>

#include "network/client.h"
#include "network/server.h"

#include <QtWidgets>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Server server;
    // server.show();

    Client client;
    client.show();

    return app.exec();
}
