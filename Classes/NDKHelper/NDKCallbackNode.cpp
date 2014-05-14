//
//  NDKCallbackNode.cpp
//  EasyNDK-for-cocos2dx
//
//  Created by Amir Ali Jiwani on 23/02/2013.
//	Rewritten by Naël MSKINE on 12/05/2014.
//
//

#include "NDKCallbackNode.h"

namespace easyndk {

NDKCallbackNode* NDKCallbackNode::create( const string &groupName, const string &name, Ref *target, SEL_EasyNDKFunc sel, bool cleanupAfterCall )
{
	auto ret = new NDKCallbackNode();
	if (ret && ret->init(groupName, name, target, sel, cleanupAfterCall) ) {
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

NDKCallbackNode* NDKCallbackNode::create( const string &groupName, const string &name, const std::function<void (Ref *)> &func, bool cleanupAfterCall )
{
	auto ret = new NDKCallbackNode();
	if (ret && ret->init(groupName, name, func, cleanupAfterCall) ) {
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

bool NDKCallbackNode::init( const string &groupName, const string &name, Ref *target, SEL_EasyNDKFunc sel, bool cleanupAfterCall )
{
	CC_SAFE_RETAIN(target);
	this->_target = target;
	this->_selector = sel;
	this->_function = nullptr;
	this->_groupName = groupName;
	this->_name = name;
	this->_cleanupAfterCall = cleanupAfterCall;

	return true;
}

bool NDKCallbackNode::init(const string &groupName, const string &name, const std::function<void (Ref *)> &func, bool cleanupAfterCall)
{
	this->_target = nullptr;
	this->_selector = nullptr;
	this->_function = func;
	this->_groupName = groupName;
	this->_name = name;
	this->_cleanupAfterCall = cleanupAfterCall;

	return true;
}

NDKCallbackNode::~NDKCallbackNode()
{
	CC_SAFE_RELEASE(_target);
}

string NDKCallbackNode::getName()
{
	return this->_name;
}

string NDKCallbackNode::getGroup()
{
	return this->_groupName;
}

void NDKCallbackNode::executeCallfunc(Ref *param)
{
	if (_target == nullptr) {
		(_target->*_selector)(param);
	}
	else if (_function) {
		_function(param);
	}
}

bool NDKCallbackNode::shouldCleanupAfterCall()
{
	return _cleanupAfterCall;
}

}