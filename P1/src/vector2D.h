#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <iostream>

/**
 * Vector bidimensional genérico.
 */
template<typename T = float>
class Vector2D {
	T x, y;

public:
	Vector2D(T x, T y) : x(x), y(y) { }
	Vector2D() : Vector2D(0, 0) { }

	// Coordenadas del vector
	T getX() const { return x; }
	T getY() const { return y; }

    // Operadores aritméticos
    Vector2D operator+(const Vector2D& otro) const { return { x + otro.x, y + otro.y }; }
    Vector2D operator-(const Vector2D& otro) const { return { x - otro.x, y - otro.y }; }

    // Producto escalar
    T operator*(const Vector2D& otro) const { return x * otro.x + y * otro.y; }

    // Producto por escalar
    Vector2D operator*(T escalar) const { return { x * escalar, y * escalar }; }
    friend Vector2D operator*(T escalar, const Vector2D& v) { return v * escalar; }

    // Operadores compuestos (opcional)
    Vector2D& operator+=(const Vector2D& otro) { x += otro.x; y += otro.y; return *this; }
    Vector2D& operator-=(const Vector2D& otro) { x -= otro.x; y -= otro.y; return *this; }
    Vector2D& operator*=(T escalar) { x *= escalar; y *= escalar; return *this; }

    // Operadores de salida
    friend std::ostream& operator<<(std::ostream& out, const Vector2D& v) {
        return out << '{' << v.x << ", " << v.y << '}';
    }
};

// Alias para puntos
template<typename T = float>
using Point2D = Vector2D<T>;

#endif // VECTOR2D_H
