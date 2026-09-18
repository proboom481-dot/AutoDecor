#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>

using namespace geode::prelude;

class $modify(AutoDecorEditorUI, EditorUI) {

    bool init(LevelEditorLayer* editorLayer) {
        if (!EditorUI::init(editorLayer))
            return false;

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
        }

        int decorated = 0;
        int index = 0;

        // Декор существующих блоков
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

            // Трава только около нижней части
            if (pos.y <= minY + 110.f) {

                if (index % 2 == 0) {

                    auto grassLeft = editor->createObject(
                        907,
                        {pos.x - 17.f, pos.y + 17.f},
                        false
                    );

                    if (grassLeft) {
                        grassLeft->setScale(0.65f);
                        grassLeft->setRotation(-5.f);
                        decorated++;
                    }

                    auto grassRight = editor->createObject(
                        907,
                        {pos.x + 17.f, pos.y + 17.f},
                        false
                    );

                    if (grassRight) {
                        grassRight->setScale(0.65f);
                        grassRight->setRotation(180.f);
                        decorated++;
                    }
                }

                // Цветок через несколько блоков
                if (index % 3 == 0) {

                    auto flower = editor->createObject(
                        939,
                        {pos.x, pos.y + 30.f},
                        false
                    );

                    if (flower) {
                        flower->setScale(0.5f);
                        decorated++;
                    }
                }

                index++;
            }
        }

        // Облака сверху
        float cloudY = maxY + 100.f;

        int cloudIndex = 0;

        for (
            float x = minX - 100.f;
            x <= maxX + 100.f;
            x += 190.f
        ) {

            auto cloud = editor->createObject(
                936,
                {x, cloudY},
                false
            );

            if (!cloud)
                continue;

            cloud->setScale(
                cloudIndex % 2 == 0
                    ? 0.65f
                    : 0.5f
            );

            cloud->setRotation(
                cloudIndex % 2 == 0
                    ? -3.f
                    : 3.f
            );

            decorated++;
            cloudIndex++;
        }

        log::info(
            "Auto Decor: created {} decorations!",
            decorated
        );

        FLAlertLayer::create(
            "AUTO DECOR",
            fmt::format(
                "Created {} decorations!",
                decorated
            ).c_str(),
            "OK"
        )->show();
    }
};