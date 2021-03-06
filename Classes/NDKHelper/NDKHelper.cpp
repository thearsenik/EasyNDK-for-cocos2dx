//
//  NDKHelper.cpp
//  EasyNDK-for-cocos2dx
//
//  Created by Amir Ali Jiwani on 23/02/2013.
//	Rewritten by Na�l MSKINE on 12/05/2014.
//
//

#include "NDKHelper.h"
#include "NDKCallbackNode.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#import "IOSNDKHelper-C-Interface.h"
#endif

namespace easyndk {

#define K_CALLED_METHOD_NAME "calling_method_name"
#define K_CALLED_METHOD_PARAMS "calling_method_params"

	NDKHelper *NDKHelper::_instance = nullptr;

	NDKHelper *NDKHelper::getInstance()
	{
		if(_instance == nullptr) {
			_instance = NDKHelper::create();
			_instance->retain();
		}
		return _instance;
	}

	void NDKHelper::destroyInstance()
	{
		CC_SAFE_RELEASE_NULL(_instance);
	}

	bool NDKHelper::init()
	{
		return true;
	}

	NDKHelper::~NDKHelper()
	{
	}

	void NDKHelper::addSelector( const string &groupName, const string &name, Ref* target, SEL_EasyNDKFunc selector, bool withCleanup /*= true*/ )
	{
		registeredSelectors.pushBack(NDKCallbackNode::create(groupName, name, target, selector, withCleanup));
	}

	void NDKHelper::addSelector( const string &groupName, const string &name, const std::function<void(Ref *)> &func, bool withCleanup /*= true*/ )
	{
		registeredSelectors.pushBack(NDKCallbackNode::create(groupName, name, func, withCleanup));
	}

	void NDKHelper::removeAtIndex(int index)
	{
		registeredSelectors.erase(index);
	}

	void NDKHelper::removeSelectorsInGroup(const string &groupName)
	{
		for(auto selector : registeredSelectors) {
			if(selector->getGroup() == groupName) {
				registeredSelectors.eraseObject(selector);
			}
		}
	}

	Ref* NDKHelper::getRefFromJson(json_t *obj)
	{
		if (obj == NULL)
			return NULL;

		if (json_is_object(obj))
		{
			NDKDictionary<Ref*> *dictionary = NDKDictionary<Ref*>::create();

			string key;
			json_t *value;

			void *iter = json_object_iter(obj);
			while(iter)
			{
				key = json_object_iter_key(iter);
				value = json_object_iter_value(iter);

				dictionary->insert(key, getRefFromJson(value));

				iter = json_object_iter_next(obj, iter);
			}

			return dictionary;
		}
		else if (json_is_array(obj))
		{
			size_t sizeArray = json_array_size(obj);
			NDKArray<Ref*> *array = NDKArray<Ref*>::create();

			for (unsigned int i = 0; i < sizeArray; i++)
			{
				array->pushBack(getRefFromJson(json_array_get(obj, i)));
			}

			return array;
		}
		else if (json_is_boolean(obj))
		{
			stringstream str;
			str << json_is_true(obj);

			String *ccString = String::create(str.str());
			return ccString;
		}
		else if (json_is_integer(obj))
		{
			stringstream str;
			str << json_integer_value(obj);

			String *ccString = String::create(str.str());
			return ccString;
		}
		else if (json_is_real(obj))
		{
			stringstream str;
			str << json_real_value(obj);

			String *ccString = String::create(str.str());
			return ccString;
		}
		else if (json_is_string(obj))
		{
			stringstream str;
			str << json_string_value(obj);

			String *ccString = String::create(str.str());
			return ccString;
		}
		else if (json_is_null(obj)) 
		{
			return String::create("null");
		}
		else {
			CCLOG("EasynNDK Warning: can't convert to a Ref* a json object from an unknown type");
		}

		return NULL;
	}

	json_t* NDKHelper::getJsonFromRef(Ref* obj)
	{
		if (dynamic_cast<NDKDictionary<Ref*>*>(obj))
		{
			NDKDictionary<Ref*> *mainDict = (NDKDictionary<Ref*>*)obj;

			json_t* jsonDict = json_object();
			for(auto dictElt = mainDict->begin(); dictElt != mainDict->end(); dictElt++) {
				json_object_set_new(jsonDict,
					dictElt->first.c_str(),
					getJsonFromRef(dictElt->second));
			}

			return jsonDict;
		}
		else if (dynamic_cast<NDKArray<Ref*>*>(obj))
		{
			NDKArray<Ref*>* mainArray = (NDKArray<Ref*>*)obj;
			json_t* jsonArray = json_array();

			for (auto ref : *mainArray) {
				json_array_append_new(jsonArray,
					getJsonFromRef(ref));
			}

			return jsonArray;
		}
		else if (dynamic_cast<String*>(obj))
		{
			String* mainString = (String*)obj;
			json_t* jsonString = json_string(mainString->getCString());

			return jsonString;
		}
		else if(dynamic_cast<Integer*>(obj)) 
		{
			Integer *mainInteger = (Integer*) obj;
			json_t* jsonString = json_integer(mainInteger->getValue());

			return jsonString;
		}

		return NULL;
	}

