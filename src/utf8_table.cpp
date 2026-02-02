#include "utf8_table.h"

#include <iostream>
#include <algorithm>
#include <cstdint>

static uint32_t NextCp(const std::string& s, size_t& i) {
    unsigned char c = (unsigned char)s[i++];
    if (c < 0x80) return c;

    if ((c >> 5) == 0x6) {
        uint32_t cp = (c & 0x1F) << 6;
        cp |= (unsigned char)s[i++] & 0x3F;
        return cp;
    }
    if ((c >> 4) == 0xE) {
        uint32_t cp = (c & 0x0F) << 12;
        cp |= ((unsigned char)s[i++] & 0x3F) << 6;
        cp |= (unsigned char)s[i++] & 0x3F;
        return cp;
    }
    uint32_t cp = (c & 0x07) << 18;
    cp |= ((unsigned char)s[i++] & 0x3F) << 12;
    cp |= ((unsigned char)s[i++] & 0x3F) << 6;
    cp |= (unsigned char)s[i++] & 0x3F;
    return cp;
}

// Упрощённая ширина:
//  - ASCII/кириллица: 1
//  - emoji/символы: 2
//  - combining: 0
static int CpWidth(uint32_t cp) {
    if ((cp >= 0x0300 && cp <= 0x036F) || (cp >= 0x1AB0 && cp <= 0x1AFF) ||
        (cp >= 0x1DC0 && cp <= 0x1DFF) || (cp >= 0x20D0 && cp <= 0x20FF) ||
        (cp >= 0xFE20 && cp <= 0xFE2F)) return 0;

    if ((cp >= 0x1F300 && cp <= 0x1FAFF) || (cp >= 0x2600 && cp <= 0x27BF))
        return 2;

    if ((cp >= 0x1100 && cp <= 0x11FF) || (cp >= 0x2E80 && cp <= 0xA4CF) ||
        (cp >= 0xAC00 && cp <= 0xD7AF) || (cp >= 0xF900 && cp <= 0xFAFF) ||
        (cp >= 0xFE10 && cp <= 0xFE19) || (cp >= 0xFF01 && cp <= 0xFF60) ||
        (cp >= 0xFFE0 && cp <= 0xFFE6))
        return 2;

    return 1;
}

static int DisplayWidth(const std::string& s) {
    int w = 0;
    for (size_t i = 0; i < s.size(); ) {
        uint32_t cp = NextCp(s, i);
        w += CpWidth(cp);
    }
    return w;
}

static std::string SanitizeCell(std::string s) {
    for (char& ch : s) {
        if (ch == '|' || ch == '\n' || ch == '\r' || ch == '\t') ch = ' ';
    }
    return s;
}

static std::string PadRight(const std::string& s, int target_width) {
    std::string t = SanitizeCell(s);
    int w = DisplayWidth(t);
    if (w >= target_width) return t;
    return t + std::string(target_width - w, ' ');
}

void PrintOwnershipTable(const std::string& title, const std::vector<OwnershipRow>& rows) {
    std::cout << "\n📊 " << title << "\n";

    const std::string h1 = "Texture";
    const std::string h2 = "Владельцы";
    const std::string h3 = "use_count";
    const std::string h4 = "Комментарий";

    int w1 = DisplayWidth(h1);
    int w2 = DisplayWidth(h2);
    int w3 = DisplayWidth(h3);
    int w4 = DisplayWidth(h4);

    for (const auto& r : rows) {
        w1 = std::max(w1, DisplayWidth(SanitizeCell(r.tex_name)));
        w2 = std::max(w2, DisplayWidth(SanitizeCell(r.owners)));
        w3 = std::max(w3, (int)std::to_string(r.use_count).size());
        w4 = std::max(w4, DisplayWidth(SanitizeCell(r.note)));
    }

    // небольшой запас
    w1 += 2; w2 += 2; w3 += 2; w4 += 2;

    auto line = [&]() {
        std::cout << "+"
                  << std::string(w1 + 2, '-') << "+"
                  << std::string(w2 + 2, '-') << "+"
                  << std::string(w3 + 2, '-') << "+"
                  << std::string(w4 + 2, '-') << "+\n";
    };

    line();
    std::cout << "| " << PadRight(h1, w1) << " | "
              << PadRight(h2, w2) << " | "
              << PadRight(h3, w3) << " | "
              << PadRight(h4, w4) << " |\n";
    line();

    for (const auto& r : rows) {
        std::cout << "| " << PadRight(" " + r.tex_name + "   ", w1) << " | "
                  << PadRight(r.owners,  w2) << " | "
                  << PadRight(std::to_string(r.use_count), w3) << " | "
                  << PadRight(r.note, w4) << " |\n";
    }
    line();
}
