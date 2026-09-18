
#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>

using namespace geode::prelude;


// ============================================================
// ExternalHack
// ============================================================

class ExternalHackMenu : public FLAlertLayer {
protected:

    CCMenu* m_categories = nullptr;
    CCMenu* m_features = nullptr;

    // Состояния функций
    bool m_noclip = false;
    bool m_hitboxes = false;
    bool m_hitboxTrail = false;
    bool m_trajectory = false;
    bool m_startpos = false;
    bool m_autoPractice = false;
    bool m_autoclicker = false;


    bool init() {

        if (!FLAlertLayer::init(
            nullptr,
            "",
            "",
            "",
            nullptr,
            540.f,
            false,
            360.f,
            0.75f
        )) {
            return false;
        }


        // ====================================================
        // TITLE
        // ====================================================

        auto title = CCLabelBMFont::create(
            "EXTERNALHACK",
            "goldFont.fnt"
        );

        title->setPosition(
            270.f,
            325.f
        );

        title->setScale(0.55f);

        m_mainLayer->addChild(
            title,
            20
        );


        // ====================================================
        // TOP ARROW
        // ====================================================

        auto topMenu = CCMenu::create();
        topMenu->setPosition(0, 0);

        m_mainLayer->addChild(
            topMenu,
            100
        );

        auto arrowSprite =
            CCSprite::createWithSpriteFrameName(
                "GJ_arrow01_001.png"
            );

        auto arrow = CCMenuItemSpriteExtra::create(
            arrowSprite,
            this,
            menu_selector(
                ExternalHackMenu::onBack
            )
        );

        arrow->setScale(0.48f);

        arrow->setPosition(
            270.f,
            344.f
        );

        topMenu->addChild(arrow);


        // ====================================================
        // EXIT BUTTON
        // ====================================================

        auto exitSprite = ButtonSprite::create(
            "EXIT",
            55,
            true,
            "bigFont.fnt",
            "GJ_button_06.png",
            18.f,
            0.45f
        );

        auto exit = CCMenuItemSpriteExtra::create(
            exitSprite,
            this,
            menu_selector(
                ExternalHackMenu::onExit
            )
        );

        exit->setPosition(
            485.f,
            25.f
        );

        topMenu->addChild(exit);


        // ====================================================
        // CATEGORY PANEL
        // ====================================================

        m_categories = CCMenu::create();
        m_categories->setPosition(0, 0);

        m_mainLayer->addChild(
            m_categories,
            10
        );


        createCategory(
            "LEVEL",
            0,
            78.f,
            292.f
        );

        createCategory(
            "UNIVERSAL",
            1,
            78.f,
            264.f
        );

        createCategory(
            "CREATOR",
            2,
            78.f,
            236.f
        );

        createCategory(
            "COSMETIC",
            3,
            78.f,
            208.f
        );

        createCategory(
            "SPEEDHACK",
            4,
            78.f,
            180.f
        );

        createCategory(
            "ICON EFFECTS",
            5,
            78.f,
            152.f
        );

        createCategory(
            "LABELS",
            6,
            78.f,
            124.f
        );

        createCategory(
            "SHORTCUTS",
            7,
            78.f,
            96.f
        );

        createCategory(
            "CONFIG",
            8,
            78.f,
            68.f
        );

        createCategory(
            "SEARCH",
            9,
            78.f,
            40.f
        );


        // ====================================================
        // FEATURES
        // ====================================================

        m_features = CCMenu::create();
        m_features->setPosition(0, 0);

        m_mainLayer->addChild(
            m_features,
            10
        );

        showLevel();

        return true;
    }


    // ========================================================
    // CATEGORY
    // ========================================================

    void createCategory(
        const char* name,
        int id,
        float x,
        float y
    ) {

        auto sprite = ButtonSprite::create(
            name,
            115,
            true,
            "bigFont.fnt",
            "GJ_button_04.png",
            20.f,
            0.42f
        );

        auto button = CCMenuItemSpriteExtra::create(
            sprite,
            this,
            menu_selector(
                ExternalHackMenu::onCategory
            )
        );

        button->setTag(id);

        button->setPosition(
            x,
            y
        );

        m_categories->addChild(button);
    }


