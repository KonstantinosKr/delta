/*
 The MIT License (MIT)

 Copyright (c) 2022 Konstantinos Krestenitis

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#ifndef DELTA_DEM2D_VEC2_H_
#define DELTA_DEM2D_VEC2_H_

#include <cmath>

#if !defined(iREAL)
#define iREAL double
#endif

namespace delta {
  namespace dem2d {

/*
 * Minimal 2D vector. Value type, no allocator, no proxy objects - it is the
 * cheapest thing that carries a plane point and keeps the contact code
 * readable. Not a general linear-algebra type.
 */
struct Vec2
{
  iREAL x;
  iREAL y;

  Vec2() : x(0.0), y(0.0) {}
  Vec2(iREAL x_, iREAL y_) : x(x_), y(y_) {}
};

inline Vec2 operator+(const Vec2& a, const Vec2& b) { return Vec2(a.x + b.x, a.y + b.y); }
inline Vec2 operator-(const Vec2& a, const Vec2& b) { return Vec2(a.x - b.x, a.y - b.y); }
inline Vec2 operator-(const Vec2& a) { return Vec2(-a.x, -a.y); }
inline Vec2 operator*(iREAL s, const Vec2& a) { return Vec2(s * a.x, s * a.y); }
inline Vec2 operator*(const Vec2& a, iREAL s) { return Vec2(s * a.x, s * a.y); }
inline Vec2 operator/(const Vec2& a, iREAL s) { return Vec2(a.x / s, a.y / s); }

inline Vec2& operator+=(Vec2& a, const Vec2& b) { a.x += b.x; a.y += b.y; return a; }
inline Vec2& operator-=(Vec2& a, const Vec2& b) { a.x -= b.x; a.y -= b.y; return a; }
inline Vec2& operator*=(Vec2& a, iREAL s) { a.x *= s; a.y *= s; return a; }

inline iREAL dot(const Vec2& a, const Vec2& b) { return a.x * b.x + a.y * b.y; }
/* 2D scalar cross product, z = a x b */
inline iREAL cross(const Vec2& a, const Vec2& b) { return a.x * b.y - a.y * b.x; }
inline iREAL lengthSqr(const Vec2& a) { return a.x * a.x + a.y * a.y; }
inline iREAL length(const Vec2& a) { return std::sqrt(a.x * a.x + a.y * a.y); }

inline Vec2 normalized(const Vec2& a)
{
  const iREAL len = length(a);
  return len > 0.0 ? Vec2(a.x / len, a.y / len) : Vec2(0.0, 0.0);
}

/* Rotate +90 degrees: perp(r) is what an angular velocity w acts through,
 * v = w * perp(r) = (-w*r.y, w*r.x). */
inline Vec2 perp(const Vec2& a) { return Vec2(-a.y, a.x); }

/* Rotate by an explicit cos/sin pair, which the callers already have. */
inline Vec2 rotate(const Vec2& a, iREAL c, iREAL s) { return Vec2(c * a.x - s * a.y, s * a.x + c * a.y); }

    }
  }

#endif /* DELTA_DEM2D_VEC2_H_ */
