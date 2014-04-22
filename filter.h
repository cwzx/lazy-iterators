/*
 * Chris Welshman 2014
 */
#ifndef INCLUDED_FILTER
#define INCLUDED_FILTER
#include <iterator>
#include <utility>

template<typename F,typename Iterator>
struct filter_range;

template<typename F,typename Iterator>
struct filter_iterator {
	typedef typename std::iterator_traits<Iterator>::value_type        original_value_type;
	typedef typename std::iterator_traits<Iterator>::reference         original_reference;
	typedef typename std::iterator_traits<Iterator>::difference_type   difference_type;
	typedef typename std::iterator_traits<Iterator>::iterator_category iterator_category;
	typedef original_value_type          value_type;
	typedef original_reference           reference;
	typedef Iterator                     pointer;
	typedef std::pair<Iterator,Iterator> range_type;

	filter_iterator() = default;

	filter_iterator( const F& f, const range_type& r ) : f(f), range(r) {
		if( range.first != range.second ) {
			while( range.first != range.second && !f(*range.first) ) {
				++range.first;
			}
			--range.second;
			while(!f(*range.second)) {
				--range.second;
			}
			++range.second;
		}
		it = range.first;
	}

	filter_iterator( const F& f, const range_type& range, const Iterator& i ) : filter_iterator(f,range) {
		it = i;
	}

	filter_iterator( const F& f, const Iterator& first, const Iterator& last ) : filter_iterator(f,range_type(first,last)) {}
	
	filter_iterator( const F& f, const Iterator& first, const Iterator& last, const Iterator& it ) : filter_iterator(f,range_type(first,last),it) {}
	
	reference operator*() const {
		return *it;
	}

	pointer operator->() const {
		return it;
	}

	filter_iterator<F,Iterator>& operator++() {
		do {
			++it;
		} while( it != range.second && !f(*it) );
		return *this;
	}

	filter_iterator<F,Iterator> operator++(int) {
		filter_iterator<F,Iterator> temp = *this;
		++(*this);
		return temp;
	}

	filter_iterator<F,Iterator>& operator--() {
		do {
			--it;
		} while( !f(*it) );
		return *this;
	}

	filter_iterator<F,Iterator> operator--(int) {
		filter_iterator<F,Iterator> temp = *this;
		--(*this);
		return temp;
	}

	filter_iterator<F,Iterator>& operator+=( difference_type offset ) {
		it += offset;
		return *this;
	}

	filter_iterator<F,Iterator> operator+( difference_type offset ) const {
		filter_iterator<F,Iterator> temp = *this;
		return temp += offset;
	}

	filter_iterator<F,Iterator>& operator-=( difference_type offset ) {
		return *this += -offset;
	}

	filter_iterator<F,Iterator> operator-( difference_type offset ) const {
		filter_iterator<F,Iterator> temp = *this;
		return temp -= offset;
	}

	difference_type operator-( const filter_iterator<F,Iterator>& rhs ) const {
		return std::distance( rhs.it, it );
	}

	reference operator[]( difference_type offset ) const {
		return *(*this + offset);
	}

	difference_type index() const {
		return N = std::distance( range.first, it );
	}

	bool operator==( const filter_iterator<F,Iterator>& rhs ) const {
		if( it == rhs.it )
			return true;
		return false;
	}

	bool operator!=( const filter_iterator<F,Iterator>& rhs ) const {
		return !(*this == rhs);
	}

	bool operator<( const filter_iterator<F,Iterator>& rhs ) const {
		return rhs - *this > 0;
	}

	bool operator>( const filter_iterator<F,Iterator>& rhs ) const {
		return rhs < *this;
	}

	bool operator<=( const filter_iterator<F,Iterator>& rhs ) const {
		return !( *this > rhs );
	}

	bool operator>=( const filter_iterator<F,Iterator>& rhs ) const {
		return !( *this < rhs );
	}

	friend filter_range<F,Iterator>;

protected:
	range_type range;
	Iterator it;
	F f;
};

template<typename F,typename Iterator>
struct filter_range {
	typedef typename std::iterator_traits<Iterator>::value_type      value_type;
	typedef typename std::iterator_traits<Iterator>::difference_type difference_type;
	typedef Iterator original_iterator;
	typedef filter_iterator<F,original_iterator> iterator;
	typedef std::reverse_iterator<iterator>      reverse_iterator;
	typedef std::pair<Iterator,Iterator>         range_type;

	filter_range( const F& f, const range_type& range ) : f(f), range(range) {}
		
	filter_range( const F& f, const Iterator& first, const Iterator& last ) : map_range(f,range_type(first,last)) {}
		
	difference_type size() const {
		return std::distance( range.first, range.second );
	}

	iterator begin() const {
		return iterator( f, range );
	}

	iterator end() const {
		iterator r( f, range );
		r.it = r.range.second;
		return r;
	}

protected:
	range_type range;
	F f;
};

template<typename F,typename Iterator>
inline filter_range<F,Iterator> filter( Iterator&& first, Iterator&& last, F&& f ) {
	return filter_range<F,Iterator>( std::forward<F>(f),
		std::make_pair(
			std::forward<Iterator>(first),
			std::forward<Iterator>(last)
		)
	);
}

template<typename Range,typename F>
inline auto filter( Range&& r, F&& f ) {
	return filter(
		begin( std::forward<Range>(r) ),
		end( std::forward<Range>(r) ),
		std::forward<F>(f)
	);
}

template<typename Range,typename F>
inline auto cfilter( const Range& r, F&& f ) {
	return filter(
		cbegin( r ),
		cend( r ),
		std::forward<F>(f)
	);
}

#endif