    // ========================================================
    // FEATURE
    // ========================================================

    void createFeature(
        const char* name,
        int id,
        float x,
        float y,
        bool value = false
    ) {

        // Checkbox

        auto off =
            CCSprite::createWithSpriteFrameName(
                "GJ_checkOff_001.png"
            );

        auto on =
            CCSprite::createWithSpriteFrameName(
                "GJ_checkOn_001.png"
            );

        auto toggle = CCMenuItemToggler::create(
            off,
            on,
            this,
            menu_selector(
                ExternalHackMenu::onToggle
            )
        );

        toggle->setTag(id);
        toggle->setScale(0.42f);

        toggle->toggle(value);

        toggle->setPosition(
            x,
            y
        );

        m_features->addChild(toggle);


        // Text

        auto label = CCLabelBMFont::create(
            name,
            "bigFont.fnt"
        );

        label->setAnchorPoint(
            {0.f, 0.5f}
        );

        label->setScale(0.30f);

        label->setPosition(
            x + 13.f,
            y
        );

        m_features->addChild(label);


        // Plus

        auto plusSprite = ButtonSprite::create(
            "+",
            20,
            true,
            "bigFont.fnt",
            "GJ_button_01.png",
            15.f,
            0.52f
        );

        auto plus = CCMenuItemSpriteExtra::create(
            plusSprite,
            this,
            menu_selector(
                ExternalHackMenu::onPlus
            )
        );

        plus->setTag(id);

        plus->setPosition(
            x + 137.f,
            y
        );

        m_features->addChild(plus);


        // Info

        auto infoSprite =
            CCSprite::createWithSpriteFrameName(
                "GJ_infoIcon_001.png"
            );

        auto info = CCMenuItemSpriteExtra::create(
            infoSprite,
            this,
            menu_selector(
                ExternalHackMenu::onInfo
            )
        );

        info->setTag(id);
        info->setScale(0.40f);

        info->setPosition(
            x + 159.f,
            y
        );

        m_features->addChild(info);
    }


    // ========================================================
    // LEVEL
    // ========================================================

    void showLevel() {

        const float left = 205.f;
        const float right = 365.f;

        const float top = 285.f;
        const float gap = 29.f;


        createFeature(
            "NOCLIP",
            1,
            left,
            top,
            m_noclip
        );

        createFeature(
            "SHOW HITBOXES",
            2,
            left,
            top - gap,
            m_hitboxes
        );

        createFeature(
            "HITBOX TRAIL",
            3,
            left,
            top - gap * 2,
            m_hitboxTrail
        );

        createFeature(
            "SHOW TRAJECTORY",
            4,
            left,
            top - gap * 3,
            m_trajectory
        );

        createFeature(
            "ACCURATE HITBOXES",
            5,
            left,
            top - gap * 4
        );

        createFeature(
            "AUTO COLLECT COINS",
            6,
            left,
            top - gap * 5
        );

        createFeature(
            "AUTOCLICKER",
            7,
            left,
            top - gap * 6,
            m_autoclicker
        );

        createFeature(
            "CLASSIC PERCENTAGE",
            8,
            left,
            top - gap * 7
        );


        createFeature(
            "INSTANT COMPLETE",
            10,
            right,
            top
        );

        createFeature(
            "HITBOXES ON DEATH",
            11,
            right,
            top - gap
        );

        createFeature(
            "STARTPOS SWITCHER",
            12,
            right,
            top - gap * 2,
            m_startpos
        );

        createFeature(
            "ACCURATE PERCENTAGE",
            13,
            right,
            top - gap * 3
        );

        createFeature(
            "PLATFORMER MODE",
            14,
            right,
            top - gap * 4
        );

        createFeature(
            "AUTO PRACTICE",
            15,
            right,
            top - gap * 5,
            m_autoPractice
        );

        createFeature(
            "BEST PERCENTAGE",
            16,
            right,
            top - gap * 6
        );

        createFeature(
            "COIN TRACERS",
            17,
            right,
            top - gap * 7
        );
    }


    // ========================================================
    // CATEGORY CLICK
    // ========================================================

