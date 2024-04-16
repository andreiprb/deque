#pragma once

#include "block.h"
#include <iostream>

template<typename T>
class Deque
{
private:
	Block<T>** header;

	short first_used_block, last_used_block;
	short first_index, last_index;

	short last_block;
	size_t count;
public:
	Deque();
	~Deque();

	void push_front(const T& value);
	void push_back(const T& value);

	void pop_front();
	void pop_back();

	void insert(const short& index, const T& value);
	void remove(const short& index);

	T front();
	T back();

	unsigned short size();

	bool empty();
	void clear();

	T& operator[] (unsigned short index);

	void print();
};

template<typename T>
inline Deque<T>::Deque()
{
	this->header = new Block<T>*[2];
	this->header[0] = new Block<T>;
	this->header[1] = new Block<T>;

	this->first_index = Block<T>::BLOCKSIZE;
	this->last_index = -1;

	this->first_used_block = 0;
	this->last_used_block = 1;

	this->last_block = 1;
	this->count = 0;
}

template<typename T>
Deque<T>::~Deque()
{
	for (int i = 0; i <= this->last_block; ++i)
		this->header[i]->~Block();

	delete[] this->header;
}

template<typename T>
void Deque<T>::push_front(const T& value)
{
	this->count++;

	if (--this->first_index == -1)
	{
		this->first_index = Block<T>::BLOCKSIZE - 1;
		this->first_used_block--;
	}

	if (this->first_used_block < 0)
	{
		this->first_used_block++;
		this->last_used_block++;
		this->last_block++;

		Block<T>** header = new Block<T>*[this->last_block + 1];

		for (int i = 0; i < this->last_block; ++i)
		{
			header[i + 1] = this->header[i];
			this->header[i] = nullptr;
		}

		delete[] this->header;
		this->header = header;
		this->header[0] = new Block<T>;
	}

	this->header[this->first_used_block]->data[this->first_index] = value;

	if (this->last_index == -1)
	{
		this->last_used_block = this->first_used_block;
		this->last_index = this->first_index;
	}
}

template<typename T>
void Deque<T>::push_back(const T& value)
{
	this->count++;

	if (++this->last_index == Block<T>::BLOCKSIZE)
	{
		this->last_index = 0;
		this->last_used_block++;
	}

	if (this->last_used_block > this->last_block)
	{
		this->last_block++;

		Block<T>** header = new Block<T>*[this->last_block + 1];

		for (int i = 0; i < this->last_block; ++i)
		{
			header[i] = this->header[i];
			this->header[i] = nullptr;
		}

		delete[] this->header;
		this->header = header;
		this->header[this->last_used_block] = new Block<T>;
	}

	this->header[this->last_used_block]->data[this->last_index] = value;

	if (this->first_index == Block<T>::BLOCKSIZE)
	{
		this->first_used_block = this->last_used_block;
		this->first_index = this->last_index;
	}
}

template<typename T>
void Deque<T>::pop_front()
{
	if (!this->count)
	{
		this->clear();
		return;
	}

	this->count--;

	if (++this->first_index == Block<T>::BLOCKSIZE)
	{
		this->first_index = 0;
		this->first_used_block++;
	}

	if (this->first_used_block > 1)
	{
		this->first_used_block--;
		this->last_used_block--;
		this->last_block--;

		Block<T>** header = new Block<T>*[this->last_block + 1];

		for (int i = 0; i < this->last_block + 1; ++i)
		{
			header[i] = this->header[i + 1];
			this->header[i + 1] = nullptr;
		}

		delete[] this->header[0]->data;
		delete[] this->header;
		this->header = header;
	}
}

template<typename T>
void Deque<T>::pop_back()
{
	if (!this->count)
	{
		this->clear();
		return;
	}

	this->count--;

	if (--this->last_index == -1)
	{
		this->last_index = Block<T>::BLOCKSIZE - 1;
		this->last_used_block--;
	}

	if (this->last_used_block < this->last_block - 1)
	{
		this->last_block--;

		Block<T>** header = new Block<T>*[this->last_block + 1];

		for (int i = 0; i < this->last_block; ++i)
		{
			header[i] = this->header[i];
			this->header[i] = nullptr;
		}

		delete[] this->header[this->last_block]->data;
		delete[] this->header;
		this->header = header;
	}
}

template<typename T>
void Deque<T>::insert(const short& index, const T& value)
{
	if (index < 0)
	{
		this->push_front(value);
		return;
	}

	if (index >= this->count)
	{
		this->push_back(value);
		return;
	}

	if (index < this->count / 2)
	{
		this->push_front((*this)[0]);

		for (int i = 1; i < index; ++i)
			(*this)[i] = (*this)[i + 1];
	}

	else
	{
		this->push_back((*this)[this->count]);

		for (int i = this->count - 1; i > index; --i)
			(*this)[i] = (*this)[i - 1];
	}

	(*this)[index] = value;
}

template<typename T>
void Deque<T>::remove(const short& index)
{
	if (index < 0 || index >= this->count)
		return;

	if (index < this->count / 2)
	{
		for (int i = index; i > 0; --i)
			(*this)[i] = (*this)[i - 1];

		this->pop_front();
	}

	else
	{
		for (int i = index; i < this->count; ++i)
			(*this)[i] = (*this)[i + 1];

		this->pop_back();
	}
}

template<typename T>
inline T Deque<T>::front() { return (*this)[0]; }

template<typename T>
inline T Deque<T>::back() { return (*this)[this->count]; }

template<typename T>
inline unsigned short Deque<T>::size() { return this->count; }

template<typename T>
inline bool Deque<T>::empty() { return !count; }

template<typename T>
inline void Deque<T>::clear()
{
	this->~Deque();

	this->header = new Block<T>*[2];
	this->header[0] = new Block<T>;
	this->header[1] = new Block<T>;

	this->first_index = Block<T>::BLOCKSIZE;
	this->last_index = -1;

	this->first_used_block = 0;
	this->last_used_block = 1;

	this->last_block = 1;
	this->count = 0;
}

template<typename T>
inline T& Deque<T>::operator[] (unsigned short index)
{
	unsigned short current_block = this->first_used_block;

	index += first_index;
	current_block += index / Block<T>::BLOCKSIZE;

	return this->header[current_block]->data[index % Block<T>::BLOCKSIZE];
}

template<typename T>
void Deque<T>::print()
{
	for (int i = 0; i < this->count; ++i)
		std::cout << (*this)[i] << " ";

	std::cout << std::endl;
}