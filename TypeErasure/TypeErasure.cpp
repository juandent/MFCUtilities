// TypeErasure.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

#include <functional>
#include <iostream>
#include <unordered_map>
#include<memory>
#include<utility>

class BaseEvent
{
public:
	virtual ~BaseEvent() {}
};

class DerivedEvent_1 : public BaseEvent {};
class DerivedEvent_2 : public BaseEvent {};

struct BaseWrapper {
	virtual void operator()(const BaseEvent *) = 0;
};

template<typename T>
struct Wrapper : BaseWrapper {
	std::function<void(T)> fn;
	void operator()(const BaseEvent *e) override {
		fn(*static_cast<const T*>(e));
	}
};


// a container holding callback functions
std::unordered_map<size_t/*event*/, std::unique_ptr<BaseWrapper>/*callback*/> _functions;


// register callback funtion associated with specific event
template<typename EVT>
void registerCallback(std::function<void(const EVT &)> cb)
{
	std::cout << "store callback associated with event " << typeid(EVT).name() << " [" << typeid(EVT).hash_code() << "]" << std::endl;
	auto w = std::make_unique<Wrapper<EVT>>();
	w->fn = cb;
	_functions[typeid(EVT).hash_code()] = std::move(w);
}


#if 1

void f_source(const int& val)
{
	auto x = typeid(int).hash_code();
}


void CopyFn()
{
	std::function<void(const int&)> fn_source = f_source;
	std::function<void(int)> fn = f_source;
}

void caller()
{
	CopyFn();
}
#endif


// trigger callback function
void triggerCallback(const BaseEvent* e)
{
	std::cout << "trigger callback with event " << typeid(*e).name() << " [" << typeid(*e).hash_code() << "]" << std::endl;
	(*_functions[typeid(*e).hash_code()])(e);
}

// callback function for DerivedEvent_1
void callback_1(const DerivedEvent_1 &event_1)
{
	std::cout << "callback_1 called" << std::endl;
}

// callback function for DerivedEvent_2
void callback_2(const DerivedEvent_2 &event_2)
{
	std::cout << "callback_2 called" << std::endl;
}

int main(int argc, char *argv[])
{
	registerCallback<DerivedEvent_1>([](DerivedEvent_1 e) { callback_1(e); });
	registerCallback<DerivedEvent_2>([](DerivedEvent_2 e) { callback_2(e); });

	DerivedEvent_1 e1;
	DerivedEvent_2 e2;

	triggerCallback(&e1);
	triggerCallback(&e2);

	return 1;
}




// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
