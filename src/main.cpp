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
                "No objects found!",
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

        // Находим границы существующего layout
        for (unsigned int i = 0; i < originalCount; i++) {

            auto object = static_cast<GameObject*>(
                objects->objectAtIndex(i)
            );

            if (!object)
                continue;

            auto pos = object->getPosition();

            if (pos.x < minX)
                minX = pos.x;

            if (pos.x > maxX)
                maxX = pos.x;

            if (pos.y < minY)
                minY = pos.y;

            if (pos.y > maxY)
                maxY = pos.y;

            if (object->m_objectID == 1)
                blockCount++;
        }

        if (blockCount == 0) {
            FLAlertLayer::create(
                "AUTO DECOR",
                "No normal blocks found!",
                "OK"
            )->show();
            return;
        }

        int decorated = 0;
        int flowers = 0;
        int clouds = 0;

        // ================================
        // ТРАВА И ЦВЕТЫ
        // ================================

        int blockIndex = 0;

        for (unsigned int i = 0; i < originalCount; i++) {

            auto block = static_cast<GameObject*>(
                objects->objectAtIndex(i)
            );

            if (!block)
                continue;

            // Только обычные блоки
            if (block->m_objectID != 1)
                continue;

            auto pos = block->getPosition();

            // Только нижняя часть уровня
            if (pos.y > minY + 110.f)
                continue;

            // Трава на каждом втором блоке
            if (blockIndex % 2 == 0) {

                auto grassLeft = editor->createObject(
                    907,
                    {pos.x - 17.f, pos.y + 17.f},
                    false
                );

                if (grassLeft) {
                    grassLeft->setScale(
                        (blockIndex % 4 == 0)
                            ? 0.55f
                            : 0.70f
                    );

                    grassLeft->setRotation(
                        (blockIndex % 4 == 0)
                            ? -8.f
                            : 0.f
                    );

                    decorated++;
                }

                auto grassRight = editor->createObject(
                    907,
                    {pos.x + 17.f, pos.y + 17.f},
                    false
                );

                if (grassRight) {
                    grassRight->setScale(
                        (blockIndex % 4 == 0)
                            ? 0.60f
                            : 0.72f
                    );

                    grassRight->setRotation(
                        (blockIndex % 3 == 0)
                            ? 165.f
                            : 180.f
                    );

                    decorated++;
                }
            }

            // Цветок на каждом третьем блоке
            if (blockIndex % 3 == 0) {

                auto flower = editor->createObject(
                    939,
                    {pos.x, pos.y + 30.f},
                    false
                );

                if (flower) {

                    flower->setScale(
                        (blockIndex % 6 == 0)
                            ? 0.42f
                            : 0.55f
                    );

                    flower->setRotation(
                        (blockIndex % 2 == 0)
                            ? -5.f
                            : 5.f
                    );

                    flowers++;
                    decorated++;
                }
            }

            blockIndex++;
        }

        // ================================
        // ОБЛАКА
        // ================================

        float cloudY = maxY + 100.f;
        float cloudSpacing = 190.f;

        int cloudIndex = 0;

        for (
            float x = minX - 150.f;
            x <= maxX + 150.f;
            x += cloudSpacing
        ) {

            auto cloud = editor->createObject(
                936,
                {x, cloudY},
                false
            );

            if (!cloud)
                continue;

            if (cloudIndex % 4 == 0) {
                cloud->setScale(0.75f);
            }
            else if (cloudIndex % 3 == 0) {
                cloud->setScale(0.55f);
            }
            else {
                cloud->setScale(0.65f);
            }

            if (cloudIndex % 2 == 0) {
                cloud->setRotation(-3.f);
            }
            else {
                cloud->setRotation(3.f);
            }

            clouds++;
            decorated++;
            cloudIndex++;
        }

        g_autoDecorDone = true;

        log::info(
            "Auto Decor: created {} decorations!",
            decorated
        );

        FLAlertLayer::create(
            "AUTO DECOR",
            fmt::format(
                "Created {} decorations!\n\nFlowers: {}\nClouds: {}",
                decorated,
                flowers,
                clouds
            ).c_str(),
            "OK"
        )->show();
    }
};