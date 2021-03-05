#pragma once

#include <set>
#include "../core/dump.h"
#include "cardwidget.h"

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

class DumpWidget : public QWidget {
    Q_OBJECT

public:
    explicit DumpWidget(const Dump& dump, QWidget *parent = nullptr);

};
