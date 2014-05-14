//
//  NDKHelperDef.h
//  EasyNDK-for-cocos2dx
//
//	Created by Naël MSKINE on 12/05/2014.
//
//

#ifndef __NDK_HELPER_DEF_H__
#define __NDK_HELPER_DEF_H__

#include "cocos2d.h"

USING_NS_CC;
using namespace std;

namespace easyndk {

typedef void (Ref::*SEL_EasyNDKFunc)(Ref*);
#define easyndkfunc_selector(_SELECTOR) static_cast<easyndk::SEL_EasyNDKFunc>(&_SELECTOR)

#if (!(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID && CC_TARGET_PLATFORM == CC_PLATFORM_IOS))
class NDKMock : public Ref
{
protected :
	string _name;
	std::function<void(Ref*)> _function;

public :
	inline static NDKMock* create(const string &name, const std::function<void (Ref *)> &func) {
		auto ret = new NDKMock();
		if (ret && ret->init(name, func) ) {
			ret->autorelease();
			return ret;
		}
		CC_SAFE_DELETE(ret);
		return nullptr;
	}

	inline virtual bool init(const string &name, const std::function<void (Ref *)> &func) {
		_name = name;
		_function = func;
		return true;
	}

	inline string getName() {
		return _name;
	}

	inline void executeCallfunc(Ref *param) {
		_function(param);
	}
};
#endif

}

#endif // __NDK_HELPER_DEF_H__