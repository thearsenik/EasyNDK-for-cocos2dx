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

namespace easyndk {

typedef void (Ref::*SEL_EasyNDKFunc)(Ref*);
#define easyndkfunc_selector(_SELECTOR) static_cast<easyndk::SEL_EasyNDKFunc>(&_SELECTOR)
}

#endif // __NDK_HELPER_DEF_H__