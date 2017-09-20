#include <functional>
#include <string>
#include "oxygine-framework.h"
#include "SlotsMachine.h"
#include "EventsController.h"
#include "ClipRectActor.h"
#include "DataManager.h"
#include "Utils.h"

using namespace oxygine;

//it is our resources
//in real project you would have more than one Resources declarations.
//It is important on mobile devices with limited memory and you would load/unload them
Resources gameResources;

const float ui_scale = 0.7f;

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
        TextStyle style = TextStyle(gameResources.getResFont("main")).withColor(Color::White).alignMiddle();
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
            
        });
        button_spin->addEventListener(TouchEvent::TOUCH_UP, [=](Event * e)
        {
            button_spin->setResAnim(gameResources.getResAnim("spin_disabled"));
            _slots_machine->spin();
            button_spin->setTouchEnabled(false);
            resetTotalWin();
            
        });
        EventsController::instance().subscribeToEvent("event.spin_end", [=]()
        {
            button_spin->setTouchEnabled(true);
            button_spin->setResAnim(gameResources.getResAnim("spin"));
            _total_win_text->setText(Utils::to_string(_slots_machine->getTotalWin()));
            coins_text->setText(Utils::to_string(DataManager::instance().getCoinsCount()));
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
        style = TextStyle(gameResources.getResFont("main")).withColor(Color::White).alignMiddle();
        _total_win_text->setStyle(style);
        resetTotalWin();
        UpdateCallback callback_update = CLOSURE(this, &MainActor::update);
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
        
                
        
    }
    
    void resetTotalWin()
    {
        _total_win_text->setText("0");
    }

    void update(const UpdateState& us)
    {
        _slots_machine->update(us.dt);
    }
    
    void runSprite()
    {
        spSprite sprite = new Sprite();
        addChild(sprite);

        int duration = 600;//ms
        int loops = -1;//infinity loops

        //animation has 8 columns - frames, check 'res.xml'
        ResAnim* animation = gameResources.getResAnim("anim");

        //add animation tween to sprite
        //TweenAnim would change animation frames
        sprite->addTween(Sprite::TweenAnim(animation), duration, loops);

        Vector2 destPos = getStage()->getSize() - sprite->getSize();
        Vector2 srcPos = Vector2(0, destPos.y);
        //set sprite initial position
        sprite->setPosition(srcPos);

        //add another tween: TweenQueue
        //TweenQueue is a collection of tweens
        spTweenQueue tweenQueue = new TweenQueue();
        tweenQueue->setDelay(1500);
        //first, move sprite to dest position
        tweenQueue->add(Sprite::TweenPosition(destPos), 2500, 1);
        //then fade it out smoothly
        tweenQueue->add(Sprite::TweenAlpha(0), 500, 1);

        sprite->addTween(tweenQueue);

        //and remove sprite from tree when tweenQueue is empty
        //if you don't hold any references to sprite it would be deleted automatically
        tweenQueue->detachWhenDone();
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
