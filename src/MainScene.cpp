#include <functional>
#include <string>
#include "oxygine-framework.h"
#include "oxygine-sound.h"
#include "SlotsMachine.h"
#include "EventsController.h"
#include "ClipRectActor.h"
#include "DataManager.h"
#include "Utils.h"
#include "TweenAnimRows.h"

using namespace oxygine;

//it is our resources
//in real project you would have more than one Resources declarations.
//It is important on mobile devices with limited memory and you would load/unload them
Resources gameResources;

extern SoundPlayer sfxPlayer;
extern SoundPlayer musicPlayer;

float ui_scale = 0.7f;

class MainActor: public Actor
{
    spSlotsMachine _slots_machine;
    spTextField _total_win_text;
public:

    MainActor()
    {
        Vector2 stage_size = getStage()->getSize();
        Vector2 center_pos = stage_size / 2;
        //create button Sprite
        spSprite background = new Sprite();
        //setup it:
        //set button.png image. Resource 'button' defined in 'res.xml'
        background->setResAnim(gameResources.getResAnim("background"));
        background->setAnchor(0.5f, 0.5f);
        //centered button at screen
        background->setPosition(center_pos);
        background->setScale(1.8);
        //attach button as child to current actor
        addChild(background);
     
        _slots_machine = new SlotsMachine(3, 5, {160, 160});
        addChild(_slots_machine);
        _slots_machine->setAnchor(0.5, 0.5);
        _slots_machine->setPosition(center_pos);
        
        spSprite coins_bg = new Sprite;
        coins_bg->setResAnim(gameResources.getResAnim("coins_bar_back"));
        coins_bg->setPosition(170, 14);
        coins_bg->setScale(ui_scale);
        addChild(coins_bg);
        
        spSprite coin_image = new Sprite;
        coin_image->setResAnim(gameResources.getResAnim("coin"));
        coin_image->setScale(ui_scale);
        coin_image->setAnchor(0.5f, 0);
        coin_image->setPosition(178, 6);
        addChild(coin_image);
        
        spTextField coins_text = new TextField();
        coins_text->attachTo(coins_bg);
        const Vector2& image_coins_bg_size = coins_bg->getSize();
        coins_text->setPosition(image_coins_bg_size.x / 2, image_coins_bg_size.y * 0.5f);
        coins_text->setFontSize(64);
        TextStyle style = TextStyle(gameResources.getResFont("myriad_pro_bold_condensed")).withColor(Color::White).alignMiddle();
        coins_text->setStyle(style);
        coins_text->setText(Utils::to_string(DataManager::instance().getCoinsCount()));
        
        EventsController::instance().subscribeToEvent("event.coins_changed", [=]()
        {
            coins_text->setText(Utils::to_string(DataManager::instance().getCoinsCount()));
        });
        
        spSprite button_spin = new Sprite();
        button_spin->setResAnim(gameResources.getResAnim("spin"));
        button_spin->addEventListener(TouchEvent::TOUCH_DOWN, [=](Event * e)
        {
            button_spin->setResAnim(gameResources.getResAnim("spin_pressed"));
            button_spin->setColor(Color::White);
        });
        button_spin->addEventListener(TouchEvent::TOUCH_UP, [=](Event * e)
        {
            button_spin->setResAnim(gameResources.getResAnim("spin_disabled"));
            _slots_machine->spin();
            button_spin->setTouchEnabled(false);
            resetTotalWin();
            sfxPlayer.play(gameResources.get("spin_button"));
        });
        EventsController::instance().subscribeToEvent("event.spin_end", [=]()
        {
            button_spin->setTouchEnabled(true);
            button_spin->setResAnim(gameResources.getResAnim("spin"));
            _total_win_text->setText(Utils::to_string(_slots_machine->getTotalWin()));
            coins_text->setText(Utils::to_string(DataManager::instance().getCoinsCount()));
            sfxPlayer.play(gameResources.get("reel_stop"));
        });
        EventsController::instance().subscribeToEvent("event.win", [=]()
        {
            sfxPlayer.play(gameResources.get("win"));
        });
        button_spin->setAnchor(1, 1);
        button_spin->setPosition(stage_size);
        button_spin->setScale(ui_scale);
        addChild(button_spin);
        
        spSprite image_total_win_bg = new Sprite();
        image_total_win_bg->setResAnim(gameResources.getResAnim("total_win_bar_back"));
        image_total_win_bg->setAnchor(1, 1);
        image_total_win_bg->setScale(ui_scale);
        image_total_win_bg->setPosition(stage_size.x - 400, stage_size.y);
        addChild(image_total_win_bg);

        _total_win_text = new TextField();
        //attach it as child to button
        _total_win_text->attachTo(image_total_win_bg);
        //centered in button
        const Vector2& image_total_win_bg_size = image_total_win_bg->getSize();
        _total_win_text->setPosition(image_total_win_bg_size.x / 2, image_total_win_bg_size.y * 0.54f);
        _total_win_text->setFontSize(64);
        style = TextStyle(gameResources.getResFont("myriad_pro_bold_condensed")).withColor(Color::White).alignMiddle();
        _total_win_text->setStyle(style);
        resetTotalWin();
        UpdateCallback callback_update = CLOSURE(this, &MainActor::_update);
        setCallbackDoUpdate(callback_update);
        
        spSprite bet_plus_button = new Sprite;
        bet_plus_button->setResAnim(gameResources.getResAnim("bet_plus"));
        bet_plus_button->setAnchor({1, 1});
        bet_plus_button->setScale(ui_scale);
        bet_plus_button->setPosition(stage_size.x - 780, stage_size.y);
        addChild(bet_plus_button);
        
        spSprite bet_bg = new Sprite;
        bet_bg->setResAnim(gameResources.getResAnim("bet_bar_back"));
        bet_bg->setAnchor({1, 1});
        bet_bg->setScale(ui_scale);
        bet_bg->setPosition(stage_size.x - 847, stage_size.y);
        addChild(bet_bg);
        
        spSprite bet_minus_button = new Sprite;
        bet_minus_button->setResAnim(gameResources.getResAnim("bet_minus"));
        bet_minus_button->setAnchor(1, 1);
        bet_minus_button->setScale(ui_scale);
        bet_minus_button->setPosition(stage_size.x - 1040, stage_size.y);
        addChild(bet_minus_button);
        
        musicPlayer.play(gameResources.get("bg_music"), PlayOptions().loop());
        
        spSprite info_button = new Sprite;
        info_button->setResAnim(gameResources.getResAnim("info"));
        info_button->setScale(ui_scale);
        info_button->setPosition(10, stage_size.y - 100);
        addChild(info_button);
        
        spSprite xp_bg = new Sprite;
        xp_bg->setResAnim(gameResources.getResAnim("xp_bar_back"));
        xp_bg->setPosition(870, 14);
        xp_bg->setScale(ui_scale);
        addChild(xp_bg);
        
        spSprite xp_image = new Sprite;
        xp_image->setResAnim(gameResources.getResAnim("xp"));
        xp_image->setScale(ui_scale);
        xp_image->setAnchor(0.5f, 0);
        xp_image->setPosition(870, 6);
        addChild(xp_image);
        
        spSprite image_lobby = new Sprite;
        image_lobby->setResAnim(gameResources.getResAnim("lobby"));
        image_lobby->setScale(ui_scale);
        image_lobby->setAnchor(0, 0);
        image_lobby->setPosition(10, 0);
        addChild(image_lobby);
    }
    
    void resetTotalWin()
    {
        _total_win_text->setText("0");
    }

    void _update(const UpdateState& us)
    {
        _slots_machine->_update(us.dt);
    }
};
//declare spMainActor as intrusive_ptr holder of MainActor
typedef oxygine::intrusive_ptr<MainActor> spMainActor;
//you could use DECLARE_SMART preprocessor definition it does the same:
//DECLARE_SMART(MainActor, spMainActor)

void main_scene_preinit() {}

//called from main.cpp
void main_scene_init()
{
    //load xml file with resources definition
    gameResources.loadXML("res.xml");


    //lets create our client code simple actor
    //spMainActor was defined above as smart intrusive pointer (read more: http://www.boost.org/doc/libs/1_60_0/libs/smart_ptr/intrusive_ptr.html)
    spMainActor actor = new MainActor;

    //and add it to Stage as child
    getStage()->addChild(actor);
}


//called each frame from main.cpp
void main_scene_update()
{
    
}

//called each frame from main.cpp
void main_scene_destroy()
{
    //free previously loaded resources
    gameResources.free();
}
