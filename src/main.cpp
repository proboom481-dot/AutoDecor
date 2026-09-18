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

        // Запоминаем количество ДО создания новых объектов.
        // Иначе мы начнём обрабатывать собственный декор повторно.
        unsigned int originalCount = objects->count();

        int decorated = 0;

        for (unsigned int i = 0; i < originalCount; i++) {
            auto object = static_cast<GameObject*>(
                objects->objectAtIndex(i)
            );

            if (!object)
                continue;

            // Пока декорируем только обычный блок ID 1.
            if (object->m_objectID != 1)
                continue;

            auto pos = object->getPosition();

            // Маленький декоративный блок сверху справа.
            auto decor = editor->createObject(
                1,
                {
                    pos.x + 14.f,
                    pos.y + 14.f
                },
                false
            );

            if (!decor)
                continue;

            decor->setScale(0.35f);
            decor->setRotation(45.f);

            // Голубоватый декоративный цвет.
            decor->setObjectColor(
                {80, 180, 255}
            );

            decorated++;
        }

        log::info(
            "Auto Decor: decorated {} blocks!",
            decorated
        );

        FLAlertLayer::create(
            "AUTO DECOR",
            fmt::format(
                "Decorated {} blocks!",
                decorated
            ).c_str(),
            "OK"
        )->show();
    }
};