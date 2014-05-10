This branch is under developement. Your help is welcome!
====================

# EasyNDK-for-cocos2dx

This repository is a fork of the original project initiated by [aajiwani](https://github.com/aajiwani/EasyNDK-for-cocos2dx).

EasyNDK is a framework that simplifies the work of communicating between C++ code and platform's native language in Cocos2d-X. For example if you work on Android, you will have to call JNI functions which is a bit complicated.
This framework allow you to call a single function in your C++ instead of writing code for every platform.

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
**_TODO_**

### In your Objective-C code
**_TODO_**

## Win32 / Visual Studio 2012
I use this platform only to develop the cross-platform part my Cocos2d-X games. Since you use EasyNDK in your projects, you will need to mock the calls to simulate the native environment.

### Setup your project
**_TODO_**

### Creating mocks
**_TODO_**

## Windows Phone 8 / Visual Studio 2012 with Windows Phone 8 SDK
**_TODO_**

# Improvements of EasyNDK-for-cocos2dx (By TheArsenik)

I started to modify this framework because I was wondering EasyNDK was working Cocos2d-x nodes instead of objects (of course "cocos2d::Ref" because it simplifies memory management regarding C++ mecanisms).

I improved the original project and I'm planning to adapt it to Cocos2d-X 3 (as you can see this branch is under developement).

I'm also planning to adapt EasyNDK for Windows Phone 8 which is the 3rd popular mobile platform and Win32 Mocks to let us develop under Windows without modifications.

I will explain more my thoughts when this project will be refactored to Cocos2d-X 3 but keep this project starred!
