#include "console_color.h"

#include <iostream>

static constexpr int CELL_W = 2;

// B=sky, G=grass, Y=sun, O=walls, W=roof, C=window,
// .=outside texture (magenta), ' '=empty, R=red(mark)

#ifdef _WIN32
  #include <windows.h>

static WORD BgAttr(char c) {
    switch (c) {
        case 'B': return BACKGROUND_BLUE | BACKGROUND_INTENSITY;
        case 'G': return BACKGROUND_GREEN | BACKGROUND_INTENSITY;
        case 'Y': return BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY;
        case 'O': return BACKGROUND_RED | BACKGROUND_GREEN;
        case 'W': return BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY;
        case 'C': return BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY;
        case '.': return BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_INTENSITY;
        case 'R': return BACKGROUND_RED | BACKGROUND_INTENSITY;
        case ' ': return 0;
        default:  return BACKGROUND_INTENSITY;
    }
}
#else
static int BgAnsi(char c) {
    switch (c) {
        case 'B': return 44;
        case 'G': return 42;
        case 'Y': return 43;
        case 'O': return 43;
        case 'W': return 47;
        case 'C': return 46;
        case '.': return 45;
        case 'R': return 41;
        case ' ': return 0;
        default:  return 100;
    }
}
#endif

void PrintColoredImage(const Image& img) {
    Size s = GetImageSize(img);

#ifdef _WIN32
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_SCREEN_BUFFER_INFO info{};
    GetConsoleScreenBufferInfo(h, &info);
    const WORD default_attr = info.wAttributes;
    const WORD fg_only = (WORD)(default_attr & 0x000F);

    auto WriteA = [&](const std::string& str) {
        DWORD written = 0;
        WriteConsoleA(h, str.c_str(), (DWORD)str.size(), &written, nullptr);
    };

    auto ClearTailCurrentLine = [&]() {
        CONSOLE_SCREEN_BUFFER_INFO ci{};
        if (!GetConsoleScreenBufferInfo(h, &ci)) return;
        COORD cur = ci.dwCursorPosition;
        int cells = ci.dwSize.X - cur.X;
        if (cells <= 0) return;

        DWORD written = 0;
        FillConsoleOutputCharacterA(h, ' ', (DWORD)cells, cur, &written);
        FillConsoleOutputAttribute (h, default_attr, (DWORD)cells, cur, &written);
    };

    auto EndLine = [&]() {
        SetConsoleTextAttribute(h, default_attr);
        ClearTailCurrentLine();
        WriteA("\n");
    };

    auto PrintCell = [&](char code) {
        if (code == ' ') {
            SetConsoleTextAttribute(h, default_attr);
            WriteA(std::string(CELL_W, ' '));
            return;
        }
        WORD bg = BgAttr(code);
        SetConsoleTextAttribute(h, (WORD)(fg_only | bg));
        WriteA(std::string(CELL_W, ' '));
        SetConsoleTextAttribute(h, default_attr);
    };

    WriteA("  +");
    for (int i = 0; i < s.width; ++i) WriteA(std::string(CELL_W, '-'));
    WriteA("+ ");
    EndLine();

    for (int y = 0; y < s.height; ++y) {
        WriteA("  |");
        for (int x = 0; x < s.width; ++x) PrintCell(img[y][x]);
        WriteA(" |");
        EndLine();
    }

    WriteA("  +");
    for (int i = 0; i < s.width; ++i) WriteA(std::string(CELL_W, '-'));
    WriteA("+ ");
    EndLine();

    SetConsoleTextAttribute(h, default_attr);

#else
    auto PrintCell = [&](char code) {
        int bg = BgAnsi(code);
        if (bg == 0) { std::cout << std::string(CELL_W, ' '); return; }
        std::cout << "\x1b[" << bg << "m" << std::string(CELL_W, ' ') << "\x1b[0m";
    };

    std::cout << "  +";
    for (int i = 0; i < s.width; ++i) std::cout << std::string(CELL_W, '-');
    std::cout << "+\n";

    for (int y = 0; y < s.height; ++y) {
        std::cout << "  |";
        for (int x = 0; x < s.width; ++x) PrintCell(img[y][x]);
        std::cout << "|\n";
    }

    std::cout << "  +";
    for (int i = 0; i < s.width; ++i) std::cout << std::string(CELL_W, '-');
    std::cout << "+\n";
#endif
}
