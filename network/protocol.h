#pragma once

constexpr int SERVER_PORT = 20810;

enum class Command : char {
    NAME = 0,
    BOARD,
    MESSAGE,
    PLAY,
    DUMP,
    HINT_COLOR,
    HINT_RANK
};
