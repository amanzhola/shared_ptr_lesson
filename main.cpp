// ============================================================================
// 📘 УРОК-ДЕМО: std::shared_ptr (Robot/Charger == Shape/Texture 1:1)
// Разбит на файлы: main.cpp = сценарий шагов, остальное = модули
//
// ПРАВКИ ДЛЯ ЯСНОСТИ (CANVAS-часть):
//  1) убран '\n' из имени 🔷 CANVAS-Square (чтобы лог не "ломался" строками)
//  2) добавлены явные заголовки/разделители перед SetTexture() для 🔷 и 🟣
//  3) (опционально полезно) показываем текущий use_count canvasHouse до/после
// ============================================================================

#include "types.h"
#include "texture.h"
#include "shape.h"
#include "canvas.h"
#include "textures_factory.h"
#include "dashed_border.h"
#include "utf8_table.h"
#include "lesson_print.h"
#include "console_color.h"

#include <iostream>
#include <memory>
#include <algorithm>
#include <cmath>

#ifdef _WIN32
  #include <windows.h>
#endif

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    std::ios::sync_with_stdio(false);

    PrintHeader("📘 shared_ptr УРОК: Robot/Charger == Shape/Texture (1:1)");
    PrintPixelLegend();

    // ─────────────────────────────────────────────────────────────
    // ШАГ 1
    // ─────────────────────────────────────────────────────────────
    std::cout << "🔹 ШАГ 1: Создаём фигуры (долгоживущие владельцы)\n";
    std::cout << "🎯 ОЖИДАЕМ: фигуры переживут внутренний блок.\n";
    PrintLine();

    Shape square("🔷 Shape#1 (square)", ShapeType::RECTANGLE);
    Shape ellipse("🟣 Shape#2 (ellipse)", ShapeType::ELLIPSE);

    std::cout << "✅ ПОЛУЧИЛИ: фигуры созданы.\n";
    PrintLine();

    // ─────────────────────────────────────────────────────────────
    // ШАГ 2-3 (внутренний блок)
    // ─────────────────────────────────────────────────────────────
    std::cout << "\n🔹 ШАГ 2: ВНУТРЕННИЙ БЛОК: создаём HouseScene/GreenFill/BlueUnused\n";
    std::cout << "🎯 ОЖИДАЕМ:\n";
    std::cout << "   - BlueUnusedTexture 🟦 умрёт при выходе из блока (никому не передана)\n";
    std::cout << "   - 🖼️ HouseSceneTexture 🌤️🏠 / GreenFillTexture 🟩 НЕ умрут, потому что их будут держать фигуры\n";
    PrintLine();

    {
        // 1) 🖼️ Сцена: небо + солнце + дом + трава
        auto houseScene = std::make_shared<Texture>(
            "🖼️ HouseSceneTexture 🌤️🏠 [содержит и небо, и солнце, и дом, и траву]",
            MakeHouseImage()
        );

        // 2) ✅ ДОКАЗАТЕЛЬСТВО “зелёное — правда”
        auto greenFillImg = MakeSolidImage(40, 20, 'G');
        std::cout << "✅ Проверка GreenFillTexture 🟩: sample[0][0]='" << greenFillImg[0][0]
                  << "' (должно быть 'G')\n";

        auto greenFill = std::make_shared<Texture>(
            "GreenFillTexture 🟩 [заливка: каждый пиксель = 'G']",
            std::move(greenFillImg)
        );

        // 3) 🟦 Неиспользуемая синяя — специально, чтобы умерла в конце блока
        auto blueUnused = std::make_shared<Texture>(
            "BlueUnusedTexture 🟦 [не используется, должна умереть после блока]",
            MakeSolidImage(40, 20, 'B')
        );

        PrintOwnershipTable("После make_shared (ШАГ 2)", {
            {"🖼️ HouseSceneTexture 🌤️🏠", "локальная houseScene", (int)houseScene.use_count(), "только локальная"},
            {"GreenFillTexture 🟩",       "локальная greenFill",  (int)greenFill.use_count(),  "только локальная"},
            {"BlueUnusedTexture 🟦",      "локальная blueUnused", (int)blueUnused.use_count(), "будет уничтожена"}
        });

        std::cout << "\n🔹 ШАГ 3: SetTexture (как UseCharger)\n";
        std::cout << "🎯 ОЖИДАЕМ:\n";
        std::cout << "   - use_count HouseScene/GreenFill станет 2 (локальная + фигура)\n";
        std::cout << "   - BlueUnused останется 1\n";
        PrintLine();

        // квадрат получает сцену
        square.SetTexture(houseScene);

        // овал получает зелёную заливку
        ellipse.SetTexture(greenFill);

        std::cout << "📌 ПОЯСНЕНИЕ: сейчас use_count=2 потому что:\n";
        std::cout << "   (1) локальная переменная (houseScene/greenFill) владеет текстурой\n";
        std::cout << "   (2) поле Shape::texture_ внутри фигуры тоже владеет той же текстурой\n";

        PrintOwnershipTable("После SetTexture (ШАГ 3)", {
            {"🖼️ HouseSceneTexture 🌤️🏠", "локальная + 🔷 square", (int)houseScene.use_count(), "используется квадратом"},
            {"GreenFillTexture 🟩",       "локальная + 🟣 ellipse",(int)greenFill.use_count(),  "используется овалом"},
            {"BlueUnusedTexture 🟦",      "локальная blueUnused", (int)blueUnused.use_count(), "всё ещё локальная"}
        });

        PrintLine();
        std::cout << "---- (выходим из внутреннего блока) ----\n";
    }

    // ─────────────────────────────────────────────────────────────
    // После выхода из блока
    // ─────────────────────────────────────────────────────────────
    PrintLine();
    std::cout << "✅ ПОЛУЧИЛИ: BlueUnusedTexture 🟦 умерла (у неё был 1 владелец в блоке)\n";

    PrintOwnershipTable("После выхода из блока", {
        {"🖼️ HouseSceneTexture 🌤️🏠", "🔷 square",  square.GetTexture()  ? (int)square.GetTexture().use_count()  : 0, "жива"},
        {"GreenFillTexture 🟩",       "🟣 ellipse", ellipse.GetTexture() ? (int)ellipse.GetTexture().use_count() : 0, "жива"},
        {"BlueUnusedTexture 🟦",      "—", 0, "уничтожена"}
    });

    // ─────────────────────────────────────────────────────────────
    // ШАГ 5
    // ─────────────────────────────────────────────────────────────
    std::cout << "\n🔹 ШАГ 5: 🟣 ellipse.SetTexture(square.GetTexture())\n";
    std::cout << "🎯 ОЖИДАЕМ:\n";
    std::cout << "   - GreenFillTexture 🟩 умрёт (если больше никто не держит)\n";
    std::cout << "   - 🖼️ HouseSceneTexture 🌤️🏠 станет общей у 🔷 и 🟣\n";
    PrintLine();

    ellipse.SetTexture(square.GetTexture());

    std::cout << "📌 ПОЯСНЕНИЕ: здесь use_count снова стал 2, потому что:\n";
    std::cout << "   (1) 🔷 square держит HouseSceneTexture\n";
    std::cout << "   (2) 🟣 ellipse теперь тоже держит HouseSceneTexture\n";
    std::cout << "   А GreenFillTexture 🟩 умерла, потому что у неё не осталось владельцев.\n";

    PrintOwnershipTable("После ellipse.SetTexture(square.GetTexture())", {
        {"🖼️ HouseSceneTexture 🌤️🏠", "🔷 square + 🟣 ellipse", square.GetTexture() ? (int)square.GetTexture().use_count() : 0, "совместное владение"},
        {"GreenFillTexture 🟩",       "—", 0, "уничтожена"}
    });

    // ─────────────────────────────────────────────────────────────
    // ШАГ 6
    // ─────────────────────────────────────────────────────────────
    std::cout << "\n🔹 ШАГ 6: 🔷 square.ResetTexture()\n";
    std::cout << "🎯 ОЖИДАЕМ: 🖼️ HouseSceneTexture 🌤️🏠 НЕ умрёт, потому что её держит 🟣.\n";
    PrintLine();

    square.ResetTexture();
    std::cout << "📌 ПОЯСНЕНИЕ: после ResetTexture() у 🔷 square больше нет владения.\n";
    std::cout << "   HouseSceneTexture остаётся жить, потому что 🟣 ellipse всё ещё владелец (use_count=1).\n";

    PrintOwnershipTable("После square.ResetTexture()", {
        {"🖼️ HouseSceneTexture 🌤️🏠", "🟣 ellipse", ellipse.GetTexture() ? (int)ellipse.GetTexture().use_count() : 0, "всё ещё жива"}
    });

    // ─────────────────────────────────────────────────────────────
    // ШАГ 7
    // ─────────────────────────────────────────────────────────────
    std::cout << "\n🔹 ШАГ 7: 🟣 ellipse.ResetTexture() (последний владелец)\n";
    std::cout << "🎯 ОЖИДАЕМ: 🖼️ HouseSceneTexture 🌤️🏠 умрёт здесь.\n";
    PrintLine();

    ellipse.ResetTexture();
    std::cout << "📌 ПОЯСНЕНИЕ: после ResetTexture() у 🟣 ellipse владельцев не осталось.\n";
    std::cout << "   use_count стал 0 => shared_ptr удалил объект => 💥 dtor Texture.\n";

    PrintOwnershipTable("После ellipse.ResetTexture()", {
        {"🖼️ HouseSceneTexture 🌤️🏠", "—", 0, "уничтожена"}
    });

    // ─────────────────────────────────────────────────────────────
    // ЛЕГЕНДА (по-русски)
    // ─────────────────────────────────────────────────────────────
    std::cout << "\n🧾 ЛЕГЕНДА (что означает каждая текстура):\n";
    std::cout << "  🖼️ HouseSceneTexture 🌤️🏠  = картинка-сцена (небо + солнце + дом + трава)\n";
    std::cout << "  GreenFillTexture 🟩        = зелёная заливка: символ 'G' на каждом пикселе\n";
    std::cout << "  BlueUnusedTexture 🟦       = синяя текстура, создана специально, чтобы умереть при выходе из блока\n";

    // =========================================================================
    // ГРАФИКА: CANVAS + TEXTURE SOURCE (красные пунктиры)
    // =========================================================================
    PrintHeader("🎨 ГРАФИКА (подтверждение) — квадрат+овал + SOURCE (красные пунктиры)");
    std::cout << "📌 ВАЖНО: графика НЕ управляет временем жизни.\n";
    std::cout << "   Сейчас создадим отдельную текстуру 🖼️ CanvasHouseScene 🎨 и нарисуем 2 фигуры.\n";
    PrintLine();

    auto canvasHouse = std::make_shared<Texture>(
        // ❗️убрал \n в конце имени, чтобы вывод был ровным и не путал
        "🖼️ CanvasHouseScene 🎨 [квадрат+овал на канвасе + source с красными пунктирами]",
        MakeHouseImage()
    );
    Size tsz = canvasHouse->GetSize();

    const Size  rect_size   {34, 20};
    const Point rect_origin {0, 0};

    const Size  ell_size    {42, 18};
    const Point ell_origin  {4, 0};

    const Point rect_pos{0, 22};
    const int   ell_y = 6;

    const int corner_x = rect_pos.x + rect_size.width - 1;
    const int corner_y = rect_pos.y;

    const double a = ell_size.width  / 2.0;
    const double b = ell_size.height / 2.0;
    const double py = (corner_y - ell_y) + 0.5;

    int ell_x = 0;
    if (py >= 0.0 && py <= ell_size.height) {
        const double yn = (py / b) - 1.0;
        if (yn * yn <= 1.0) {
            const double xn = -std::sqrt(1.0 - yn * yn);
            const double px = (xn + 1.0) * a - 0.5;
            ell_x = (int)std::ceil(corner_x - px + 1.0);
        }
    }

    const int MARGIN_R = 2, MARGIN_B = 2;
    int canvas_w = std::max(rect_pos.x + rect_size.width,  ell_x + ell_size.width)  + MARGIN_R;
    int canvas_h = std::max(rect_pos.y + rect_size.height, ell_y + ell_size.height) + MARGIN_B;

    Canvas canvas({canvas_w, canvas_h});

    // Небольшой helper только для CANVAS-части: быстро показать use_count
    auto PrintCanvasHousePtr = [&](const char* where) {
        std::cout << "   🔎 [" << where << "] canvasHouse.get()=" << canvasHouse.get()
                  << " use_count=" << canvasHouse.use_count() << "\n";
    };

    // --- 1) КВАДРАТ ---
    auto s1 = std::make_unique<Shape>("🔷 CANVAS-Square", ShapeType::RECTANGLE);
    s1->SetPosition(rect_pos);
    s1->SetSize(rect_size);
    s1->SetTextureOrigin(rect_origin);

    PrintLine();
    std::cout << "🧩 CANVAS: 🔷 square.SetTexture(canvasHouse)\n";
    std::cout << "🎯 ОЖИДАЕМ: внутри SetTexture() use_count временно станет 2 (копия параметра)\n";
    PrintCanvasHousePtr("до square.SetTexture");
    PrintLine();

    s1->SetTexture(canvasHouse);

    PrintLine();
    PrintCanvasHousePtr("после square.SetTexture");
    std::cout << "✅ ПОЛУЧИЛИ: теперь владельцы = canvasHouse + 🔷 square (use_count должен быть 2)\n";
    PrintLine();

    canvas.AddShape(std::move(s1));

    // --- 2) ОВАЛ ---
    auto s2 = std::make_unique<Shape>("🟣 CANVAS-Ellipse", ShapeType::ELLIPSE);
    s2->SetPosition({ell_x, ell_y});
    s2->SetSize(ell_size);
    s2->SetTextureOrigin(ell_origin);

    PrintLine();
    std::cout << "🧩 CANVAS: 🟣 ellipse.SetTexture(canvasHouse)\n";
	std::cout << "🎯 ОЖИДАЕМ: в строке [ВНУТРИ ФУНКЦИИ] (в SetTexture) use_count станет 3\n";
	std::cout << "           потому что параметр tex — КОПИЯ shared_ptr (передача по значению)\n";
	std::cout << "           (владельцы уже: canvasHouse + 🔷 square, плюс копия параметра)\n";
	PrintCanvasHousePtr("до ellipse.SetTexture");
	
	std::cout << "📌 ДО вызова (в main) use_count=2 — это нормально;\n";
	std::cout << "   значение 3 видно в строке [ВНУТРИ ФУНКЦИИ] внутри SetTexture()\n";
	
    PrintLine();

    s2->SetTexture(canvasHouse);

    PrintLine();
    PrintCanvasHousePtr("после ellipse.SetTexture");
    std::cout << "✅ ПОЛУЧИЛИ: теперь владельцы = canvasHouse + 🔷 square + 🟣 ellipse (use_count должен быть 3)\n";
    PrintLine();

    canvas.AddShape(std::move(s2));
    
    std::cout << "📌 Сейчас у canvasHouse владельцев: " << canvasHouse.use_count()
          << " (canvasHouse + 🔷 square + 🟣 ellipse)\n";
	std::cout << "📌 Адрес объекта Texture (canvasHouse.get()) = " << canvasHouse.get() << "\n";

    std::cout << "\n=== CANVAS ===\n";
    canvas.PrintColored();

    // ---- TEXTURE SOURCE с красными пунктирными контурами ----
    PrintHeader("🔴 TEXTURE SOURCE (откуда взяты 🔷 прямоугольник и 🟣 овал)");

    const int PAD_L = 4, PAD_T = 2, PAD_R = 14, PAD_B = 6;
    Size view_sz{tsz.width + PAD_L + PAD_R, tsz.height + PAD_T + PAD_B};

    Image view(view_sz.height, std::string(view_sz.width, ' '));
    for (int y = 0; y < tsz.height; ++y)
        for (int x = 0; x < tsz.width; ++x)
            view[y + PAD_T][x + PAD_L] = canvasHouse->GetPixelColor({x, y});

    DrawRectBorderDashed(view, {rect_origin.x + PAD_L, rect_origin.y + PAD_T}, rect_size, 'R');
    DrawEllipseBorderDashed(view, {ell_origin.x + PAD_L, ell_origin.y + PAD_T}, ell_size, 'R');

    std::cout << "\n=== TEXTURE (with margins) + RED MARKS ===\n";
    PrintColoredImage(view);

    std::cout << "\n✅ END: когда canvasHouse и фигуры канваса выйдут из области видимости -> CanvasHouseScene 🎨 уничтожится автоматически.\n";
    return 0;
}
