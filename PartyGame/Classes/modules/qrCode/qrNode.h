#include "cocos2d.h"
#include "QR_Encode.h"

class qrNode : public cocos2d::DrawNode
{
public:
    static cocos2d::DrawNode* createQrNode(std::string aQrText);

    void setText(std::string text) { qrText = text; };
    virtual bool init();
private:
    std::string qrText;
	CQR_Encode QRCoder;
    
    void drawQrCode();
};
