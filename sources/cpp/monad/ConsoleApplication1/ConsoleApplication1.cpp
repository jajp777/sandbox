// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <functional>
#include <iostream>
#include <tuple>

struct NoneToken
{
	static NoneToken None;
};
NoneToken NoneToken::None = {};

template <typename T>
class InverseWrapper
{
	bool None;
	const T& Value;
public:
	InverseWrapper(bool none, const T& value) : None(none), Value(value)
	{
	}

	template <typename F>
	void operator >> (F f)
	{
		if (None)
		{
			f();
		}
	}
};

template <typename T>
class Wrapper
{
	bool None;
	T Value;

	//template <typename T2> friend Wrapper<T2>;
public:
	Wrapper(const NoneToken& token) : None(true)
	{
	}

	Wrapper(const T&& item) : None(false), Value(item)
	{
	}

	template <typename F>
	InverseWrapper<T> operator >> (F f)
	{
		if (!None)
			f(Value);
		return { None,Value };
	}

	template <typename T2>
	Wrapper<std::tuple<T, T2>> Join (Wrapper<T2> w2)
	{
		if (None || w2.None)
			return NoneToken::None;
		return std::make_tuple( Value, w2.Value );
	}
};

template<typename T1, typename T2, template<typename> typename W>
W<std::tuple<T1, T2>> operator + (W<T1> w1, W<T2> w2)
{
	return w1.Join(w2);
}

template <typename T, template<typename> typename W>
class Enumerable
{
	unsigned int Count;
	char ** Strings;
public:
	Enumerable(unsigned int count, char** strings)
	{
		Count = count;
		Strings = strings;
	}

	W<T> operator [] (unsigned int i) const noexcept
	{
		if (i < Count)
			return std::string{ Strings[i] };
		else
			return NoneToken::None;
	}

	W<T> First() const noexcept
	{
		return std::string{ Strings[0] };
	}
};

Enumerable<std::string, Wrapper> wrap(unsigned int count, char** strings)
{
	return { count, strings };
}

#define ONVALUE >> [](const auto& x) {
#define ONVALUEXY >> [](const auto& item) {auto[x,y] = item; x; y;
#define ONEMPTY >> []() {

int main(int argc, char**argv)
{
	auto args = wrap((unsigned int)argc, argv);
	args[0] + args[1] 
		ONVALUEXY std::cout << x << std::endl; }
		ONEMPTY std::cout << "Empty" << std::endl; };
	std::string str;
	std::cin >> str;
	return 0;
}

