# EventSimulator
This is an event-driven programming framework inspired by [ns-3](https://www.nsnam.org/). When I use ns-3 to finish my academic project, I find the event driven framework used by ns-3 is very easy to use and extend. So I decide to reinvent the wheel to sharp my C++ knowledge and skill.

## Features
* Intrusive smart pointer
* Runtime type and attribute system
* Callback wrapper
* Event scheduler framework
* ObjectFactory using `string` name to configure and create objects

## Object Model
There are three special base classes:
* `class Object`
* `class ObjectBase`
* `class RefCount`

Inheriting from `RefCount` gives your class the smart pointer property. Inheriting from `ObjectBase` gives you the type and attribute property. Deriving from `Object` get both the properties above.<br>

### Smart pointer property
I use CRTP to implement the base class template `RefCount`. `RefCount` provide the object the reference count function. The burden for calling `Unref()` `Ref()` is relieved by use the smart pointer class `Ptr`.

### Type and attribute system
To register a user defined class into the runtime type attribute system, You must inherit your class from `ObjectBase` and define a `GetTypeID` public static member function. This function adds the attributes of the derived class to the type system. The `OBJECT_ENSURE_REGISTERED()` marco will ensure this function to be called before the running of the simulation system.

### Callback wrapper
I use template class partial specialization to increase the safety and efficiency of the callback template wrapper. <br>
If the `template CallbackImpl` contains all the possible `operator()` interface, The derived `FuncCallbackImpl` functor and `MemFuncCallbakcImpl` functor must implement all these interface. This is impossible because of the actual type of the function pointer they contain. By using class template partial specialization and the feature that only the called template member function will be initialized, we can customize the interface `FuncCallbackImpl` and `MemFuncCallbakcImpl` inherits and ensure the type safety. 
