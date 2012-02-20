
#include <stdio.h>
#include <cassert>

template<typename T>
glVector<T>::glVector()
{
	// Default values
	x = y = z = 0.0;
}

template<typename T>
glVector<T>::glVector(T _x, T _y, T _z)
{
	// Set the given values
	x = _x;
	y = _y;
	z = _z; 
}



template<typename T>
glVector<T> glVector<T>::operator+(const glVector<T> vec) const
{
	// Add value to value
	T tx = x + vec.x;
	T ty = y + vec.y;
	T tz = z + vec.z;
	return glVector<T>(tx, ty, tz);
}

template<typename T>
glVector<T> glVector<T>::operator-(const glVector<T> vec) const
{
	// Subtract value from value
	T tx = x - vec.x;
	T ty = y - vec.y;
	T tz = z - vec.z;
	return glVector<T>(tx, ty, tz);
}

template<typename T>
T glVector<T>::operator[](const int &index) const
{
	assert(index < 3);
	
	// Get the wanted value
	if(index == 0)
	{
		return x;
	}

	if(index == 1)
	{
		return y;
	}

	if(index == 2)
	{
		return z;
	}

	return 0;
}

template<typename T>
T glVector<T>::operator*(const glVector<T> vec) const
{
  // Calculate the result
  return (x * vec.x + y * vec.y + z * vec.z); 
}

template<typename T>
glVector<T> glVector<T>::operator*(T scale) const
{
	// Calculate the result
	T tx = x * scale;
	T ty = y * scale;
	T tz = z * scale;
	return glVector<float>(tx, ty, tz);
}

template<typename T>
void glVector<T>::operator+=(glVector<T> v)
{
	// Add value to value
	x += v.x;
	y += v.y;
	z += v.z;
}
