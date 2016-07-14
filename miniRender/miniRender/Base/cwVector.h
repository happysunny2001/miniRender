/*
Copyright © 2015-2016 Ziwei Wang (happy.sunny.2001@163.com)

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
associated documentation files (the “Software”), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or
substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef __CW_VECTOR_H__
#define __CW_VECTOR_H__

#include "cwMacros.h"
#include "cwBasicType.h"
#include <assert.h>
#include <vector>
#include <functional>
#include <algorithm>

NS_MINIR_BEGIN

template<class T>
class CW_DLL cwVector
{
public:
	typedef typename std::vector<T>::iterator iterator;
	typedef typename std::vector<T>::const_iterator const_iterator;

	typedef typename std::vector<T>::reverse_iterator reverse_iterator;
	typedef typename std::vector<T>::const_reverse_iterator const_reverse_iterator;

	iterator begin() { return m_nData.begin(); }
	iterator end()   { return m_nData.end();   }

	const_iterator begin() const { return m_nData.begin(); }
	const_iterator end() const   { return m_nData.end();   }

	reverse_iterator rbegin() { return m_nData.rbegin(); }
	reverse_iterator rend()   { return m_nData.rend();   }

	const_reverse_iterator rbegin() const { return m_nData.crbegin(); }
	const_reverse_iterator rend() const   { return m_nData.crend();   }

	cwVector<T>() : m_nData() {
		static_assert(std::is_convertible<T, cwRef*>::value, "Invalid Type for minir::cwVector<K, V>!");
	}

	//copy constructor
	cwVector<T>(const cwVector<T>& V) {
		static_assert(std::is_convertible<T, cwRef*>::value, "Invalid Type for minir::cwVector<K, V>!");
		m_nData = V.m_nData;
		addRef();
	}

	//move constructor
	cwVector<T>(cwVector<T>&& V) {
		static_assert(std::is_convertible<T, cwRef*>::value, "Invalid Type for minir::cwVector<K, V>!");
		m_nData = std::move(V.m_nData);
	}

	~cwVector<T>() {
		clear();
	}

	//copy assignment
	cwVector<T>& operator = (const cwVector<T>& V) {
		if (this != &V) {
			clear();
			m_nData = V.m_nData;
			addRef();
		}

		return *this;
	}

	//move assignment
	cwVector<T>& operator = (cwVector<T>&& V) {
		if (this != &V) {
			clear();
			m_nData = std::move(V.m_nData);
		}

		return *this;
	}

	void reserve(CWUINT64 n) {
		m_nData.reserve(n);
	}

	void resize(CWUINT64 n) {
		m_nData.resize(n);
	}

	CWUINT64 capacity() const {
		return m_nData.capacity();
	}

	CWUINT64 size() const {
		return m_nData.size();
	}

	bool empty() const {
		return m_nData.empty();
	}

	CWUINT64 max_size() const {
		return m_nData.max_size();
	}

	CWUINT64 getIndex(T object) const {
		auto it = std::find(m_nData.begin(), m_nData.end(), object);
		if (it != m_nData.end())
			return it - m_nData.begin();
		return -1;
	}

	iterator find(T object) {
		return std::find(m_nData.begin(), m_nData.end(), object);
	}

	const_iterator find(T object) const {
		return std::find(m_nData.begin(), m_nData.end(), object);
	}

	T at(CWUINT64 index) const {
		assert(index<m_nData.size());
		return m_nData[index];
	}

	T front() const {
		return m_nData.front();
	}

	T back() const {
		return m_nData.back();
	}

	bool contains(T object) const {
		return (std::find(m_nData.begin(), m_nData.end(), object) != m_nData.end());
	}

	bool equal(const cwVector<T>& V) const {
		if (this->size() != V.size()) return false;

		for (CWUINT64 i = 0; i < this->size(); ++i) {
			if (this->at(i) != V.at(i)) return false;
		}
		return true;
	}

	void pushBack(T object) {
		m_nData.push_back(object);
		if (object)
			object->retain();
	}

	void pushBack(const cwVector<T>& V) {
		for (const auto& obj : V) {
			m_nData.push_back(obj);
			if (obj)
				obj->retain();
		}
	}

	void insert(CWUINT64 index, T object) {
		assert(index <= m_nData.size());
		m_nData.insert(std::begin(m_nData) + index, object);
		if (object)
			object->retain();
	}

	void popBack() {
		if (m_nData.empty()) return;
		auto last = this->back();
		m_nData.pop_back();
		if (last)
			last->release();
	}

	void erase(T object, bool bRemoveAll = false) {
		assert(object != nullptr);
		if (bRemoveAll) {
			for (auto it = m_nData.begin(); it != m_nData.end(); ++it) {
				if ((*it) == object) {
					it = m_nData.erase(it);
					object->release();
				}
				else {
					++it;
				}
			}
		}
		else {
			auto it = std::find(m_nData.begin(), m_nData.end(), object);
			if (it != m_nData.end()) {
				m_nData.erase(it);
				object->release();
			}
		}
	}

	iterator erase(iterator pos) {
		assert(pos >= m_nData.begin() && pos < m_nData.end());
		(*pos)->release();
		return m_nData.erase(pos);
	}

	iterator erase(iterator first, iterator last) {
		for (auto it = first; it != last; ++it) {
			(*it)->release();
		}
		return m_nData.erase(first, last);
	}

	iterator erase(CWUINT64 index) {
		assert(index < m_nData.size());
		auto it = std::next(m_nData.begin(), index);
		(*it)->release();
		return m_nData.erase(it);
	}

	void swap(CWUINT64 index1, CWUINT64 index2) {
		assert(index1 < m_nData.size() && index2 < m_nData.size());
		std::swap(m_nData[index1], m_nData[index2]);
	}

	void replace(T object, CWUINT64 index) {
		assert(index < m_nData.size());
		if (m_nData[index])
			m_nData[index]->release();
		m_nData[index] = object;
		if (object)
			object->retain();
	}

	void reverse() {
		std::reverse(m_nData.begin(), m_nData.end());
	}

	void shrinkToFit() {
		m_nData.shrink_to_fit();
	}

	void clear() {
		if (m_nData.empty()) return;
		for (auto it = m_nData.begin(); it != m_nData.end(); ++it) {
			if (*it)
				(*it)->release();
		}

		m_nData.clear();
	}

private:
	void addRef(){
		for (auto it = m_nData.begin(); it != m_nData.end(); ++it) {
			(*it)->retain();
		}
	}

private:
	std::vector<T> m_nData;
};

NS_MINIR_END

#endif