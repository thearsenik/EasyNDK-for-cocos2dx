//
//  NDKHelper.h
//  EasyNDK-for-cocos2dx
//
//  Created by Amir Ali Jiwani on 23/02/2013.
//	Rewritten by Naël MSKINE on 12/05/2014.
//
//

#ifndef __NDK_HELPER_H__
#define __NDK_HELPER_H__

#include <iostream>
#include "cocos2d.h"
#include <string>
#include <vector>
#include "jansson.h"
#include "NDKHelperDef.h"
#include "NDKCallbackNode.h"
#include "NDKCalledSelector.h"

USING_NS_CC;
using namespace std;

namespace easyndk {

class NDKHelper : public Ref
{
public:
	static NDKHelper *getInstance();
	static void destroyInstance();

	static Ref* getRefFromJson(json_t *obj);
	static json_t* getJsonFromRef(Ref* obj);

	void addSelector(const string &groupName, const string &name, Ref* target, SEL_EasyNDKFunc selector);
	void removeSelectorsInGroup(const string &groupName);
	void printSelectorList();
	void handleMessage(json_t *methodName, json_t* methodParams);
	void sendMessageWithParams(const string &methodName, Ref* methodParams);

protected:
	CREATE_FUNC(NDKHelper);
	virtual bool init();
	virtual ~NDKHelper();
	void removeAtIndex(int index);

protected:
	static NDKHelper *_instance;
	Vector<NDKCallbackNode*> registeredSelectors;
	Vector<NDKCalledSelector*> calledSelectors;
};
}

#endif // __NDK_HELPER_H__
