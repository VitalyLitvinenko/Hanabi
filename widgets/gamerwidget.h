#pragma once

#include "../core/gamer.h"

// #include <QWidget>
#include <QFrame>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "cardslotwidget.h"

class GamerWidget : public QFrame {
    Q_OBJECT

private:
    QHBoxLayout *hLayout;

public:
    explicit GamerWidget(const Gamer& gamer, bool is_other_gamer_view, bool is_other_gamer_do, QWidget *parent = nullptr);

public slots:
    void slotCardClicked(CARD_COLOR, uint8_t);

signals:
    void signalActiveCard(int, CARD_COLOR, uint8_t);
};
