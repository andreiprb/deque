#pragma once

template<typename T>
class Block
{
public:
	const static int BLOCKSIZE;
	T* data = nullptr;

	Block();
	~Block();
};

template<typename T>
const int Block<T>::BLOCKSIZE = (int)(512 / sizeof(T));

template<typename T>
inline Block<T>::Block() { this->data = new T[this->BLOCKSIZE]; }

template<typename T>
inline Block<T>::~Block() { if (this->data != nullptr) delete[] this->data; }
