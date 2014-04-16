/*
 * Chris Welshman 2014
 */
#ifndef INCLUDED_ZIP
#define INCLUDED_ZIP
#include <utility>
#include <iterator>
#include <algorithm>

template<typename It1,typename It2>
struct zip_iterator {
	typedef typename std::iterator_traits<It1>::value_type        value_type_1;
	typedef typename std::iterator_traits<It1>::reference         reference_1;
	typedef typename std::iterator_traits<It1>::pointer           pointer_1;
	typedef typename std::iterator_traits<It1>::difference_type   difference_type_1;
	typedef typename std::iterator_traits<It1>::iterator_category iterator_category_1;

	typedef typename std::iterator_traits<It2>::value_type        value_type_2;
	typedef typename std::iterator_traits<It2>::reference         reference_2;
	typedef typename std::iterator_traits<It2>::pointer           pointer_2;
	typedef typename std::iterator_traits<It2>::difference_type   difference_type_2;
	typedef typename std::iterator_traits<It2>::iterator_category iterator_category_2;
		
	typedef std::pair<value_type_1,value_type_2> value_type;
	typedef std::pair<reference_1,reference_2>   reference;
	typedef std::pair<pointer_1,pointer_2>       pointer;
	typedef std::pair<It1,It2>                   pair_type;

	typedef difference_type_1   difference_type;
	typedef iterator_category_1 iterator_category;

	zip_iterator() = default;

	explicit zip_iterator( const pair_type& pair ) : pair(pair) {}

	zip_iterator( const It1& it1, const It2& it2 ) : zip_iterator(pair_type(it1,it2)) {}

	reference operator*() const {
		return reference( *pair.first, *pair.second );
	}

	reference_1 first() const {
		return reference_1( *pair.first );
	}

	reference_2 second() const {
		return reference_2( *pair.second );
	}

	zip_iterator<It1,It2>& operator++() {
		++pair.first;
		++pair.second;
		return *this;
	}

	zip_iterator<It1,It2> operator++(int) {
		zip_iterator<It1,It2> temp = *this;
		++(*this);
		return temp;
	}

	zip_iterator<It1,It2>& operator--() {
		--pair.first;
		--pair.second;
		return *this;
	}

	zip_iterator<It1,It2> operator--(int) {
		zip_iterator<It1,It2> temp = *this;
		--(*this);
		return temp;
	}

	zip_iterator<It1,It2>& operator+=( difference_type offset ) {
		pair.first += offset;
		pair.second += offset;
		return *this;
	}

	zip_iterator<It1,It2> operator+( difference_type offset ) const {
		zip_iterator<It1,It2> temp = *this;
		return temp += offset;
	}

	zip_iterator<It1,It2>& operator-=( difference_type offset ) {
		return *this += -offset;
	}

	zip_iterator<It1,It2> operator-( difference_type offset ) const {
		zip_iterator<It1,It2> temp = *this;
		return temp -= offset;
	}

	difference_type operator-( const zip_iterator<It1,It2>& rhs ) {
		return std::distance( rhs.pair.first, pair.first );
	}

	reference operator[]( difference_type offset ) const {
		return *(*this + offset);
	}

	bool operator==( const zip_iterator<It1,It2>& rhs ) const {
		if( pair == rhs.pair )
			return true;
		return false;
	}

	bool operator!=( const zip_iterator<It1,It2>& rhs ) const {
		return !(*this == rhs);
	}

	bool operator<( const zip_iterator<It1,It2>& rhs ) const {
		return rhs - *this > 0;
	}

	bool operator>( const zip_iterator<It1,It2>& rhs ) const {
		return rhs < *this;
	}

	bool operator<=( const zip_iterator<It1,It2>& rhs ) const {
		return !( *this > rhs );
	}

	bool operator>=( const zip_iterator<It1,It2>& rhs ) const {
		return !( *this < rhs );
	}

protected:
	pair_type pair; 
};

template<typename It1,typename It2>
struct zip_wrapper {
	typedef typename std::iterator_traits<It1>::difference_type difference_type_1;
	typedef typename std::iterator_traits<It2>::difference_type difference_type_2;
	typedef typename std::common_type<difference_type_1,difference_type_2>::type difference_type;
	typedef It1 iterator_1;
	typedef It1 const_iterator_1;
	typedef It2 iterator_2;
	typedef It2 const_iterator_2;
	typedef zip_iterator<iterator_1,iterator_2>             iterator;
	typedef zip_iterator<const_iterator_1,const_iterator_2> const_iterator;
	typedef std::reverse_iterator<iterator>                 reverse_iterator;
	typedef std::reverse_iterator<const_iterator>           const_reverse_iterator;
	typedef std::pair<It1,It1>                              pair_type_1;
	typedef std::pair<It2,It2>                              pair_type_2;
	typedef std::pair<pair_type_1,pair_type_2>              range_type;

	explicit zip_wrapper( const range_type& range ) : range(range) {}
		
	zip_wrapper( const pair_type_1& range_1, const pair_type_2& range_2 ) : range(range_type(range_1,range_2)) {}
		
	difference_type size() const {
		difference_type N1 = std::distance( range.first.first, range.first.second );
		difference_type N2 = std::distance( range.second.first, range.second.second );
		return std::min( N1, N2 );
	}

	iterator begin() {
		return iterator( range.first.first, range.second.first );
	}

	iterator end() {
		return iterator( range.first.second, range.second.first + size() );
	}

	const_iterator begin() const {
		return const_iterator( range.first.first, range.second.first );
	}

	const_iterator end() const {
		return const_iterator( range.first.second, range.second.first + size() );
	}

protected:
	range_type range;
};

template<typename It1,typename It2>
inline zip_wrapper<It1,It2> zip( It1&& first_1, It1&& last_1, It2&& first_2, It2&& last_2 ) {
	return zip_wrapper<It1,It2>(
		std::make_pair(
			std::forward<It1>(first_1),
			std::forward<It1>(last_1)
		),
		std::make_pair(
			std::forward<It2>(first_2),
			std::forward<It2>(last_2)
		)
	);
}

template<typename C1,typename C2>
inline auto zip( C1&& c1, C2&& c2 ) {
	return zip(
		begin( std::forward<C1>(c1) ),
		end( std::forward<C1>(c1) ),
		begin( std::forward<C2>(c2) ),
		end( std::forward<C2>(c2) )
	);
}

template<typename C1,typename C2>
inline auto czip( C1&& c1, C2&& c2 ) {
	return zip(
		cbegin( std::forward<C1>(c1) ),
		cend( std::forward<C1>(c1) ),
		cbegin( std::forward<C2>(c2) ),
		cend( std::forward<C2>(c2) )
	);
}

#endif
