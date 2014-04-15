/*
 * Chris Welshman 2014
 */
#ifndef INCLUDED_PAIRS
#define INCLUDED_PAIRS
#include <iterator>
#include <utility>

template<typename Iterator>
struct pairs_iterator {
	typedef typename std::iterator_traits<Iterator>::value_type        original_value_type;
	typedef typename std::iterator_traits<Iterator>::reference         original_reference;
	typedef typename std::iterator_traits<Iterator>::pointer           original_pointer;
	typedef typename std::iterator_traits<Iterator>::difference_type   difference_type;
	typedef typename std::iterator_traits<Iterator>::iterator_category iterator_category;
	
	typedef std::pair<original_value_type,original_value_type> value_type;
	typedef std::pair<original_reference,original_reference>   reference;
	typedef std::pair<original_pointer,original_pointer>       pointer;
	typedef std::pair<Iterator,Iterator>                       pair_type;
	
	pair_type range, pair;

	pairs_iterator() = default;

	explicit pairs_iterator( const pair_type& range ) : range(range) {
		pair.first = range.first;
		pair.second = range.first;
	}

	pairs_iterator( const Iterator& first, const Iterator& last ) : pairs_iterator(pair_type(first,last)) {}
	
	pairs_iterator( const pair_type& range, const pair_type& pair ) : range(range), pair(pair) {}

	reference operator*() const {
		return reference( *pair.first, *pair.second );
	}

	original_reference first() const {
		return original_reference( *pair.first );
	}

	original_reference second() const {
		return original_reference( *pair.second );
	}

	pairs_iterator<Iterator>& operator++() {
		++pair.second;
		if( pair.second == range.second ) {
			++pair.first;
			pair.second = range.first;
		}
		return *this;
	}

	pairs_iterator<Iterator> operator++(int) {
		pairs_iterator<Iterator> temp = *this;
		++(*this);
		return temp;
	}

	pairs_iterator<Iterator>& operator--() {
		if( pair.second == range.first ) {
			pair.second = range.second;
			--pair.second;
			--pair.first;
		}
		return *this;
	}

	pairs_iterator<Iterator> operator--(int) {
		pairs_iterator<Iterator> temp = *this;
		--(*this);
		return temp;
	}

	pairs_iterator<Iterator>& operator+=( difference_type offset ) {
		difference_type N = std::distance( range.first, range.second );
		difference_type k = index(N) + offset;
		difference_type i = k / N;
		difference_type j = k % N;
		pair.first = range.first + i;
		pair.second = range.first + j;
		return *this;
	}

	pairs_iterator<Iterator> operator+( difference_type offset ) const {
		pairs_iterator<Iterator> temp = *this;
		return temp += offset;
	}

	pairs_iterator<Iterator>& operator-=( difference_type offset ) {
		return *this += -offset;
	}

	pairs_iterator<Iterator> operator-( difference_type offset ) const {
		pairs_iterator<Iterator> temp = *this;
		return temp -= offset;
	}

	difference_type operator-( const pairs_iterator<Iterator>& rhs ) const {
		difference_type dfirst = std::distance( rhs.pair.first, pair.first );
		difference_type dsecond = std::distance( rhs.pair.second, pair.second );
		difference_type N = std::distance( range.first, range.second );
		return dfirst * N + dsecond;
	}

	reference operator[]( difference_type offset ) const {
		return *(*this + offset);
	}

	difference_type index() const {
		difference_type N = std::distance( range.first, range.second );
		return index(N);
	}

	bool operator==( const pairs_iterator<Iterator>& rhs ) const {
		if( pair == rhs.pair )
			return true;
		return false;
	}

	bool operator!=( const pairs_iterator<Iterator>& rhs ) const {
		return !(*this == rhs);
	}

	bool operator<( const pairs_iterator<Iterator>& rhs ) const {
		return rhs - *this > 0;
	}

	bool operator>( const pairs_iterator<Iterator>& rhs ) const {
		return rhs < *this;
	}

	bool operator<=( const pairs_iterator<Iterator>& rhs ) const {
		return !( *this > rhs );
	}

	bool operator>=( const pairs_iterator<Iterator>& rhs ) const {
		return !( *this < rhs );
	}

protected:

	difference_type index( difference_type N ) const {
		return index(
			std::distance( range.first, pair.first ),
			std::distance( range.first, pair.second ),
			N
		);
	}

	difference_type index( difference_type i, difference_type j, difference_type N ) const {
		return N*i + j;
	}

};

template<typename Iterator>
struct pairs_wrapper {
	typedef typename std::iterator_traits<Iterator>::difference_type difference_type;
	typedef Iterator original_iterator;
	typedef Iterator original_const_iterator;
	typedef pairs_iterator<original_iterator>       iterator;
	typedef pairs_iterator<original_const_iterator> const_iterator;
	typedef std::reverse_iterator<iterator>         reverse_iterator;
	typedef std::reverse_iterator<const_iterator>   const_reverse_iterator;
	typedef std::pair<Iterator,Iterator>            pair_type;

	explicit pairs_wrapper( const pair_type& range ) : range(range) {}
	
	pairs_wrapper( const Iterator& first, const Iterator& last ) : range(pair_type(first,last)) {}
	
	difference_type size() const {
		difference_type N = std::distance( range.first, range.second );
		return N * N;
	}

	iterator begin() {
		return iterator( range );
	}

	iterator end() {
		return iterator(
			range,
			std::make_pair( range.second, range.first )
		);
	}

	const_iterator begin() const {
		return const_iterator( range );
	}

	const_iterator end() const {
		return const_iterator(
			range,
			std::make_pair( range.second, range.first )
		);
	}

protected:
	pair_type range;
};

template<typename Iterator>
inline pairs_wrapper<Iterator> pairs( const std::pair<Iterator,Iterator>& range ) {
	return pairs_wrapper<Iterator>(range);
}

template<typename Iterator>
inline pairs_wrapper<Iterator> pairs( Iterator&& first, Iterator&& last ) {
	return pairs_wrapper<Iterator>(
		std::make_pair(
			std::forward<Iterator>(first),
			std::forward<Iterator>(last)
		)
	);
}

template<typename Container>
inline auto pairs( Container&& c ) {
	return pairs(
		begin( std::forward<Container>(c) ),
		end( std::forward<Container>(c) )
	);
}

template<typename Container>
inline auto cpairs( const Container& c ) {
	return pairs(
		cbegin(c),
		cend(c)
	);
}

#endif
