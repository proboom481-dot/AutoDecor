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

        // Создаём настоящий GameObject
        auto object = GameObject::createWithKey(1);

        if (!object) {
            FLAlertLayer::create(
                "Auto Decor",
                "Failed to create GameObject.",
                "OK"
            )->show();
            return;
        }

        object->setPosition({300.f, 120.f});

        // Добавляем объект через UI-часть редактора.
        editor->addUIObject(object);

        log::info("Auto Decor: GameObject created!");

        FLAlertLayer::create(
            "Auto Decor",
            "GameObject created!",
            "OK"
        )->show();
    }
};