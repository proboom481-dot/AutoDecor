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
        auto editor = GameManager::sharedState()->getEditorLayer();

        if (!editor) {
            log::error("Auto Decor: editor not found!");
            return;
        }

        // Создаём 12 базовых блоков
        for (int i = 0; i < 12; i++) {
            auto object = GameObject::createWithKey(1);

            if (!object) {
                log::error("Auto Decor: failed to create object!");
                continue;
            }

            float x = 200.f + i * 60.f;
            float y = 120.f + (i % 3) * 30.f;

            object->setPosition({x, y});

            editor->addObject(object);
        }

        log::info("Auto Decor: added 12 objects!");
    }
};