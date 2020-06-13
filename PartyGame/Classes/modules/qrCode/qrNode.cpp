#include "qrNode.h"
USING_NS_CC;

DrawNode* qrNode::createQrNode(std::string aQrText)
{
	qrNode* ret = new (std::nothrow) qrNode();
	if (ret)
	{
		ret->setText(aQrText);
		if (ret->init()) {
			ret->autorelease();
			return ret;
		}
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

bool qrNode::init()
{
    if (!DrawNode::init())
        return false;

	bool bRet = QRCoder.EncodeData(0, 0, 1, -1, (char*)qrText.c_str());
	if (bRet)
		drawQrCode();
	else 
		return false;

    return true;
}

void qrNode::drawQrCode() {
	int nSize = 5;
	int originalSize = QRCoder.m_nSymbleSize + (QR_MARGIN * 2);

	Point pt[6];
	Color4F color;

	pt[0] = Point(0, 0);
	pt[1] = Point((QRCoder.m_nSymbleSize + QR_MARGIN * 2) * nSize, (QRCoder.m_nSymbleSize + QR_MARGIN * 2) * nSize);
	pt[2] = Point((QRCoder.m_nSymbleSize + QR_MARGIN * 2) * nSize, 0);

	pt[3] = pt[0];
	pt[4] = Point(0, (QRCoder.m_nSymbleSize + QR_MARGIN * 2) * nSize);
	pt[5] = pt[1];
	color = Color4F(1, 1, 1, 1);
	drawPolygon(pt, 6, color, 0, color);

	for (int i = 0; i < QRCoder.m_nSymbleSize; ++i)
	{
		for (int j = 0; j < QRCoder.m_nSymbleSize; ++j)
		{
			pt[0] = Point((i + QR_MARGIN) * nSize, (j + QR_MARGIN) * nSize);
			pt[1] = Point(((i + QR_MARGIN) + 1) * nSize, ((j + QR_MARGIN) + 1) * nSize);
			pt[2] = Point(((i + QR_MARGIN) + 1) * nSize, ((j + QR_MARGIN) + 0) * nSize);

			pt[3] = pt[0];
			pt[4] = Point(((i + QR_MARGIN) + 0) * nSize, ((j + QR_MARGIN) + 1) * nSize);
			pt[5] = pt[1];
			if (QRCoder.m_byModuleData[i][j] == 1)
			{
				color = Color4F(0, 0, 0, 1);
			}
			else
			{
				color = Color4F(1, 1, 1, 1);
			}
			drawPolygon(pt, 6, color, 0, color);
		}
	}
	setContentSize(Size(Point((nSize * QRCoder.m_nSymbleSize) * 1.5f, (nSize * QRCoder.m_nSymbleSize) * 1.5f)));
	setScaleX(1.5f);
	setScaleY(-1.5f);
}
