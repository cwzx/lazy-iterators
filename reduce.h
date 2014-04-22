/*
 * Chris Welshman 2014
 */
#ifndef INCLUDED_REDUCE
#define INCLUDED_REDUCE
#include <iterator>
#include <type_traits>

template<typename Range,typename F>
inline auto reduce( Range&& c, F&& f ) {
	auto it = begin(c);
	auto e = end(c);
	std::decay_t<decltype(*it)> x{};
	if( it != e ) {
		x = *it;
		for(++it;it!=e;++it) {
			x = f(x,*it);
		}
	}
	return x;
}

template<typename Range,typename F>
inline auto creduce( const Range& c, F&& f ) {
	auto it = cbegin(c);
	auto e = cend(c);
	std::decay_t<decltype(*it)> x{};
	if( it != e ) {
		x = *it;
		for(++it;it!=e;++it) {
			x = f(x,*it);
		}
	}
	return x;
}

template<typename Range,typename T,typename F>
inline auto reduce( Range&& c, T x, F&& f ) {
	auto it = begin(c);
	auto e = end(c);
	if( it != e ) {
		x = *it;
		for(++it;it!=e;++it) {
			x = f(x,*it);
		}
	}
	return x;
}

template<typename Range,typename T,typename F>
inline auto creduce( const Range& c, T x, F&& f ) {
	auto it = cbegin(c);
	auto e = cend(c);
	if( it != e ) {
		x = *it;
		for(++it;it!=e;++it) {
			x = f(x,*it);
		}
	}
	return x;
}

#endif
