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
            log::error("Auto Decor: editor not found!");
            return;
        }

        auto objects = editor->getAllObjects();

        if (!objects) {
            log::error("Auto Decor: object list not found!");
            return;
        }

        unsigned int originalCount = objects->count();
        int decorated = 0;

        for (unsigned int i = 0; i < originalCount; i++) {
            auto block = static_cast<GameObject*>(
                objects->objectAtIndex(i)
            );

            if (!block)
                continue;

            // Пока декорируем обычные блоки.
            if (block->m_objectID != 1)
                continue;

            auto pos = block->getPosition();

            // 🌱 Трава слева
            auto grassLeft = editor->createObject(
                907,
                {pos.x - 18.f, pos.y + 18.f},
                false
            );

            if (grassLeft) {
                grassLeft->setScale(0.65f);
                decorated++;
            }

            // 🌱 Трава справа
            auto grassRight = editor->createObject(
                907,
                {pos.x + 18.f, pos.y + 18.f},
                false
            );

            if (grassRight) {
                grassRight->setScale(0.65f);
                grassRight->setRotation(180.f);
                decorated++;
            }

            // 🌸 Цветок сверху
            auto flower = editor->createObject(
                939,
                {pos.x, pos.y + 30.f},
                false
            );

            if (flower) {
                flower->setScale(0.55f);
                decorated++;
            }
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