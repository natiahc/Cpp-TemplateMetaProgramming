// 01-Sfinae.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include <type_traits>

// primary template class
template<typename T>
struct st_integer
{
	static const bool value = false;
};

// full specialization for template class
template<> // "full specialization" means template<> is used
struct st_integer<int>
{
	static const bool value = true;
    using type = int;
};

// full specialization for template class
template<>
struct st_integer<short>
{
	static const bool value = true;
    using type = short;
};

// full specialization for template class
template<>
struct st_integer<long>
{
	static const bool value = true;
    using type = long;
};

// full specialization for template class
template<>
struct st_integer<long long>
{
	static const bool value = true;
    using type = long long;
};

// full specialization for template class
template<> // "full specialization" means template<> is used
struct st_integer<unsigned int>
{
	static const bool value = true;
    using type = unsigned int;
};

// full specialization for template class
template<>
struct st_integer<unsigned short>
{
	static const bool value = true;
    using type = unsigned short;
};

// full specialization for template class
template<>
struct st_integer<unsigned long>
{
	static const bool value = true;
    using type = unsigned long;
};

// full specialization for template class
template<>
struct st_integer<unsigned long long>
{
	static const bool value = true;
    using type = unsigned long long;
};

// value template using constexpr keyword 
template<typename T> 
constexpr bool is_integer = st_integer<T>::value;

// using temmplate - type alias
template<typename T>
using integer_t = typename st_integer<T>::type;


// primary class template
template<typename T1, typename T2>
struct st_same_type
{
    static const bool value = false;
};

// partial specialization
template<typename T> // partial means we still need template parameter
struct st_same_type<T, T>
{
    static const bool value = true;
    using type = T;
};

// value template
template<typename T1, typename T2>
constexpr bool same_v = st_same_type<T1, T2>::value;

// using template - type alias
template<typename T1, typename T2>
using same_t = typename st_same_type<T1, T2>::type;

// primary class template
// initialize the template parameter 
// then provide default parameter at primary class template
template<bool TEST, typename RType=void>
struct st_enable_true
{
    static const bool value = TEST;
};

// partial specialization
template<typename RType>
struct st_enable_true<true, RType>
{
    static const bool value = true;
    using type = RType;
};

template<bool TEST, typename RType>
using enable_true_t = typename st_enable_true<TEST, RType>::type;


/*
	type list enclosed with < > before class definition
	is called template parameter list
*/

template<typename T, 
		typename = integer_t<T> // SFINAE out any non-integral types 
>
class Modulo
{
protected:
	T m_mem;

public:
	explicit Modulo(T m = T{}) : m_mem{ m } { }

	// implement operator % only for integral type
	// break the tie between class template 
	// and operator% member function

	// if you want to implement (enable or disable) member function
	// depending on the types of template arguments,
	// we have to make member function "function template"

	// SFINAE out any non-integral type of S
	// if S is non-integral type,
	// then this function disappears at compile time
	// template<typename S, typename = integer_t<S> >
	// same_t<S, T> operator%(S m)
	// {
	// 	return (this->m_mem % m);
	// }

	// second way
	// template<typename S, typename = integer_t<S> >
	// enable_true_t<same_v<S, T>, S> 
	
	// in standard c++
	template<typename S, typename = integer_t<S> >
	std::enable_if_t<std::is_same_v<S, T>, S>
	operator%(S m)
	{
		return (this->m_mem % m);
	}

	friend std::ostream& operator<<(std::ostream& os, const Modulo& m)
	{
		os << m.m_mem;
		return os;
	}
};

int main()
{
	Modulo<int> m{ 5 };

	int n{ 3 };

	std::cout << m << "%" << n << "=" << (m%n) << std::endl;

	// in c++17 standard, class template argument list
	// is deduced using its constructor
	Modulo k{ 7 };
	std::cout << k << "%" << n << "=" << (k%n) << std::endl;

	Modulo j{ 5 };
	//double d = 3.0;
	short d = 3;
	// make the follwing code error
	//std::cout << j << "%" << d << "=" << (j%d) << std::endl;

	Modulo l{ 5u };
	unsigned int ui = 3;
	std::cout << l << "%" << ui << "=" << (l%ui) << std::endl;
}