    void onCategory(CCObject* sender) {

        auto button =
            static_cast<CCMenuItemSpriteExtra*>(
                sender
            );

        int id = button->getTag();

        m_features->removeAllChildren();


        if (id == 0) {

            showLevel();
            return;
        }


        const char* title = "CATEGORY";

        switch (id) {

            case 1:
                title = "UNIVERSAL";
                break;

            case 2:
                title = "CREATOR";
                break;

            case 3:
                title = "COSMETIC";
                break;

            case 4:
                title = "SPEEDHACK";
                break;

            case 5:
                title = "ICON EFFECTS";
                break;

            case 6:
                title = "LABELS";
                break;

            case 7:
                title = "SHORTCUTS";
                break;

            case 8:
                title = "CONFIG";
                break;

            case 9:
                title = "SEARCH";
                break;
        }


        auto label = CCLabelBMFont::create(
            title,
            "bigFont.fnt"
        );

        label->setPosition(
            365.f,
            190.f
        );

        label->setScale(0.55f);

        m_features->addChild(label);


        auto coming = CCLabelBMFont::create(
            "FEATURES WILL BE ADDED HERE",
            "bigFont.fnt"
        );

        coming->setPosition(
            365.f,
            160.f
        );

        coming->setScale(0.30f);

        m_features->addChild(coming);
    }


    // ========================================================
    // TOGGLE
    // ========================================================

    void onToggle(CCObject* sender) {

        auto toggle =
            static_cast<CCMenuItemToggler*>(
                sender
            );

        bool enabled = toggle->isToggled();

        switch (toggle->getTag()) {

            case 1:
                m_noclip = enabled;
                break;

            case 2:
                m_hitboxes = enabled;
                break;

            case 3:
                m_hitboxTrail = enabled;
                break;

            case 4:
                m_trajectory = enabled;
                break;

            case 12:
                m_startpos = enabled;
                break;

            case 15:
                m_autoPractice = enabled;
                break;

            case 7:
                m_autoclicker = enabled;
                break;
        }


        log::info(
            "ExternalHack feature {} = {}",
            toggle->getTag(),
            enabled
        );
    }


    // ========================================================
    // PLUS
    // ========================================================

    void onPlus(CCObject* sender) {

        auto button =
            static_cast<CCMenuItemSpriteExtra*>(
                sender
            );

        FLAlertLayer::create(
            "ExternalHack",
            fmt::format(
                "Settings for feature {}",
                button->getTag()
            ).c_str(),
            "OK"
        )->show();
    }


    // ========================================================
    // INFO
    // ========================================================

    void onInfo(CCObject* sender) {

        auto button =
            static_cast<CCMenuItemSpriteExtra*>(
                sender
            );

        FLAlertLayer::create(
            "ExternalHack",
            fmt::format(
                "ExternalHack feature ID: {}",
                button->getTag()
            ).c_str(),
            "OK"
        )->show();
    }


    // ========================================================
    // ARROW
    // ========================================================

    void onBack(CCObject*) {
        this->keyBackClicked();
    }


    // ========================================================
    // EXIT
    // ========================================================

    void onExit(CCObject*) {
        this->keyBackClicked();
    }


public:

    static ExternalHackMenu* create() {

        auto ret = new ExternalHackMenu();

        if (ret && ret->init()) {
            ret->autorelease();
            return ret;
        }

        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};


// ============================================================
// PAUSE LAYER
// ============================================================

class $modify(
    ExternalHackPauseLayer,
    PauseLayer
) {

    void customSetup() {

        PauseLayer::customSetup();


        auto menu = CCMenu::create();
        menu->setPosition(0, 0);

        this->addChild(
            menu,
            100
        );


        auto button = CCMenuItemSpriteExtra::create(
            ButtonSprite::create(
                "EXTERNAL",
                80,
                true,
                "bigFont.fnt",
                "GJ_button_01.png",
                25.f,
                0.55f
            ),
            this,
            menu_selector(
                ExternalHackPauseLayer::openExternalHack
            )
        );


        auto size =
            CCDirector::sharedDirector()->getWinSize();


        button->setPosition(
            size.width - 75.f,
            45.f
        );


        menu->addChild(button);
    }


    void openExternalHack(CCObject*) {

        auto menu =
            ExternalHackMenu::create();

        if (menu)
            menu->show();
    }
};