	void NDKHelper::printSelectorList()
	{
		for(auto selector : registeredSelectors) {
			string logStr = "Selector: " + selector->getName() + " Group:" + selector->getGroup();
			CCLOG(logStr.c_str());
		}
	}

	void NDKHelper::handleMessage(json_t *methodName, json_t* methodParams)
	{
		if (methodName == NULL) {
			return;
		}

		Ref *param = getRefFromJson(methodParams);
		string methodNameStr = json_string_value(methodName);
		callSelector(methodNameStr, param);


	}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	// TODO
#include "../cocos2dx/platform/android/jni/JniHelper.h"
#include <android/log.h>
#include <jni.h>
#define  LOG_TAG    "EasyNDK-for-cocos2dx"

#define CLASS_NAME "com/easyndk/classes/AndroidNDKHelper"

#endif
    
	extern "C"
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		// Method for recieving NDK messages from Java, Android
		void Java_com_easyndk_classes_AndroidNDKHelper_CPPNativeCallHandler(JNIEnv* env, jobject thiz, jstring json)
		{
			string jsonString = JniHelper::jstring2string(json);
			const char *jsonCharArray = jsonString.c_str();

			json_error_t error;
			json_t *root;
			root = json_loads(jsonCharArray, 0, &error);

			if (!root)
			{
				fprintf(stderr, "error: on line %d: %s\n", error.line, error.text);
				return;
			}

			json_t *jsonMethodName, *jsonMethodParams;
			jsonMethodName = json_object_get(root, K_CALLED_METHOD_NAME);
			jsonMethodParams = json_object_get(root, K_CALLED_METHOD_PARAMS);

			// Just to see on the log screen if messages are propogating properly
			__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, jsonCharArray);

			NDKHelper::getInstance()->HandleMessage(jsonMethodName, jsonMethodParams);
			json_decref(root);
		}
#endif

		// Method for sending message from CPP to the targetted platform
		void NDKHelper::sendMessage(const string &methodName, Ref* methodParams)
		{
			if (0 == strcmp(methodName.c_str(), "")) {
				return;
			}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
			json_t *toBeSentJson = json_object();
			json_object_set_new(toBeSentJson, K_CALLED_METHOD_NAME, json_string(methodName.c_str()));

			if (methodParams != nullptr)
			{
				json_t* paramsJson = getJsonFromRef(methodParams);
				json_object_set_new(toBeSentJson, K_CALLED_METHOD_PARAMS, paramsJson);
			}

			JniMethodInfo t;

			if (JniHelper::getStaticMethodInfo(t,
				CLASS_NAME,
				"RecieveCppMessage",
				"(Ljava/lang/String;)V"))
			{
				char* jsonStrLocal = json_dumps(toBeSentJson, JSON_COMPACT | JSON_ENSURE_ASCII);
				string jsonStr(jsonStrLocal);
				free(jsonStrLocal);

				jstring stringArg1 = t.env->NewStringUTF(jsonStr.c_str());
				t.env->CallStaticVoidMethod(t.classID, t.methodID, stringArg1);
				t.env->DeleteLocalRef(stringArg1);
				t.env->DeleteLocalRef(t.classID);
			}

			json_decref(toBeSentJson);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
			json_t *jsonMessageName = json_string(methodName.c_str());

			if (methodParams != nullptr)
			{
				json_t *jsonParams = getJsonFromRef(methodParams);
				IOSNDKHelperImpl::receiveCppMessage(jsonMessageName, jsonParams);
				json_decref(jsonParams);
			}
			else
			{
				IOSNDKHelperImpl::receiveCppMessage(jsonMessageName, nullptr);
			}

			json_decref(jsonMessageName);
#else
			for(auto mock : registeredMocks) {
				if(mock->getName() == methodName) {
					mock->executeCallfunc(methodParams);
				}
			}
#endif
		}
	}

	void NDKHelper::sendMessageWithCallbackSelector( const string &callbackSelectorName, Ref* target, SEL_EasyNDKFunc selector, const string &methodName, Ref* methodParams /*= nullptr*/ )
	{
		addSelector("", callbackSelectorName, target, selector, true);
		sendMessage(methodName, methodParams);
	}

	void NDKHelper::sendMessageWithCallbackSelector( const string &callbackSelectorName, const std::function<void(Ref *)> &func, const string &methodName, Ref* methodParams /*= nullptr*/ )
	{
		addSelector("", callbackSelectorName, func, true);
		sendMessage(methodName, methodParams);
	}
    
#if (!(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID && CC_TARGET_PLATFORM == CC_PLATFORM_IOS))
	void NDKHelper::addMock( const string &methodName, const std::function<void(Ref *)> &func )
	{
		registeredMocks.pushBack(NDKMock::create(methodName, func));
	}
#endif

	void NDKHelper::callSelector( const string &methodName, Ref * param )
	{
		for (auto selector : registeredSelectors) {
			if(selector->getName() == methodName) {
				calledSelectors.pushBack(NDKCalledSelector::create(selector, param));
			}
		}

		string methodKey = "EasyNDKCalledSelectorsUpdate";
		Director::getInstance()->getScheduler()->schedule([&](float delta) {
			if(calledSelectors.size() > 0) {
				for (auto selector : calledSelectors) {
					selector->executeCallback();
				}
				calledSelectors.clear();
			}
		}, this, 0, 0, 0, false, methodKey);
	}

}