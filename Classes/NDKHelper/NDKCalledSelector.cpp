#include "NDKCalledSelector.h"
#include "NDKCallbackNode.h"

namespace easyndk {

NDKCalledSelector* NDKCalledSelector::create( NDKCallbackNode *callback, Ref *param )
{
	auto ret = new NDKCalledSelector();
	if (ret && ret->init(callback, param) ) {
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

bool NDKCalledSelector::init( NDKCallbackNode *callback, Ref *param )
{
	this->_callback = callback;
	CC_SAFE_RETAIN(callback);
	this->_param = param;
	CC_SAFE_RETAIN(param);

	return true;
}

NDKCalledSelector::~NDKCalledSelector()
{
	CC_SAFE_RELEASE(_param);
}

void NDKCalledSelector::executeCallback()
{
	_callback->executeCallfunc(_param);
}

}