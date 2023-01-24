#include "includes.h"
#include <filesystem>
#include "ini.h"

namespace fs = std::filesystem;

const string configName = "MenuAnimation.ini";

bool loadedBG = false;
int texturesLoaded = 0;
bool loadedBGSprites = false;
bool loadedOtherAssets = false;
bool fromR = false;

float animSpeed;
bool useAntiAlias;

CCSprite* getBGSprite() {
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    auto frames = CCArray::create();

    auto size = CCTextureCache::sharedTextureCache()->textureForKey("bganim\\0.png")->getContentSize();

    auto spriteCache = CCTextureCache::sharedTextureCache();
    for (size_t i = 0; i < framesCount(); i++) {
        string spriteName = string("bganim\\") + std::to_string(i) + string(".png");

        auto tex = spriteCache->textureForKey(spriteName.c_str());
        if (useAntiAlias) tex->setAntiAliasTexParameters();
        else tex->setAliasTexParameters();

        frames->addObject(CCSpriteFrame::createWithTexture(tex, {0, 0, size.width, size.height}));
    }

    auto action = CCRepeatForever::create(
        CCAnimate::create(
            CCAnimation::createWithSpriteFrames(
                frames,
                1.0f / animSpeed
            )
        )
    );

    auto bg = CCSprite::createWithTexture(CCTextureCache::sharedTextureCache()->textureForKey("bganim\\0.png"));
    bg->runAction(action);
    auto bgSize = bg->getContentSize();
    bg->setScale((winSize.width > winSize.height) ? (winSize.width / bgSize.width) : (winSize.height / bgSize.height));
    bg->setAnchorPoint({0, 0});
    
    return bg;
}

class a {
public:
    void callback(CCObject* pObj) {
        texturesLoaded++;

        if (texturesLoaded == framesCount()) {
            loadedBGSprites = true;
            if (loadedOtherAssets) {
                auto scene = MenuLayer::scene(fromR);
                CCDirector::sharedDirector()->pushScene(CCTransitionFade::create(.5f, scene));
            }
        }
    }
};

bool (__thiscall* LoadingLayer_init_o)(CCLayer* self, bool fromReload);
bool __fastcall LoadingLayer_init_hk(CCLayer* self,  void*, bool fromReload) {
    loadedBG = false;
    texturesLoaded = 0;
    fromR = fromReload;
    loadedBGSprites = false;
    loadedOtherAssets = false;

    if (!fs::exists(configName)) {
        std::ofstream outfile(configName);

        outfile << "[General]\nAnimationSpeed = 15 // FPS of the animation basically (bigger => faster)\nUseAntiAlisaing = 1 // Use anti aliasing (smoothing the texture instead of a pixelart-like look). 0 or 1.";

        outfile.close();
    }

    ini_t* config = ini_load(configName.c_str());

    animSpeed = atof(ini_get(config, "General", "AnimationSpeed"));
    useAntiAlias = atoi(ini_get(config, "General", "UseAntiAlisaing"));

    ini_free(config);

    return LoadingLayer_init_o(self, fromReload);
}

void (__thiscall* loadAssets_o)(CCLayer* self);
void __fastcall loadAssets_hk(CCLayer* self, void*) {
    if (!loadedBG) {
        for (size_t i = 0; i < framesCount(); i++) {
            string spriteName = string("bganim\\") + std::to_string(i) + string(".png");

            CCTextureCache::sharedTextureCache()->addImageAsync(
                CCFileUtils::sharedFileUtils()->fullPathForFilename(spriteName.c_str(), false).c_str(),
                self,
                callfuncO_selector(a::callback)
            );
        }

        loadedBG = true;
    }

    loadAssets_o(self);
}

bool (__thiscall* MenuLayer_init_o)(CCLayer* self);
bool __fastcall MenuLayer_init_hk(CCLayer* self, void*) {
    if (!MenuLayer_init_o(self)) return false;

    dynamic_cast<CCNode*>(self->getChildren()->objectAtIndex(0))->removeFromParent();

    self->addChild(getBGSprite(), -1);

    return true;
}

void (__thiscall* loadingFinished_o)(void* self);
void __fastcall loadingFinished_hk(void* self, void*) {
    loadedOtherAssets = true;
    if (loadedBGSprites) {
        auto scene = MenuLayer::scene(fromR);
        CCDirector::sharedDirector()->pushScene(CCTransitionFade::create(.5f, scene));
    }
}

inline CCSprite* (__cdecl* CCSprite_create_o)(const char* name);
CCSprite* CCSprite_create_hk(const char* name) {
    if (string(name) == "GJ_gradientBG.png")
        return getBGSprite();

    return CCSprite_create_o(name);
}

DWORD WINAPI thread_func(void* hModule) {
#ifdef INDEV
    AllocConsole();
    freopen_s(reinterpret_cast<FILE**>(stdout), "CONOUT$", "w", stdout);
    SetConsoleOutputCP(65001); // UTF-8 outputting
#endif // INDEV

    if (MH_Initialize() != MH_OK) {
        log << "Failed to initialize MinHook!";
        return 0;
    }

    HOOK(0x18C080, LoadingLayer_init_hk, LoadingLayer_init_o);
    HOOK(0x18C8E0, loadAssets_hk, loadAssets_o);
    HOOK(0x18C790, loadingFinished_hk, loadingFinished_o);
    HOOK(0x1907B0, MenuLayer_init_hk, MenuLayer_init_o);

    CC_HOOK("?create@CCSprite@cocos2d@@SAPAV12@PBD@Z", CCSprite_create_hk, CCSprite_create_o);

    MH_EnableHook(MH_ALL_HOOKS);

    return 0;
}

BOOL APIENTRY DllMain(HMODULE handle, DWORD reason, LPVOID reserved) {
    if (reason == DLL_PROCESS_ATTACH) {
        auto h = CreateThread(0, 0, thread_func, handle, 0, 0);
        if (h)
            CloseHandle(h);
        else
            return FALSE;
    }
    return TRUE;
}