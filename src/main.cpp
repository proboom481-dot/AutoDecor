#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>

using namespace geode::prelude;

static bool g_autoDecorDone = false;

class $modify(AutoDecorEditorUI, EditorUI) {

    bool init(LevelEditorLayer* editorLayer) {
        if (!EditorUI::init(editorLayer))
            return false;

        g_autoDecorDone = false;

        auto menu = CCMenu::create();
        menu->setPosition(0, 0);

        auto buttonSprite = ButtonSprite::create("AUTO DECOR");

        auto button = CCMenuItemSpriteExtra::create(
            buttonSprite,
            this,
            menu_selector(AutoDecorEditorUI::onAutoDecor)
        );

        button->setPosition(100, 100);

        menu->addChild(button);
        this->addChild(menu);

        return true;
    }

    void onAutoDecor(CCObject* sender) {

        if (g_autoDecorDone) {
            FLAlertLayer::create(
                "AUTO DECOR",
                "Decoration has already been added!",
                "OK"
            )->show();

            return;
        }

        auto editor = LevelEditorLayer::get();

        if (!editor) {
            FLAlertLayer::create(
                "AUTO DECOR",
                "Editor not found!",
                "OK"
            )->show();

            return;
        }

        auto objects = editor->getAllObjects();

        if (!objects || objects->count() == 0) {
            FLAlertLayer::create(
                "AUTO DECOR",
                "No objects found.",
                "OK"
            )->show();

            return;
        }

        unsigned int originalCount = objects->count();

        float minX = 999999.f;
        float maxX = -999999.f;
        float minY = 999999.f;
        float maxY = -999999.f;

        int blockCount = 0;

        // ---------------------------------------------
        // Ищем размеры существующего layout
        // ---------------------------------------------

        for (unsigned int i = 0; i < originalCount; i++) {

            auto object = static_cast<GameObject*>(
                objects->objectAtIndex(i)
            );

            if (!object)
                continue;

            auto pos = object->getPosition();

            minX = std::min(minX, pos.x);
            maxX = std::max(maxX, pos.x);
            minY = std::min(minY, pos.y);
            maxY = std::max(maxY, pos.y);

            if (object->m_objectID == 1)
                blockCount++;
        }

        if (blockCount == 0) {
            FLAlertLayer::create(
                "AUTO DECOR",
                "No normal blocks found.",
                "OK"
            )->show();

            return;
        }

        int decorated = 0;
        int flowers = 0;
        int clouds = 0;

        // ---------------------------------------------
        // 🌿 НИЖНЯЯ РАСТИТЕЛЬНОСТЬ
        // ---------------------------------------------

        int blockIndex = 0;

        for (unsigned int i = 0; i < originalCount; i++) {

            auto block = static_cast<GameObject*>(
                objects->objectAtIndex(i)
            );

            if (!block)
                continue;

            if (block->m_objectID != 1)
                continue;

            auto pos = block->getPosition();

            /*
             * Только нижний слой.
             * Верхние платформы не будут покрываться травой.
             */
            if (pos.y > minY + 120.f)
                continue;

            // Каждая вторая подходящая поверхность
            if (blockIndex % 2 == 0) {

                auto grassLeft = editor->createObject(
                    907,
                    {pos.x - 16.f, pos.y + 17.f},
                    false
                );

                if (grassLeft) {

                    float scale =
                        (blockIndex % 4 == 0)
                        ? 0.55f
                        : 0.68f;

                    grassLeft->setScale(scale);

                    if (blockIndex % 5 == 0)
                        grassLeft->setRotation(-10.f);

                    decorated++;
                }

                auto grassRight = editor->createObject(
                    907,
                    {pos.x + 16.f, pos.y + 17.f},
                    false
                );

                if (grassRight) {

                    float scale =
                        (blockIndex % 6 == 0)
                        ? 0.58f
                        : 0.70f;

                    grassRight->setScale(scale);

                    if (blockIndex % 3 == 0)
                        grassRight->setRotation(170.f);
                    else
                        grassRight->setRotation(180.f);

                    decorated++;
                }
            }

            // -----------------------------------------
            // 🌸 Цветы
            // -----------------------------------------

            /*
             * Цветок не на каждом блоке.
             * Это делает композицию менее шаблонной.
             */
            if (blockIndex % 4 == 0) {

                auto flower = editor->createObject(
                    939,
                    {pos.x, pos.y + 29.f},
                    false
                );

                if (flower) {

                    if (blockIndex % 8 == 0)
                        flower->setScale(0.42f);
                    else if (blockIndex % 6 == 0)
                        flower->setScale(0.50f);
                    else
                        flower->setScale(0.57f);

                    flower->setRotation(
                        (blockIndex % 2 == 0)
                        ? -6.f
                        : 6.f
                    );

                    flowers++;
                    decorated++;
                }
            }

            blockIndex++;
        }

        // ---------------------------------------------
        // ☁️ ВЕРХНИЙ СЛОЙ ОБЛАКОВ
        // ---------------------------------------------

        float cloudBaseY = maxY + 90.f;

        /*
         * Первый слой
         */
        float spacing1 = 220.f;

        int cloudIndex = 0;

        for (
            float x = minX - 220.f;
            x <= maxX + 220.f;
            x += spacing1
        ) {

            auto cloud = editor->createObject(
                936,
                {x, cloudBaseY},
                false
            );

            if (!cloud)
                continue;

            if (cloudIndex % 3 == 0)
                cloud->setScale(0.72f);
            else if (cloudIndex % 2 == 0)
                cloud->setScale(0.60f);
            else
                cloud->setScale(0.50f);

            cloud->setRotation(
                (cloudIndex % 2 == 0)
                ? -3.f
                : 3.f
            );

            clouds++;
            decorated++;
            cloudIndex++;
        }

        // ---------------------------------------------
        // ☁️ ВТОРОЙ, БОЛЕЕ ДАЛЬНИЙ СЛОЙ
        // ---------------------------------------------

        float spacing2 = 310.f;

        int cloudIndex2 = 0;

        for (
            float x = minX - 100.f;
            x <= maxX + 100.f;
            x += spacing2
        ) {

            auto cloud = editor->createObject(
                936,
                {x, cloudBaseY + 75.f},
                false
            );

            if (!cloud)
                continue;

            if (cloudIndex2 % 2 == 0)
                cloud->setScale(0.42f);
            else
                cloud->setScale(0.50f);

            cloud->setRotation(
                (cloudIndex2 % 2 == 0)
                ? 4.f
                : -4.f
            );

            clouds++;
            decorated++;
            cloudIndex2++;
        }

        // ---------------------------------------------
        // ГОТОВО
        // ---------------------------------------------

        g_autoDecorDone = true;

        log::info(
            "Auto Decor v1.2: {} decorations created!",
            decorated
        );

        FLAlertLayer::create(
            "AUTO DECOR",
            fmt::format(
                "Decorated!\n\n"
                "Total: {}\n"
                "Flowers: {}\n"
                "Clouds: {}",
                decorated,
                flowers,
                clouds
            ).c_str(),
            "OK"
        )->show();
    }
};