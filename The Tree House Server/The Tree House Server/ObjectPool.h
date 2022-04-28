#pragma once
#include <type_traits>
#include <concurrent_queue.h>



template<typename T>
class cObjectPool
{
public:
	cObjectPool(int _max_size)
	{
		m_max_size = _max_size;

		for (int i = 0; i < m_max_size; ++i)
		{
			T* empty_object = new T();
			m_objects.push(empty_object);
		}
	}
	~cObjectPool() 
	{
		while (m_objects.empty() == true)
		{
			T* object;
			if (m_objects.try_pop(object) == true)
				delete object;
		}
	
		m_max_size = 0;
	};

	T* PopObject()
	{
		T* object=nullptr;

		//다썼으면 정해진 만큼 미리 확장
		if (m_objects.try_pop(object) == false)
		{
			ExpandPool(5);
			return PopObject();
		}

		return object;
	}
	void ReturnObject(T* _object)
	{
		m_objects.push(_object);
	}

	void ExpandPool(int _size)
	{
		for (int i = 0; i < _size; ++i)
		{
			T* empty_object = new T();
			m_objects.push(empty_object);
		}
		std::cout << "Object Pool Expaned [ " << m_max_size << " ] -> [ " 
			<< m_max_size + _size << " ]\n";

		m_max_size += _size;
	}

private:
	Concurrency::concurrent_queue<T*> m_objects;
	std::atomic<int>     m_max_size;

};


