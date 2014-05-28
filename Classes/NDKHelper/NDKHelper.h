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
		friend NDKCalledSelector;
	public:
		//************************************
		// Method:    getInstance
		// FullName:  easyndk::NDKHelper::getInstance
		// Access:    public static 
		// Returns:   NDKHelper *
		// Qualifier: Returns the singleton of this class
		//************************************
		static NDKHelper *getInstance();

		//************************************
		// Method:    destroyInstance
		// FullName:  easyndk::NDKHelper::destroyInstance
		// Access:    public static 
		// Returns:   void
		// Qualifier: Destroys the singleton of this class
		//************************************
		static void destroyInstance();

		//************************************
		// Method:    addSelector
		// FullName:  easyndk::NDKHelper::addSelector
		// Access:    public 
		// Returns:   void
		// Qualifier: Register a method (given by target + selector) to be called if "name" is called by the native environment
		// Parameter: const string & groupName: An identifier to specify when you want to remove this selector
		// Parameter: const string & name: The name of the selector that native environment will call
		// Parameter: Ref * target: the target object to call when "name" is called by the native environment
		// Parameter: SEL_EasyNDKFunc selector: the selector to call when "name" is called by the native environment
		// Parameter: bool withCleanup: if true this selector will be automatically removed after a call so it's a single callable selector. Otherwise you will have to remove it your self. Default value is true.
		//************************************
		void addSelector(const string &groupName, const string &name, Ref* target, SEL_EasyNDKFunc selector, bool withCleanup = true);

		//************************************
		// Method:    addSelector
		// FullName:  easyndk::NDKHelper::addSelector
		// Access:    public 
		// Returns:   void
		// Qualifier: Register a lambda function to be called if "name" is called by the native environment
		// Parameter: const string & groupName: An identifier to specify when you want to remove this selector
		// Parameter: const string & name: The name of the selector that native environment will call
		// Parameter: const std::function<void(Ref *)> & selectorFunc: the lambda function to call when "name" is called by native environment
		// Parameter: bool withCleanup: if true this selector will be automatically removed after a call so it's a single callable selector. Otherwise you will have to remove it your self. Default value is true.
		//************************************
		void addSelector(const string &groupName, const string &name, const std::function<void(Ref *)> &func, bool withCleanup = true);

		//************************************
		// Method:    removeSelectorsInGroup
		// FullName:  easyndk::NDKHelper::removeSelectorsInGroup
		// Access:    public 
		// Returns:   void
		// Qualifier: Remove all selectors from the given group
		// Parameter: const string & groupName: name of the group to remove
		//************************************
		void removeSelectorsInGroup(const string &groupName);

		//************************************
		// Method:    printSelectorList
		// FullName:  easyndk::NDKHelper::printSelectorList
		// Access:    public 
		// Returns:   void
		// Qualifier: A dummy method that print all selectors in CCLOG's output. This method is only usefull when debuging.
		//************************************
		void printSelectorList();

		//************************************
		// Method:    handleMessage
		// FullName:  easyndk::NDKHelper::handleMessage
		// Access:    public 
		// Returns:   void
		// Qualifier: Do not call this method! It is called by native environments to call a selector.
		// Parameter: json_t * methodName
		// Parameter: json_t * methodParams
		//************************************
		void handleMessage(json_t *methodName, json_t* methodParams);

		//************************************
		// Method:    callSelector
		// FullName:  easyndk::NDKHelper::callSelector
		// Access:    public 
		// Returns:   void
		// Qualifier: This method will call a previously registered selector. The selector is found by his name (methodName
		// Parameter: string & methodName: the name of the selector
		// Parameter: Ref * params: parameters to pass to the selector
		//************************************
		void callSelector( const string &methodName, Ref * params );

		//************************************
		// Method:    sendMessage
		// FullName:  easyndk::NDKHelper::sendMessage
		// Access:    public 
		// Returns:   void
		// Qualifier: Send a message to native environment (use it to call your native code).
		// Parameter: const string & methodName: the method you want to call
		// Parameter: Ref * methodParams: (optional) params you want to pass
		//************************************
		void sendMessage(const string &methodName, Ref* methodParams = nullptr);

		//************************************
		// Method:    sendMessageWithCallbackSelector
		// FullName:  easyndk::NDKHelper::sendMessageWithCallbackSelector
		// Access:    public 
		// Returns:   void
		// Qualifier: Add a single callable selector then send a message to native environment (use it to call your native code that will call back your code once).
		// Parameter: const string & callbackSelectorName: it corresponds to "addSelector"'s "name" parameter
		// Parameter: Ref * target: it corresponds to "addSelector"'s "target" parameter
		// Parameter: SEL_EasyNDKFunc selector: it corresponds to "addSelector"'s "selector" parameter
		// Parameter: const string & methodName: it corresponds to "sendMessage"'s "methodName" parameter
		// Parameter: Ref * methodParams: it corresponds to "sendMessage"'s "methodParams" parameter
		//************************************
		void sendMessageWithCallbackSelector(const string &callbackSelectorName, Ref* target, SEL_EasyNDKFunc selector, const string &methodName, Ref* methodParams = nullptr);

		//************************************
		// Method:    sendMessageWithCallbackSelector
		// FullName:  easyndk::NDKHelper::sendMessageWithCallbackSelector
		// Access:    public 
		// Returns:   void
		// Qualifier: Add a single callable selector (with a lambda function) then send a message to native environment (use it to call your native code that will call back your code once).
		// Parameter: const string & callbackSelectorName: it corresponds to "addSelector"'s "name" parameter 
		// Parameter: const std::function<void>(Ref*) & func: it corresponds to "addSelector"'s "func" parameter
		// Parameter: const string & methodName: it corresponds to "sendMessage"'s "methodName" parameter
		// Parameter: Ref * methodParams: it corresponds to "sendMessage"'s "methodParams" parameter
		//************************************
		void sendMessageWithCallbackSelector(const string &callbackSelectorName, const std::function<void(Ref *)> &func, const string &methodName, Ref* methodParams = nullptr);

#if (!(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID && CC_TARGET_PLATFORM == CC_PLATFORM_IOS))
		//************************************
		// Method:    addMock
		// FullName:  easyndk::NDKHelper::addMock
		// Access:    public 
		// Returns:   void
		// Qualifier: Add a mock that simulate the native call of "methodName"
		// Parameter: const string & methodName: the name of the method that is simulated with mock
		// Parameter: const std::function<void(Ref *)> & func: the lambda function that simulates the native call
		//************************************
		void addMock(const string &methodName, const std::function<void(Ref *)> &func);

	protected:
		Vector<NDKMock*> registeredMocks;
#endif

	protected:
		CREATE_FUNC(NDKHelper);
		virtual bool init();
		virtual ~NDKHelper();
		void removeAtIndex(int index);

		static Ref* getRefFromJson(json_t *obj);
		static json_t* getJsonFromRef(Ref* obj);

	protected:
		static NDKHelper *_instance;
		Vector<NDKCallbackNode*> registeredSelectors;
		Vector<NDKCalledSelector*> calledSelectors;
	};
}

#endif // __NDK_HELPER_H__
