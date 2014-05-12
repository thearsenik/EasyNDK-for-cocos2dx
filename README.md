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
### Calling a native method
**_TODO_**

### Handling a native method call
**_TODO_**

## Android
**_TODO_**

## iOS SDK / XCode
### Setup your project
Once you generated a project with Cocos2d-X tools, open your XCode project.
In XCode click on Files > Add file in <your project>. Select libEasyNDK.xcodeproj located in EasyNDK/proj.ios.
You also need to define a delegate that will handle your C++ calls. Usually the RootViewController is a good choice to do this job but you can create an other class to do it. Here is the code to define the delegate:
```Objective-C
[IOSNDKHelper SetNDKReciever:self];
```

### In your Objective-C code
When you call a method from C++, EasyNDK searches for it in the delegate class. The method must have the following prototype:
```Objective-C
-(void) foo:(NSObject*)param;
```

To call a method from Objective-C to C++ (for instance when you callback your code after a job) you simply call IOSNDKHelper like that:
```Objective-C
[IOSNDKHelper SendMessage:CPPFunctionToBeCalled WithParameters:nil];
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
1. Right click on the solution > properties
2. In the left tree view select Common properties > Framework and References
3. Click on the "Add References" button
4. Check libEasyNDK project
5. Then click, in the tree view, on Configuration Properties > C/C++
6. In the "Additionnal Include Directories" input, add this: "<path_to_easyndk>\Classes\NDKHelper\"

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
NDKHelper::getInstance()->addMock("myMethod", [](Ref *params) {
  cocos2d::Map<std::string, cocos2d::Ref *> parameters;
  parameters.insert("foo", cocos2d::String("bar"));
  parameters.insert("number", cocos2d::String("2"));
  NDKHelper::getInstance()->callSelector("myMethodCallback", parameters);
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
