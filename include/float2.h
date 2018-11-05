#ifndef FLOAT_2_H_
#define FLOAT_2_H_

#include <math.h>

class Float2 {
public:
  float x;
  float y;

  inline Float2();

  inline Float2(const float x, const float y);

  //Copy constructor
  inline Float2(const Float2& o);

  inline Float2& operator += (const Float2 &f);

  inline Float2& operator -= (const Float2 &f);

  inline Float2& operator *= (const Float2 &f);

  inline Float2& operator *= (float f);

  inline Float2& operator /= (const Float2 &f);

  inline Float2& operator /= (float f);

  inline float Length() const;

  inline float DotProduct(const Float2 &value) const;

};


inline Float2::Float2() {
  this->x = 0;
  this->y = 0;
}

inline Float2::Float2(const float x, const float y) {
  this->x = x;
  this->y = y;
}

inline Float2::Float2(const Float2& o) {
  this->x = o.x;
  this->y = o.y;
}

inline Float2& Float2::operator += (const Float2 &f) {
  this->x += f.x;
  this->y += f.y;
  return *this;
}

inline Float2& Float2::operator -= (const Float2 &f) {
  this->x -= f.x;
  this->y -= f.y;
  return *this;
}

inline Float2& Float2::operator *= (const Float2 &f) {
  this->x *= f.x;
  this->y *= f.y;
  return *this;
}

inline Float2& Float2::operator *= (float f) {
  this->x *= f;
  this->y *= f;
  return *this;
}

inline Float2& Float2::operator /= (const Float2 &f) {
  this->x /= f.x;
  this->y /= f.y;
  return *this;
}

inline Float2& Float2::operator /= (float f) {
  this->x /= f;
  this->y /= f;
  return *this;
}

inline float Float2::Length() const {
  return sqrtf((this->x * this->x) + (this->y * this->y));
}

inline float Float2::DotProduct(const Float2 &value) const {
  return (this->x*value.x) + (this->y*value.y);
}

inline Float2 operator+(const Float2 &valueA, const Float2 &valueB) {
  Float2 result;
  result.x = valueA.x + valueB.x;
  result.y = valueA.y + valueB.y;
  return result;
}

inline Float2 operator-(const Float2 &valueA, const Float2 &valueB) {
  Float2 result;
  result.x = valueA.x - valueB.x;
  result.y = valueA.y - valueB.y;
  return result;
}

inline Float2 operator*(const Float2 &valueA, const Float2 &valueB) {
  Float2 result;
  result.x = valueA.x * valueB.x;
  result.y = valueA.y * valueB.y;
  return result;
}

inline Float2 operator*(const Float2 &valueA, float valueB) {
  Float2 result;
  result.x = valueA.x * valueB;
  result.y = valueA.y * valueB;
  return result;
}

inline Float2 operator/(const Float2 &valueA, const Float2 &valueB) {
  Float2 result;
  result.x = valueA.x / valueB.x;
  result.y = valueA.y / valueB.y;
  return result;
}

inline Float2 operator/(const Float2 &valueA, float valueB) {
  Float2 result;
  result.x = valueA.x / valueB;
  result.y = valueA.y / valueB;
  return result;
}

inline bool operator==(const Float2 &valueA, const Float2 &valueB) {
  return (valueA.x == valueB.x && valueA.y == valueB.y);
}

inline bool operator!=(const Float2 &op1, const Float2 &op2) {
  return (op1.x != op2.x || op1.y != op2.y);
}

#endif