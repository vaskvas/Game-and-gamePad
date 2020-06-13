#include "cocos2d.h"

class mainGUi : public cocos2d::Menu
{
public:
    static cocos2d::Menu* createGUI(bool allGUI = false);
    virtual bool init();
    void setGui(bool allGui) { isAllGUI = allGui; }
    void changeSoundCallback(cocos2d::Ref* pSender);
    void menuBackCallback(cocos2d::Ref* pSender);
    void menuCloseCallback(cocos2d::Ref* pSender);
    CREATE_FUNC(mainGUi);
private:
    bool isAllGUI = false;
    bool isEnabledSound = true;
};
