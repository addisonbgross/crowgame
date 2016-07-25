#include "SpriteGenerator.hpp"

SpriteGenerator::SpriteGenerator(ModelFactory *modelFactory, LightFactory *lightFactory)
{
    mf = modelFactory;
    lf = lightFactory;

    // Models
    mf->setObjSource( OsType::osConvert("../res/obj/") );
    mf->addObj("crow");
    mf->addObj("squirrel");
    mf->addObj("tree");
    mf->addObj("house_2");
    mf->addObj("house_3");
    mf->addObj("house_5");
    mf->addObj("house_6");
    mf->addObj("corner_road");
    mf->addObj("road_1");
    mf->addObj("store_doublesquare");
    mf->addObj("store_corner");
    mf->addObj("marker-orange");
    mf->addObj("cube");
    mf->addObj("garbage_empty");
    mf->addObj("garbage_full");
    mf->addObj("garbage_bag");
    mf->addObj("crow_goods_marker");
    mf->addObj("squirrel_goods_marker");
    mf->addObj("combat_marker");
    mf->addObj("car_green");
    mf->addObj("car_purple");
    mf->addObj("car_red");
    mf->addObj("car_yellow");

    // trash
    mf->setObjSource( OsType::osConvert("../res/obj/trash/") );
    mf->addObj("applecore");
    mf->addObj("chickenleg");
    mf->addObj("fishbone");
    mf->addObj("milkcarton");
    mf->addObj("tunacan");

    // landscape
    mf->setObjSource( OsType::osConvert("../res/landscape/") );
    mf->addObj("sun");
    mf->addObj("moon");
    mf->addObj("land");
    mf->addObj("cloud");

    // Animations
    mf->setObjSource( OsType::osConvert( "../res/animations/crow/" ) );
    mf->addObj("crow_0");
    mf->addObj("crow_1");
    mf->addObj("crow_2");
    mf->addObj("crow_3");
    mf->addObj("crow_4");
    mf->addObj("crow_5");
    mf->setObjSource( OsType::osConvert( "../res/animations/crow_def/" ) );
    mf->addObj("crow_def_0");
    mf->addObj("crow_def_1");
    mf->addObj("crow_def_2");
    mf->addObj("crow_def_3");
    mf->addObj("crow_def_4");
    mf->addObj("crow_def_5");
    mf->setObjSource( OsType::osConvert( "../res/animations/crowAction/" ) );
    mf->addObj("crowAction_0");
    mf->addObj("crowAction_1");
    mf->addObj("crowAction_2");
    mf->addObj("crowAction_3");
    mf->addObj("crowAction_4");
    mf->addObj("crowAction_5");
    mf->setObjSource( OsType::osConvert( "../res/animations/squirrel/" ) );
    mf->addObj("squirrel_0");
    mf->addObj("squirrel_1");
    mf->addObj("squirrel_2");
    mf->addObj("squirrel_3");
    mf->addObj("squirrel_4");
    mf->addObj("squirrel_5");
    mf->setObjSource( OsType::osConvert( "../res/animations/selector/" ) );
    mf->addObj("selector_0");
    mf->addObj("selector_1");
    mf->addObj("selector_2");
    mf->addObj("selector_3");
    mf->addObj("selector_4");
    mf->addObj("selector_5");
    mf->setObjSource( OsType::osConvert( "../res/animations/click/" ) );
    mf->addObj("click_0");
    mf->addObj("click_1");
    mf->addObj("click_2");
    mf->addObj("click_3");
    mf->addObj("click_4");
    mf->addObj("click_5");
    mf->setObjSource( OsType::osConvert( "../res/animations/leaves/" ) );
    mf->addObj("leaves_0");
    mf->addObj("leaves_1");
    mf->addObj("leaves_2");
    mf->addObj("leaves_3");
    mf->addObj("leaves_4");
    mf->addObj("leaves_5");

    // interface
    mf->setObjSource( OsType::osConvert( "../res/obj/interface/" ) );
    mf->addObj("crowCoin");
    mf->addObj("squirrelCoin");
    mf->addObj("foodCoin");
    mf->addObj("hoverer");
    mf->addObj("crowButton");
    mf->addObj("threeCrowButton");
    mf->addObj("crowVictory");
    mf->addObj("squirrelVictory");

    // lights
    mf->setObjSource( OsType::osConvert( "../res/obj/light/" ) );
    mf->addObj("lightball");

    // text/numbers
    mf->setObjSource( OsType::osConvert( "../res/obj/alphanumeric/" ) );
    mf->addObj("0");
    mf->addObj("1");
    mf->addObj("2");
    mf->addObj("3");
    mf->addObj("4");
    mf->addObj("5");
    mf->addObj("6");
    mf->addObj("7");
    mf->addObj("8");
    mf->addObj("9");
    mf->addObj("A");
    mf->addObj("C");
    mf->addObj("R");
    mf->addObj("O");
    mf->addObj("W");
    mf->addObj("G");
    mf->addObj("M");
    mf->addObj("E");
}

