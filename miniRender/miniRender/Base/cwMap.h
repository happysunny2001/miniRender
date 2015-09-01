/*
Copyright © 2015 Ziwei Wang

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

#ifndef __CW_MAP_H__
#define __CW_MAP_H__

#include "cwMacros.h"
#include <assert.h>
#include <unordered_map>

NS_MINIR_BEGIN

template<class K, class V>
class cwMap
{
public:
	typedef std::unordered_map<K, V> RefMap;
	typedef typename RefMap::iterator iterator;
	typedef typename RefMap::const_iterator const_iterator;

	iterator begin() { return m_nData.begin(); }
	iterator end() { return m_nData.end(); }
	const_iterator begin() const { return m_nData.begin(); }
	const_iterator end() const { return m_nData.end(); }

	cwMap<K, V>() : m_nData() {
		static_assert(std::is_convertible<V, cwRef*>::value, "Invalid Type for apoca::cwMap<K, V>!");
	}

	~cwMap<K, V>() {
		clear();
	}

	iterator find(const K& key) {
		return m_nData.find(key);
	}

	const_iterator find(const K& key) const {
		return m_nData.find(key);
	}

	bool empty() const  { return m_nData.empty(); }
	size_t size() const { return m_nData.size(); }

	size_t erase(const K& key) {
		iterator it = find(key);
		if (it != m_nData.end()) {
			CW_SAFE_RELEASE(it->second);
			m_nData.erase(it);
			return 1;
		}
		return 0;
	}

	void insert(const K& key, V object) {
		assert(object != nullptr);
		erase(key);
		m_nData.insert(std::make_pair(key, object));
		object->retain();
	}

private:
	void addRef(){
		for (auto it = m_nData.begin(); it != m_nData.end(); ++it) {
			it->second->retain();
		}
	}

	void clear() {
		for (auto it = m_nData.begin(); it != m_nData.end(); ++it) {
			CW_SAFE_RELEASE(it->second);
		}
		m_nData.clear();
	}

private:
	RefMap m_nData;

};

NS_MINIR_END

#endif
