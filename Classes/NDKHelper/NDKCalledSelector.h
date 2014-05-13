//
//  NDKCalledSelector.h
//  EasyNDK-for-cocos2dx
//
//	Created by Naël MSKINE on 12/05/2014.
//
//

#ifndef __NDK_CALLED_SELECTOR_H__
#define __NDK_CALLED_SELECTOR_H__

#include "cocos2d.h"
#include <string>

USING_NS_CC;
using namespace std;

namespace easyndk {
	class NDKCallbackNode;

class NDKCalledSelector : public Ref
{
private:
	NDKCallbackNode *_callback;
	Ref *_param;

public:
	static NDKCalledSelector* create(NDKCallbackNode *callback, Ref *param);
	virtual bool init(NDKCallbackNode *callback, Ref *param);
	virtual ~NDKCalledSelector();
	void executeCallback();
};

}

#endif // __NDK_CALLED_SELECTOR_H__
