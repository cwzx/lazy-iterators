/*
 * Chris Welshman 2014
 */
#ifndef INCLUDED_REDUCE
#define INCLUDED_REDUCE
#include <iterator>

template<typename Container,typename F>
inline auto reduce( Container&& c, F&& f ) {
	auto it = begin(c);
	auto e = end(c);
	auto x = *it;
	for(++it;it!=e;++it) {
		x = f(x,*it);
	}
	return x;
}

template<typename Container,typename F>
inline auto creduce( Container&& c, F&& f ) {
	auto it = cbegin(c);
	auto e = cend(c);
	auto x = *it;
	for(++it;it!=e;++it) {
		x = f(x,*it);
	}
	return x;
}

#endif