void SpriteGenerator::killAll()
{
    for (int i = 0; i < sprites.size(); ++i) {
        sprites.at(i)->alive = false;
    }

    sanitation();
}

// memory management
bool SpriteGenerator::recycleSprite(int position)
{
    sprites.at(position)->kill( mf ) ;
    sprites.erase(sprites.begin() + position);
    sprites.shrink_to_fit();
}

void SpriteGenerator::sanitation()
{
    for (int i = 0; i < sprites.size(); ++i) {
        if (sprites.at(i)->isDead()) {
            recycleSprite(i);
        }
    }
}

// Model
Sprite * SpriteGenerator::createSpriteModel(std::string name)
{
    Model *m = mf->makeModel(0.0f, 0.0f, 0.0f, name);
    m->name = name;
    models.push_back(m);

    Sprite *s = new Sprite(m);
    sprites.push_back(s);

    return s;
}

Sprite * SpriteGenerator::createSpriteModel(float x, float y, float z, std::string name)
{
    Model *m = mf->makeModel(x, y, z, name);
    m->name = name;
    models.push_back(m);

    Sprite *s = new Sprite(m);
    sprites.push_back(s);

    return s;
}

// Animation
Sprite * SpriteGenerator::createSpriteAnimation(std::string name)
{
    Animation *a = mf->makeAnimation(0.0f, 0.0f, 0.0f, name);
    animations.push_back(a);

    Sprite *s = new Sprite(a);
    sprites.push_back(s);

    return s;
}

Sprite * SpriteGenerator::createSpriteAnimation(std::string name, std::string mod)
{
    Animation *a = mf->makeAnimation(0.0f, 0.0f, 0.0f, name);
    animations.push_back(a);

    if (mod == "once") {
        a->runOnce();
    } else if (mod == "pingpong") {
        a->setPingPong(true);
    } else {
        std::cout << ">> Sprite Mod Not Found! -> " << name << " : \'" << mod << " <<" << std::endl;
    }

    Sprite *s = new Sprite(a);
    sprites.push_back(s);

    return s;
}

Sprite * SpriteGenerator::createSpriteAnimation(float x, float y, float z, std::string name)
{
    Animation *a = mf->makeAnimation(x, y, z, name);
    animations.push_back(a);

    Sprite *s = new Sprite(a);
    sprites.push_back(s);

    return s;
}

Sprite * SpriteGenerator::createSpriteAnimation(float x, float y, float z, std::string name, std::string mod)
{
    Animation *a = mf->makeAnimation(x, y, z, name);
    animations.push_back(a);

    if (mod == "once") {
        a->runOnce();
    } else if (mod == "pingpong") {
        a->setPingPong(true);
    } else {
        std::cout << ">> Sprite Mod Not Found! -> " << name << " : \'" << mod << "\' @ " << x << "," << y << "," << z << " <<" << std::endl;
    }

    Sprite *s = new Sprite(a);
    sprites.push_back(s);

    return s;
}

