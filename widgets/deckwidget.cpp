#include "deckwidget.h"

DeckWidget::DeckWidget(size_t dec_size, QWidget *parent) : QLabel(parent) {
    setAlignment(Qt::AlignCenter);
    setFont(QFont("Arial",40));
    setFixedSize(70,100);
    SetSize(dec_size);
}

void DeckWidget::SetSize(size_t new_size) {
    _deck_size = new_size;
    RefreshSize();
}

void DeckWidget::RefreshSize() {
    if (_deck_size > 0) {
        setText(QString::number(_deck_size));
        setFrameStyle(3);
        setFrameShadow(Shadow::Raised);
        setFrameShape(Shape::WinPanel);
    } else {
        setText("");
        setFrameStyle(0);
        setFrameShadow(Shadow::Plain);
    }
}
