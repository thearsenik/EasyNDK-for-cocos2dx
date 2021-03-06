This branch is under developement. Your help is welcome!
====================

# EasyNDK-for-cocos2dx

This repository is a fork of the original project initiated by [aajiwani](https://github.com/aajiwani/EasyNDK-for-cocos2dx).

EasyNDK is a framework that simplifies the work of communicating between C++ code and platform's native language in Cocos2d-X. For example if you work on Android, you will have to call JNI functions which is a bit complicated.
This framework allows you to call a single function in C++ instead of writing code for every platform.

## How it works?
Every call is encoded by EasyNDK's Helper in a JSON message that is transmitted to the native part of EasyNDK. The native part of EasyNDK is using reflection mecanism to find the proper method. You can pass parameters to your methods so you are able to communicate without limitation.

**Warning:** The way this framework is working should warn you that it is made to be used with sparingly (encoding/decoding into/from JSON and reflection mecanism are a bit costly in time). I recommend you not to use this framework if you want to call native code every frame of your Cocos2d-X app. Otherwise you can use it.

## In your C++ code
*All your C++ code will require to include "NDKHelper.h" and use "easyndk" namespace. So please copy the following code in all your files*
```C++
#include "NDKHelper.h"

using namespace easyndk;
```

### Calling a native method
EasyNDK let you call a native method with a single line of code:
```C++
NDKHelper::getInstance()->sendMessageWithParams("showAd");
```
And with parameters:
```C++
cocos2d::String *param = cocos2d::String("my name is not Mr.Anderson...");
NDKHelper::getInstance()->sendMessageWithParams("showLeaderboard", param);
```

Parameters could be of type:

- ``` easyndk::NDKDictionary * ```
- ``` easyndk::NDKArray * ```
- ``` cocos2d::String * ```
- ``` cocos2d::Integer * ```

The native method will be called by using the reflection mecanism so that's all you have to do. **Please read the section about your platform to know what you have to do on it.**

### Handling a native method call
Of course EasyNDK let you communicate from native environment to your C++ code. Before calling a C++ method from your native environment, you need to tell EasyNDK which selector to call. In native environment this step is not needed because there are reflection mecanisms but C++ does not provide this functionnality (this would be a greate improvement isn't it?).
So you need to call "addSelector" method from the NDKHelper which fills a "Map" that makes the connection between a name of a method and the method it self.
Here is an example of what you have to write in your C++ code:
```C++
NDKHelper::getInstance()->addSelector("MyGroup", "showLeaderboardCallback", [](Ref *param) {
 // Do something
}, false); 
```
You can also use the C++ version with an object (target) and a function pointer (selector):
```C++
NDKHelper::getInstance()->addSelector("MyGroup2", "showAchievementsCallback", this, easyndkfunc_selector(MyMenu::showAchievementsCallback), false); 
```

Now you can call you C++ method from your native environment. **Please read the section about your platform to know what you have to do on it.**

**Note:** You have to remove your selector when you delete the target object. This point is very important because it could lead to crashes if you don't do it. If your selector will be called once, pass "true" as the last parameter in "addSelector" method.

### An easier way to write your code
In most cases you will need to call a native method and have a method that will be called back when native code execution has ended. That's why EasyNDK provides a simple way to call a method and to execute a callback method.
Here is an example:
```C++
NDKHelper::getInstance()->sendMessageWithCallbackSelector("showAdCallback", [](cocos2d::Ref *param) {
	cocos2d::String *paramStr = static_cast<cocos2d::String *>(param);
	if(paramStr->boolValue()) {
		CCLOG("The user watched my Ad! Because I need $ .... ");
	}
	else {
		CCLOG("No ad to watch");
	}
}, "showAd");
```
In this case your selector can be called only once. It is automatically removed after the native call (this is the cleanup functionnality) but this is what you need in most cases.

## Android
**_TODO_**

## iOS SDK / XCode
### Setup your project
Once you generated a project with Cocos2d-X tools, open your XCode project.
In XCode drag & drop EasyNDK folder located in PATH_TO_EASYNDK/proj.ios/ into your project. Select the following options and click on Finish.

![](https://raw.githubusercontent.com/thearsenik/EasyNDK-for-cocos2dx/cocos2d-x-3/tutorial/add%20options.png)

Add also the folders located in PATH_TO_EASYNDK/Classes/ (jansson and NDKHelper) into your project (in your Classes or in EasyNDK, it doesn't matter). You should have this kind of project tree:

![](https://raw.githubusercontent.com/thearsenik/EasyNDK-for-cocos2dx/cocos2d-x-3/tutorial/project%20tree.png)

Now you need to define a delegate that will handle your C++ calls. Usually the RootViewController is a good choice to do this job but you can create an other class to do it. Here is the code to define the delegate:
```Objective-C
[IOSNDKHelper setNDKReciever:self];
```

### In your Objective-C code
When you call a method from C++, EasyNDK searches for it in the delegate class. The method must have the following prototype:
```Objective-C
-(void) foo:(NSObject*)param;
```

To call a method from Objective-C to C++ (for instance when you callback your code after a job) you simply call IOSNDKHelper like that:
```Objective-C
[IOSNDKHelper sendMessage:cppFunctionToBeCalled withParameters:nil];
```

## Win32 / Visual Studio 2012
I use this platform only to develop the cross-platform part my Cocos2d-X games. Since you use EasyNDK in your projects, you will need to mock the calls to simulate the native environment.

### Setup your project
Open your solution in Visual Studio (I'm using Visual Studio 2012). You need to add jansson then libEasyNDK. Do the following steps:

1. Right click on your solution > Add... > Existing project
2. Select jansson.vcxproj located in EasyNDK/proj.win32/jansson-2.5/win32/vs2010/
3. Right click on your solution > Add... > Existing project
4. Select libEasyNDK.vcxproj located in EasyNDK/proj.win32/

Don't forget to tell Visual Studio you depend on EasyNDK project and to add jansson and EasyNDK folders to include paths by:

1. Right click on your project (your vcxproj) > properties
2. In the left tree view select Common properties > Framework and References
3. Click on the "Add References" button
4. Check libEasyNDK project
5. Then click, in the tree view, on Configuration Properties > C/C++
6. In the "Additionnal Include Directories" input, add these entries:
  * "PATH_TO_EASYNDK\Classes\NDKHelper\"
  * "PATH_TO_EASYNDK\proj.win32\jansson-2.5\src"

Here it is! You should be able to compile & run your project. The only problem is when you call a method and you are waiting for a callback it will never happen because there is no different language and no native functionnality you could invoke.
To simulate a native call you can create mocks.

### Creating mocks
In your project you should have a main.cpp file that starts Cocos2d-X.
Before:
```C++
// create the application instance
AppDelegate app;
return Application::getInstance()->run();
```
add your mocks:
```C++
easyndk::NDKHelper::getInstance()->addMock("myMethod", [](Ref *params) {
  auto parameters = easyndk::NDKDictionary<cocos2d::Ref*>::create();
  parameters->insert("foo", cocos2d::String::create("bar"));
  parameters->insert("number", cocos2d::String::create("2"));
  easyndk::NDKHelper::getInstance()->callSelector("myMethodCallback", parameters);
});
```
You can also encapsulate your mocks in a seperate class but don't forget to call your code before cocos2d-X.

## Windows Phone 8 / Visual Studio 2012 with Windows Phone 8 SDK
**_TODO_**

# My improvements

I started to modify this framework because I was wondering EasyNDK was working Cocos2d-x nodes instead of objects (of course "cocos2d::Ref" because it simplifies memory management regarding C++ mecanisms).

I improved the original project and I'm planning to adapt it to Cocos2d-X 3 (as you can see this branch is under developement).

I'm also planning to adapt EasyNDK for Windows Phone 8 which is the 3rd popular mobile platform and Win32 Mocks to let us develop under Windows without modifications.
I also want to rewrite the existing code to fit with C++ conventions.

I will explain more my thoughts when this project will be refactored to Cocos2d-X 3 but keep this project starred!
