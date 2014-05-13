//
//  NDKCallbackNode.h
//  EasyNDK-for-cocos2dx
//
//  Created by Amir Ali Jiwani on 23/02/2013.
//	Rewritten by Naël MSKINE on 12/05/2014.
//
//

#ifndef __NDK_CALLBACK_NODE_H__
#define __NDK_CALLBACK_NODE_H__

#include "cocos2d.h"
#include <string>
#include "NDKHelperDef.h"

USING_NS_CC;
using namespace std;

namespace easyndk {

class NDKCallbackNode : public Ref
{
private :
	string _name;
	string _groupName;
	Ref *_target;
	SEL_EasyNDKFunc _selector;
	std::function<void(Ref*)> _function;

public :
	static NDKCallbackNode* create(const string &groupName, const string &name, Ref *target, SEL_EasyNDKFunc sel);
	static NDKCallbackNode* create(const string &groupName, const string &name, const std::function<void (Ref *)> &func);
	virtual bool init(const string &groupName, const string &name, Ref *target, SEL_EasyNDKFunc sel);
	virtual bool init(const string &groupName, const string &name, const std::function<void (Ref *)> &func);
	virtual ~NDKCallbackNode();
	string getName();
	string getGroup();
	void executeCallfunc(Ref *param);
};

}

#endif // __NDK_CALLBACK_NODE_H__
