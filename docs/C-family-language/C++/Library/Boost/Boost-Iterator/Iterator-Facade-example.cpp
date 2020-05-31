/*
 * example.cpp
 *
 */

#include <iostream>
# include <boost/iterator/iterator_facade.hpp>
struct node_base {
	node_base() :
			m_next(0) {
	}
	// Each node manages all of its tail nodes
	virtual ~node_base() {
		delete m_next;
	}
	// Access the rest of the list
	node_base* next() const {
		return m_next;
	}
	// print to the stream
	virtual void print(std::ostream& s) const = 0;
	// double the value
	virtual void double_me() = 0;
	void append(node_base* p) {
		if (m_next)
			m_next->append(p);
		else
			m_next = p;
	}
private:
	node_base* m_next;
};

template<class T>
struct node: node_base {
	node(T x) :
			m_value(x) {
	}
	void print(std::ostream& s) const {
		s << this->m_value;
	}
	void double_me() {
		m_value += m_value;
	}
private:
	T m_value;
};

inline std::ostream& operator<<(std::ostream& s, node_base const& n) {
	n.print(s);
	return s;
}

class node_iterator: public boost::iterator_facade<node_iterator, node_base,
		boost::forward_traversal_tag> {
public:
	node_iterator() :
			m_node(0) {
	}
	explicit node_iterator(node_base* p) :
			m_node(p) {
	}
private:
	friend class boost::iterator_core_access;
	void increment() {
		m_node = m_node->next();
	}
	bool equal(node_iterator const& other) const {
		return this->m_node == other.m_node;
	}
	node_base& dereference() const {
		return *m_node;
	}

	node_base* m_node;
};