AdvancedSprite * SpriteGenerator::createCrow(float x, float y, float z)
{
    Animation *a = mf->makeAnimation(x, y, z, "crow");
    a->setPingPong(true);
    animations.push_back(a);

    Sprite *goods = createSpriteModel( x, y + 2, z, "garbage_bag" );
    goods->setHighlight( true );
    goods->hide();
    sprites.push_back(goods);

    AdvancedSprite *c = new AdvancedSprite( a, goods );
    c->setScale(0.1);
    c->setRate(0.8);
    sprites.push_back( c );

    Sprite *goodsMarker = createSpriteModel( x, y, z, "crow_goods_marker" );
    goodsMarker->setScale(2.5f);
    goodsMarker->setHighlight(true);
    goodsMarker->hide();
    c->goodsMarker = goodsMarker;
    sprites.push_back( goodsMarker );

    return c;
}

AdvancedSprite * SpriteGenerator::createDefensiveCrow(float x, float y, float z)
{
    Animation *a = mf->makeAnimation(x, y, z, "crow_def");
    a->setPingPong(true);
    a->setRate(0.35f);
    animations.push_back(a);

    Sprite *goods = createSpriteModel( x, y + 2, z, "garbage_bag" );
    goods->setHighlight( true );
    goods->hide();
    sprites.push_back(goods);

    AdvancedSprite *c = new AdvancedSprite( a, goods );
    c->setScale(0.1);
    c->setRate(0.8);
    sprites.push_back( c );

    Sprite *goodsMarker = createSpriteModel( x, y, z, "crow_goods_marker" );
    goodsMarker->setScale(2.5f);
    goodsMarker->setHighlight(true);
    goodsMarker->hide();
    c->goodsMarker = goodsMarker;
    sprites.push_back( goodsMarker );

    return c;
}

AdvancedSprite * SpriteGenerator::createSquirrel(float x, float y, float z)
{
    Animation *a = mf->makeAnimation(x, y, z, "squirrel");
    animations.push_back(a);

    Sprite *goods = createSpriteModel( x, y + 2, z, "garbage_bag" );
    goods->setHighlight( true );
    goods->hide();
    sprites.push_back(goods);

    AdvancedSprite *s = new AdvancedSprite( a, goods );
    s->setScale(0.03);
    s->setRate(0.8);
    sprites.push_back( s );

    Sprite *goodsMarker = createSpriteModel( x, y, z, "squirrel_goods_marker" );
    goodsMarker->setScale(2.5f);
    goodsMarker->setHighlight(true);
    goodsMarker->hide();
    s->goodsMarker = goodsMarker;
    sprites.push_back( goodsMarker );

    return s;
}

void SpriteGenerator::moveAll()
{
    int numSprites = getSprites()->size();
    std::vector<Sprite *> * sprites = getSprites();

    for (int i = 0; i < numSprites; ++i) {
        Sprite *sp = sprites->at(i);
        if (&sp->path) {
            if (sp->isMoving) {
                AdvancedSprite *asp = dynamic_cast<AdvancedSprite *>( sp );

                if (sp->path.size() > 0) {
                    sp->move();

                    if ( asp ) {
                        asp->advancedMove();
                    }
                } else {
                    sp->isMoving = false;

                    // has arrived at destination
                    if ( asp ) {
                        asp->arrivalAction();
                    }
                }
            }
        }
    }
}

void SpriteGenerator::addPointLight(float x, float y, float z, bool isOn)
{
    Light *lt = lf->makePointLight( x, y, z );
    lt->isOn = isOn;
}

void SpriteGenerator::addPointLight(Sprite *sp, bool isOn)
{
    Light *lt = lf->makePointLight( sp->rep->position.x,
                                    sp->rep->position.y,
                                    sp->rep->position.z );
    lt->isOn = isOn;
}

// get
ModelFactory * SpriteGenerator::getFactory()
{
    return mf;
}

std::vector<Sprite *> * SpriteGenerator::getSprites()
{
    return &sprites;
}
