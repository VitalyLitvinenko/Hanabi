#pragma once

#include <QLabel>

class DeckWidget : public QLabel {
    Q_OBJECT

private:
    size_t _deck_size;

public:
    explicit DeckWidget(size_t dec_size, QWidget *parent = nullptr);
    void SetSize(size_t new_size);

private slots:
    void RefreshSize();
};
