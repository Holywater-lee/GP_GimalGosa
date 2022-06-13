#pragma once 

#include <math.h>

class Vector2D {
public:
    Vector2D(float x, float y) : m_x(x) , m_y(y) {}
    float getX() { return m_x; }
    float getY() { return m_y; }
    void setX(float x) { m_x = x; }
    void setY(float y) { m_y = y; }
    void limit(float max) // 추가: max 값을 넘었다면 길이를 max 값으로 보정
    {
      float l = length();
      if (l > 0)
      {
        if (l > max)
        {
          (*this) *= 1 / l;
          (*this) *= max;
        }
      }
    }

    float length() { return sqrt(m_x * m_x + m_y * m_y); }
    float lengthSqr() { return m_x * m_x + m_y * m_y; }

    Vector2D Normalized()
    {
        float l = length();
        if (l > 0)
        {
            return Vector2D((*this).m_x / l, (*this).m_y / l);
        }

        return Vector2D(0, 0);
    }

    Vector2D Rotate(float radian)
    {
        return Vector2D((*this).m_x * cos(radian) - (*this).m_y * sin(radian), (*this).m_x * sin(radian) + (*this).m_y * cos(radian));
    }

    Vector2D operator+(const Vector2D& v2) const
    {
        return Vector2D(m_x + v2.m_x, m_y + v2.m_y);
    }

    friend Vector2D& operator+=(Vector2D& v1, const Vector2D& v2)
    {
        v1.m_x += v2.m_x;
        v1.m_y += v2.m_y;
        return v1; 
    }

    Vector2D operator-(const Vector2D& v2) const
    {
        return Vector2D(m_x - v2.m_x, m_y - v2.m_y);
    }

    friend Vector2D& operator-=(Vector2D& v1, const Vector2D& v2)
    {
        v1.m_x -= v2.m_x;
        v1.m_y -= v2.m_y;
        return v1; 
    }

    Vector2D operator*(float scalar)
    {
        return Vector2D(m_x * scalar, m_y * scalar);
    }

    Vector2D& operator*=(float scalar)
    {
        m_x *= scalar;
        m_y *= scalar;
        return *this;
    }
    
    Vector2D operator/(float scalar)
    {
        return Vector2D(m_x / scalar, m_y / scalar);
    }

    Vector2D& operator/=(float scalar)
    {
        m_x /= scalar;
        m_y /= scalar;
        return *this;
    }
    void normalize()
    {
        float l = length();
        if (l > 0)   // we never want to attempt to divide by 0
        {
            (*this) *= 1 / l;
        }
    }

private:
    float m_x;
    float m_y;
};