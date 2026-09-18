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

        int created = 0;

        // Тест: создаём 12 обычных блоков
        for (int i = 0; i < 12; i++) {
            float x = 200.f + i * 60.f;
            float y = 120.f + (i % 3) * 30.f;

            auto object = editor->createObject(
                1,
                {x, y},
                false
            );

            if (object) {
                created++;
            }
        }

        log::info("Auto Decor: created {} objects", created);

        FLAlertLayer::create(
            "Auto Decor",
            fmt::format("Created {} objects!", created).c_str(),
            "OK"
        )->show();
    }
};