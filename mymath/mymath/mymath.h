#pragma once

#ifndef _MYMATH_H_
#define _MYMATH_H_

#include "math.h"
#include <algorithm>

#define PI 3.141592654f

namespace mymath {

	template < typename T >
	class Quaternion;

	template < typename T, int len >
	class VectorN {
	public:

		VectorN(T x, T y, T z) {
			data[0] = x;
			data[1] = y;
			data[2] = z;
		}
		VectorN() {
			for (int i = 0; i < len; i++) {
				data[i] = 0;
			}
		}
		/*
				VectorN(T v) {
					for (int i = 0; i < len; i++) {
						data[i] = v;
					}
				}
		*/
		VectorN(const VectorN& vectorN) {
			assign(vectorN);
		}

		void assign(const VectorN& vectorN) {

			for (int i = 0; i < len; i++) {
				data[i] = vectorN.data[i];
			}

		}

		const VectorN operator*(T v) const {

			VectorN ret = *this;

			for (int i = 0; i < len; i++) {
				ret.data[i] *= v;
			}

			return ret;
		}

		const VectorN& operator*=(const T& v) {

			for (int i = 0; i < len; i++) {
				data[i] *= v;
			}

			return *this;
		}

		const VectorN& operator+=(const T& v) {

			for (int i = 0; i < len; i++) {
				data[i] += v;
			}

			return *this;
		}

		T& operator[](const int h) {
			return data[h];
		}

		const VectorN<T, len> operator/(const T& v) {

			VectorN<T, len> ret;
			for (int i = 0; i < len; i++) {
				ret[i] = (*this)[i] / v;
			}

			return ret;
		}

		const VectorN<T, len> operator+(const VectorN<T, len>& that) const {

			VectorN<T, len> ret = *this;
			for (int i = 0; i < len; i++) {
				ret.data[i] += that.data[i];
			}

			return ret;
		}

		const VectorN<T, len> operator-(const T& v) const {

			VectorN<T, len> ret = this;
			for (int i = 0; i < len; i++) {
				ret -= v;
			}

			return ret;
		}

		const VectorN<T, len> operator-(const VectorN<T, len>& that) const {

			VectorN<T, len> ret = *this;
			for (int i = 0; i < len; i++) {
				ret.data[i] -= that.data[i];
			}

			return ret;
		}

		// Dot Product
		const T operator*(const VectorN<T, len>& that) {

			T ret = 0;
			for (int i = 0; i < len; i++) {
				ret += data[i] * that.data[i];
			}

			return ret;
		}

		VectorN<T, len> cross(VectorN<T, len>& that) {

			VectorN<T, len> crossed;

			for (int i = 0; i < len; i++) {
				crossed[i] = (*this)[(i + 1) % len] * that[(i + 2) % len]
					- that[(i + 1) % len] * (*this)[(i + 2) % len];
			}

			return crossed;
		}

		T length() {
			return sqrtf((*this) * (*this));
		}

		VectorN normalize() {
			T t = length();
			if (t == 0.0f) {
				return (*this);
			}

			(*this) = (*this) / t;
			return (*this);
		}

	protected:
		T data[len];
	};

	template< typename T >
	class Vector2 : public VectorN <T, 2 > {
		typedef VectorN<T, 2> base;
	public:
		Vector2() {
			base::data[0] = 0;
			base::data[1] = 0;
		}
		Vector2(const base& that) : base(that) {
		}

		Vector2(T u, T v) {
			base::data[0] = u;
			base::data[1] = v;
		}

	};

	typedef Vector2<float> Vectorf2;

	template< typename T >
	class Vector3 : public VectorN <T, 3 > {
		typedef VectorN <T, 3 > base;
		typedef Vector3<T> mytype;

	public:
		Vector3(T x, T y, T z) {
			base::data[0] = x;
			base::data[1] = y;
			base::data[2] = z;
		}

		Vector3(const base& that) : base(that) {
		}

		Vector3() {
			base::data[0] = 0;
			base::data[1] = 0;
			base::data[2] = 0;
		}

		T getX() const { return base::data[0]; }
		T getY() const { return base::data[1]; }
		T getZ() const { return base::data[2]; }

		void setX(const T& v) { base::data[0] = v; }
		void setY(const T& v) { base::data[1] = v; }
		void setZ(const T& v) { base::data[2] = v; }
		/*
				// Duplicate.
				const Vector3 cross(const Vector3& that) {

					Vector3 ret = {
						base::data[1] * that.data[2] - base::data[2] * that.data[1],
						base::data[2] * that.data[0] - base::data[0] * that.data[2],
						base::data[0] * that.data[1] - base::data[1] * that.data[0],
					};

					return ret;
				}
		*/
	};

	typedef Vector3<float> Vectorf3;
	typedef Vector3<int> Vectori3;

	template< typename T >
	class Vector4 : public VectorN <T, 4 > {
		typedef VectorN <T, 4 > base;

	public:
		Vector4(T x, T y, T z, T w) {
			base::data[0] = x;
			base::data[1] = y;
			base::data[2] = z;
			base::data[3] = w;
		}

		Vector4(const base& that) : base(that) {
		}

		Vector4(const Vectorf3& that) {
			Vectorf3 myThat = that;

			base::data[0] = myThat[0];
			base::data[1] = myThat[1];
			base::data[2] = myThat[2];
			base::data[3] = 1;
		}

		Vector4() {
			base::data[0] = 0;
			base::data[1] = 0;
			base::data[2] = 0;
			base::data[3] = 0;
		}

		Vectorf3 homogeniousDivide() {

			T w = base::data[3];

			if (w != 0.0f) {

				Vectorf3 ret = { base::data[0] / w, base::data[1] / w, base::data[2] / w };
				return ret;
			}
			else {

				Vectorf3 ret = { base::data[0] , base::data[1] , base::data[2] };
				return ret;
			}
		}
	};

	typedef Vector4<float> Vectorf4;

	/*
	template< typename T >
	const T operator*(const T& v1, const T& v2) {

		return v1 * v2;
	}
	*/

	template< typename T >
	const VectorN<T, 3> operator*(const T& v, const VectorN<T, 3>& that) {

		VectorN<T, 3> ret = that;

		for (int i = 0; i < 3; i++) {
			ret[i] = v * ret[i];
		}

		return ret;
	}

	template< typename T >
	const VectorN<T, 4> operator*(const T& v, const VectorN<T, 4>& that) {

		VectorN<T, 4> ret = that;

		for (int i = 0; i < 4; i++) {
			ret[i] = v * ret[i];
		}

		return ret;
	}







	template< typename T, int h, int w >
	class MatrixNM {

	public:
		typedef class VectorN<T, h> vector_type;

		MatrixNM() {
			vector_type zero;
			for (int i = 0; i < h; i++) {
				data[i] = zero;
			}
			/*
						for (int i = 0; i < w; i++) {
							data[i][i] = 1;
						}
			*/
		}

		MatrixNM(const T& v) {
			for (int i = 0; i < h; i++) {
				data[i] = v;
			}
		}

		MatrixNM(const MatrixNM& that) {
			assign(that);
		}

		MatrixNM(const vector_type& that) {
			for (int i = 0; i < h; i++) {
				data[i] = that;
			}
		}

		VectorN<T, h>& operator[](int w) {
			return data[w];
		}

		operator T* () { return &data[0][0]; }


	protected:
		VectorN<T, w> data[h];

		void assign(const MatrixNM& that) {
			for (int i = 0; i < h; i++) {
				data[i] = that.data[i];
			}

		}
	};

	template< typename T >
	class Matrix3x3 : public MatrixNM< T, 3, 3> {

	public:

		typedef MatrixNM< T, 3, 3> base;
		typedef T element_type;

		Matrix3x3() : MatrixNM< T, 3, 3>{} {

		}

		Matrix3x3(const Vectorf3 p, const Vectorf3 q, const Vectorf3 r) {
			base::data[0] = p;
			base::data[1] = q;
			base::data[2] = r;
		}

		
		Matrix3x3( T hInDegree, T pInDegree, T bInDegree){

			T h, p, b;
			h = hInDegree * (PI / 180);
			p = pInDegree * (PI / 180);
			b = bInDegree * (PI / 180);

			Vector3<T> vP{ cos(h)*cos(b)+sin(h)*sin(p)*sin(b)  , sin(b)*cos(p) , -sin(h)*cos(b)+ cos(h)*sin(p)*sin(b) };
			Vector3<T> vQ{ -cos(h)*cos(b)+ sin(h)*sin(p)*cos(b), cos(b)*cos(p) , sin(b)*sin(h)+ cos(h)*sin(p)*cos(b)  };
			Vector3<T> VR{ sin(h)*cos(p)                       , -sin(p)       ,     cos(h)*cos(p)                    };

			(*this)[0] = vP;
			(*this)[1] = vQ;
			(*this)[2] = VR;
		}

		void GetEulerAngles( T& hInDegree, T& pInDegree, T& bInDegree) {

			T h, p, b;

			p = asin( -(*this)[2][1] );

			if ( p >= (PI / 2.0f) || p <= -(PI / 2.0f) ) {

				h = atan2f(-(*this)[0][2], (*this)[0][0]);
				b = 0.0f;
			}
			else {
				h = atan2f((*this)[2][0], (*this)[2][2]);
				b = atan2f((*this)[0][1], (*this)[1][1]);
			}

			hInDegree = h * (180 / PI);
			pInDegree = p * (180 / PI);
			bInDegree = b * (180 / PI);

			//
			// Slightly adjust the value to fit the gap between canonical angles and the range of atan2() which
			// returns the range from -180 to +180.  -180 is not included in the canonical angles.
			// This returns +180 instead of -180.

			if (hInDegree <= -179.9999f) {
				hInDegree *= -1.0f;
			}

			if (bInDegree <= -179.9999f) {
				bInDegree *= -1.0f;
			}

		}

		void identity() {
			/*			base::data[0] = { 1, 0, 0 };
						base::data[1] = { 0, 1, 0 };
						base::data[2] = { 0, 0, 1 };
			*/
			base::data[0][0] = 1.0f;
			base::data[0][1] = 0.0f;
			base::data[0][2] = 0.0f;

			base::data[1][0] = 0.0f;
			base::data[1][1] = 1.0f;
			base::data[1][2] = 0.0f;

			base::data[2][0] = 0.0f;
			base::data[2][1] = 0.0f;
			base::data[2][2] = 1.0f;
		}

		base::vector_type& operator[](const int w) {
			return base::data[w];
		}

		operator Quaternion<T> () {

			T m11 = (*this)[0][0];
			T m12 = (*this)[0][1];
			T m13 = (*this)[0][2];

			T m21 = (*this)[1][0];
			T m22 = (*this)[1][1];
			T m23 = (*this)[1][2];

			T m31 = (*this)[2][0];
			T m32 = (*this)[2][1];
			T m33 = (*this)[2][2];

			T m12plusm21  = m12 + m21;
			T m12minusm21 = m12 - m21;

			T m23plusm32  = m23 + m32;
			T m23minusm32 = m23 - m32;

			T m31plusm13  = m31 + m13;
			T m31minusm13 = m31 - m13;

//			T w = sqrtf((m11 + m22 + m33 + 1) / 4);
//			T x = sqrtf((m11 - m22 - m33 + 1) / 4);
//			T y = sqrtf((m22 - m11 - m33 + 1) / 4);
//			T z = sqrtf((m33 - m11 - m22 + 1) / 4);

			T w = m11 + m22 + m33 + 1.0f;
			T x = m11 - m22 - m33 + 1.0f;
			T y = m22 - m11 - m33 + 1.0f;
			T z = m33 - m11 - m22 + 1.0f;

			enum largestIndex { indexW, indexX, indexY, indexZ };

			enum largestIndex index = indexW;
			T largest = w;

			if ( x > largest ) {
				largest = x;
				index = indexX;
			}
			if ( y > largest) {
				largest = y;
				index = indexY;
			}
			if ( z > largest) {
				largest = z;
				index = indexZ;
			}

			T w_final = 0;
			T x_final = 0;
			T y_final = 0;
			T z_final = 0;

			switch (index) {
			case indexW:
				// w is a largest.
//				w_final = w;
				w_final = sqrtf(largest / 4.0f);
				x_final = m23minusm32   / (4.0f * w_final);
				y_final = m31minusm13   / (4.0f * w_final);
				z_final = m12minusm21   / (4.0f * w_final);
				break;

			case indexX:
				// x is a largest.
//				x_final = x;
				x_final = sqrtf(largest / 4.0f);
				w_final = m23minusm32   / (4.0f * x_final);
				y_final = m12plusm21    / (4.0f * x_final);
				z_final = m31plusm13    / (4.0f * x_final);
				break;

			case indexY:
				// y is a largest.
//				y_final = y;
				y_final = sqrtf(largest / 4.0f);
				w_final = m31minusm13   / (4.0f * y_final);
				x_final = m12plusm21    / (4.0f * y_final);
				z_final = m23plusm32    / (4.0f * y_final);
				break;

			case indexZ:
				// z is a largest.
//				z_final = z;
				z_final = sqrtf(largest / 4.0f);
				w_final = m12minusm21   / (4.0f * z_final);
				x_final = m31plusm13    / (4.0f * z_final);
				y_final = m23plusm32    / (4.0f * z_final);
				break;

			}

			VectorN<T, 3> partVector{ x_final, y_final, z_final };
			Quaternion<T> quaternion{ w_final, partVector };

			return quaternion;

		}

/*
	operator Quaternion<T>() {

		T m11 = ((*this)[0])[0];
		T m12 = ((*this)[0])[1];
		T m13 = ((*this)[0])[2];

		T m21 = ((*this)[1])[0];
		T m22 = ((*this)[1])[1];
		T m23 = ((*this)[1])[2];

		T m31 = ((*this)[2])[0];
		T m32 = ((*this)[2])[1];
		T m33 = ((*this)[2])[2];

		T m12plusm21 = m12 + m21;
		T m12minusm21 = m12 - m21;

		T m23plusm32 = m23 + m32;
		T m23minusm32 = m23 - m32;

		T m31plusm13 = m31 + m13;
		T m31minusm13 = m31 - m13;

		//			T w = sqrtf((m11 + m22 + m33 + 1) / 4);
		//			T x = sqrtf((m11 - m22 - m33 + 1) / 4);
		//			T y = sqrtf((m22 - m11 - m33 + 1) / 4);
		//			T z = sqrtf((m33 - m11 - m22 + 1) / 4);

		T w = m11 + m22 + m33 + 1.0f;
		T x = m11 - m22 - m33 + 1.0f;
		T y = m22 - m11 - m33 + 1.0f;
		T z = m33 - m11 - m22 + 1.0f;

		enum largestIndex { indexW, indexX, indexY, indexZ };

		enum largestIndex index = indexW;
		T largest = w;

		if (x > largest) {
			largest = x;
			index = indexX;
		}
		if (y > largest) {
			largest = y;
			index = indexY;
		}
		if (z > largest) {
			largest = z;
			index = indexZ;
		}

		T w_final = 0;
		T x_final = 0;
		T y_final = 0;
		T z_final = 0;

		switch (index) {
		case indexW:
			// w is a largest.
	//				w_final = w;
			w_final = sqrtf(largest) * 0.5f;
			x_final = (m23minusm32 / w_final) * 0.25f;
			y_final = (m31minusm13 / w_final) * 0.25f;
			z_final = (m12minusm21 / w_final) * 0.25f;
			break;

		case indexX:
			// x is a largest.
	//				x_final = x;
			x_final = sqrtf(largest) * 0.5f;
			w_final = (m23minusm32 / x_final) * 0.25f;
			y_final = (m12plusm21 / x_final) * 0.25f;
			z_final = (m31plusm13 / x_final) * 0.25f;
			break;

		case indexY:
			// y is a largest.
	//				y_final = y;
			y_final = sqrtf(largest) * 0.5f;
			w_final = (m31minusm13 / y_final) * 0.25f;
			x_final = (m12plusm21 / y_final) * 0.25f;
			z_final = (m23plusm32 / y_final) * 0.25f;
			break;

		case indexZ:
			// z is a largest.
	//				z_final = z;
			z_final = sqrtf(largest) * 0.5f;
			w_final = (m12minusm21 / z_final) * 0.25f;
			x_final = (m31plusm13 / z_final) * 0.25f;
			y_final = (m23plusm32 / z_final) * 0.25f;
			break;

		}

		VectorN<T, 3> partVector{ x_final, y_final, z_final };
		Quaternion<T> quaternion{ w_final, partVector };

		return quaternion;

	}
*/
	Matrix3x3<T> operator*(const Matrix3x3<T>& that) {

			Matrix3x3<T> myThat = { that };
			Matrix3x3<T> ret;

			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					for (int k = 0; k < 3; k++) {
						ret[i][j] += (*this)[i][k] * myThat[k][j];
					}
				}
			}

			return ret;
		}

		Matrix3x3 orthogonilize() const {

			Matrix3x3 orth = *this;

			// orth[0] = orth[0] 1st row will be the same as before.

			orth[1] = orth[1] -
				(orth[1] * orth[0]) / (orth[0] * orth[0]) * orth[0];

			orth[2] = orth[2] -
				(orth[2] * orth[0]) / (orth[0] * orth[0]) * orth[0] -
				(orth[2] * orth[1]) / (orth[1] * orth[1]) * orth[1];

			return orth;
		}

		Matrix3x3 transpose() {

			Matrix3x3 trans = *this;

			for (int j = 0; j < 3; j++) {
				for (int i = 0; i < 3; i++) {
					trans[i][j] = (*this)[j][i];
				}
			}

			return trans;
		}

		Matrix3x3 normalize() {

			Matrix3x3 norm;

			for (int i = 0; i < 3; i++) {
				norm[i] = (*this)[i].normalize();
			}

			return norm;

		}
	};

	typedef Matrix3x3<float> Matrixf3x3;

	template< typename T >
	class Matrix4x4 : public MatrixNM< T, 4, 4> {

	public:

		typedef MatrixNM< T, 4, 4> base;
		typedef T element_type;

		Matrix4x4() : MatrixNM< T, 4, 4>{} {
		}

		Matrix4x4(const Matrixf3x3& that3x3) : MatrixNM< T, 4, 4>{} {
			Matrixf3x3 mythat = that3x3;

			base::data[0][0] = mythat[0][0];
			base::data[0][1] = mythat[0][1];
			base::data[0][2] = mythat[0][2];
			base::data[0][3] = 0;

			base::data[1][0] = mythat[1][0];
			base::data[1][1] = mythat[1][1];
			base::data[1][2] = mythat[1][2];
			base::data[1][3] = 0;

			base::data[2][0] = mythat[2][0];
			base::data[2][1] = mythat[2][1];
			base::data[2][2] = mythat[2][2];
			base::data[2][3] = 0;

			base::data[3][0] = 0;
			base::data[3][1] = 0;
			base::data[3][2] = 0;
			base::data[3][3] = 1;

		}

		Matrix4x4(const Vectorf4 p, const Vectorf4 q, const Vectorf4 r, const Vectorf4 s) {
			base::data[0] = p;
			base::data[1] = q;
			base::data[2] = r;
			base::data[3] = s;
		}

		void identity() {
			Vectorf4 row1 = { 1.0f,0.0f,0.0f,0.0f };
			Vectorf4 row2 = { 0.0f,1.0f,0.0f,0.0f };
			Vectorf4 row3 = { 0.0f,0.0f,1.0f,0.0f };
			Vectorf4 row4 = { 0.0f,0.0f,0.0f,1.0f };

			base::data[0] = row1;
			base::data[1] = row2;
			base::data[2] = row3;
			base::data[3] = row4;
		}

		base::vector_type& operator[](const int w) {
			return base::data[w];
		}

		Matrix4x4<T> operator*(const Matrix4x4<T>& that) {

			Matrix4x4<T> myThat = { that };
			Matrix4x4<T> ret;

			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					for (int k = 0; k < 4; k++) {
						ret[i][j] += (*this)[i][k] * myThat[k][j];
					}
				}
			}

			return ret;
		}

		Matrixf3x3 homogeniousDivide() {

			T w = base::data[3][3];

			Matrixf3x3 ret = { { base::data[0][0] / w, base::data[0][1] / w, base::data[0][2] / w },
							   { base::data[1][0] / w, base::data[1][1] / w, base::data[1][2] / w },
							   { base::data[2][0] / w, base::data[2][1] / w, base::data[2][2] / w },
			};

			return ret;
		}

		Matrix4x4<T> orthogonilize() const {

			Matrix4x4<T> orth = *this;

			// orth[0] = orth[0] 1st row will be the same as before.

			orth[1] = orth[1] -
				(orth[1] * orth[0]) / (orth[0] * orth[0]) * orth[0];

			orth[2] = orth[2] -
				(orth[2] * orth[0]) / (orth[0] * orth[0]) * orth[0] -
				(orth[2] * orth[1]) / (orth[1] * orth[1]) * orth[1];

			orth[3] = orth[3] -
				(orth[3] * orth[0]) / (orth[0] * orth[0]) * orth[0] -
				(orth[3] * orth[1]) / (orth[1] * orth[1]) * orth[1] -
				(orth[3] * orth[2]) / (orth[2] * orth[2]) * orth[2];

			return orth;
		}

		Matrix4x4<T> transpose() {

			Matrix4x4<T> trans = *this;

			for (int j = 0; j < 4; j++) {
				for (int i = 0; i < 4; i++) {
					trans[i][j] = (*this)[j][i];
				}
			}

			return trans;
		}

		Matrix4x4<T> normalize() {

			Matrix4x4<T> norm;

			for (int i = 0; i < 4; i++) {
				norm[i] = (*this)[i].normalize();
			}

			return norm;

		}

		Matrix4x4<T> identityNegateZAxis() {

			identity();
			(*this)[2][2] = -1.0f;

			return (*this);
		}

	};

	typedef Matrix4x4<float> Matrixf4x4;

	/*
		// Dot Product
		Vector3<float> operator*(const matrixf3x3& inThis, const matrixf3x3& inThat) {
			Vector3<float> ret;

			matrixf3x3 myThis = inThis;
			matrixf3x3 myThat = inThat;
			for (int i = 0; i < 3; i++) {
				for (int k = 0; k < 3; k++) {
					ret[i] += myThis[i][k] * myThat[i][k];
				}
			}

			return ret;
		}
	*/
	/*
	Vector3<float> operator*(const Vector3<float>& inThis, const matrixf3x3& inThat) {
		Vector3<float> ret;

		Vector3<float> myThis = inThis;
		matrixf3x3 myThat = inThat;
		for (int i = 0; i < 3; i++) {
			for (int k = 0; k < 3; k++) {
				ret[i] += myThis[k] * myThat[i][k];
			}
		}

		return ret;
	}
*/

	static Vector3<float> operator*(const Vector3<float>& inThis, const Matrixf3x3& inThat) {
		Vector3<float> ret;

		Vector3<float> myThis = inThis;
		Matrixf3x3 myThat = inThat;
		for (int i = 0; i < 3; i++) {
			for (int k = 0; k < 3; k++) {
				ret[i] += myThis[k] * myThat[k][i];
			}
		}

		return ret;
	}


	static Vector4<float> operator*(const Vector4<float>& inThis, const Matrixf4x4& inThat) {
		Vector4<float> ret;

		Vector4<float> myThis = inThis;
		Matrixf4x4 myThat = inThat;
		for (int i = 0; i < 4; i++) {
			for (int k = 0; k < 4; k++) {
				ret[i] += myThis[k] * myThat[k][i];
			}
		}

		return ret;
	}

	/*
		static Matrixf4x4 frustum(float zoomX, float zoomY, float left, float right, float bottom, float top, float n, float f)
		{
			Matrixf4x4 perspectiveTransform;

			perspectiveTransform[0][0] = zoomX * ((2.0f * n) / (right - left));

			perspectiveTransform[1][1] = zoomY * ((2.0f * n) / (top - bottom));

			perspectiveTransform[2][0] = zoomX * ((right + left) / (right - left));
			perspectiveTransform[2][1] = zoomY * ((top + bottom) / (top - bottom));
			perspectiveTransform[2][2] = f / (n - f);
			perspectiveTransform[2][3] = -1.0f;

			perspectiveTransform[3][2] = (f * n) / (n - f);

			return perspectiveTransform;
		}
	*/
	/*
	static Matrixf4x4 frustum(float zoomX, float zoomY, float left, float right, float bottom, float top, float n, float f)
	{

		Matrixf4x4 zoomMat;
		zoomMat.identity();
		zoomMat[0][0] = zoomX;
		zoomMat[1][1] = zoomY;

		Matrixf4x4 perspectiveTransform;

		perspectiveTransform[0][0] = ((2.0f * n) / (right - left));

		perspectiveTransform[1][1] = ((2.0f * n) / (top - bottom));

		perspectiveTransform[2][0] = ((right + left) / (right - left));
		perspectiveTransform[2][1] = ((top + bottom) / (top - bottom));

		perspectiveTransform[2][2] = -(f + n) / (f - n);
		perspectiveTransform[2][3] = -1.0f;

		perspectiveTransform[3][2] = (-2.0f * f * n) / (f - n);

		return zoomMat * perspectiveTransform;
	}
	*/

	static Matrixf4x4 frustum_nozoom(float zoomX, float zoomY, float left, float right, float bottom, float top, float n, float f)
	{
		Matrixf4x4 perspectiveTransform;

		perspectiveTransform[0][0] = ((2.0f * n) / (right - left));

		perspectiveTransform[1][1] = ((2.0f * n) / (top - bottom));

		perspectiveTransform[2][0] = ((right + left) / (right - left));
		perspectiveTransform[2][1] = ((top + bottom) / (top - bottom));

		perspectiveTransform[2][2] = -(f + n) / (f - n);
		perspectiveTransform[2][3] = -1.0f;

		perspectiveTransform[3][2] = (-2.0f * f * n) / (f - n);

		return perspectiveTransform;
	}

	/*
	// Text version of frustum with zooming calculation for x and y.( not for z )
	static Matrixf4x4 frustum(float zoomX, float zoomY, float left, float right, float bottom, float top, float n, float f)
	{
		Matrixf4x4 perspectiveTransform;

		perspectiveTransform[0][0] = zoomX * ((2.0f * n) / (right - left));
//		perspectiveTransform[0][0] = ((2.0f * n) / (right - left));

		perspectiveTransform[1][1] = zoomY * ((2.0f * n) / (top - bottom));
//		perspectiveTransform[1][1] = ((2.0f * n) / (top - bottom));

//		perspectiveTransform[2][0] = zoomX * ((right + left) / (right - left));
//		perspectiveTransform[2][1] = zoomY * ((top + bottom) / (top - bottom));
		perspectiveTransform[2][0] = ((right + left) / (right - left));
		perspectiveTransform[2][1] = ((top + bottom) / (top - bottom));

		perspectiveTransform[2][2] = -(f + n) / (f - n);
		perspectiveTransform[2][3] = -1.0f;

		perspectiveTransform[3][2] = (-2.0f * f * n) / (f - n);

		return perspectiveTransform;
	}
*/
/*
	// Text version of frustum without zooming calculation.
	static Matrixf4x4 frustum2(float left, float right, float bottom, float top, float n, float f)
	{
		Matrixf4x4 perspectiveTransform;

		perspectiveTransform[0][0] = ((2.0f * n) / (right - left));

		perspectiveTransform[1][1] = ((2.0f * n) / (top - bottom));

		perspectiveTransform[2][0] = ((right + left) / (right - left));
		perspectiveTransform[2][1] = ((top + bottom) / (top - bottom));
		perspectiveTransform[2][2] = -(f + n) / (f - n);
		perspectiveTransform[2][3] = -1.0f;

		perspectiveTransform[3][2] = (-2.0f * f * n) / (f - n);

		return perspectiveTransform;
	}
*/
	static Matrixf4x4 rotate(float normal_x, float normal_y, float normal_z, float theta)
	{
		Matrixf4x4 rotationTransform;
		rotationTransform.identity();

		rotationTransform[0][0] = powf(normal_x, 2.0f) * (1.0f - cosf(theta)) + cosf(theta);
		rotationTransform[0][1] = normal_x * normal_y * (1.0f - cosf(theta)) + normal_z * sinf(theta);
		rotationTransform[0][2] = normal_x * normal_z * (1.0f - cosf(theta)) - normal_y * sinf(theta);

		rotationTransform[1][0] = normal_x * normal_y * (1.0f - cosf(theta)) - normal_z * sinf(theta);
		rotationTransform[1][1] = powf(normal_y, 2.0f) * (1.0f - cosf(theta)) + cosf(theta);
		rotationTransform[1][2] = normal_y * normal_z * (1.0f - cosf(theta)) + normal_x * sinf(theta);

		rotationTransform[2][0] = normal_x * normal_z * (1.0f - cosf(theta)) + normal_y * sinf(theta);
		rotationTransform[2][1] = normal_y * normal_z * (1.0f - cosf(theta)) - normal_x * sinf(theta);
		rotationTransform[2][2] = powf(normal_z, 2.0f) * (1.0f - cosf(theta)) + cosf(theta);

		return rotationTransform;
	}

	static Matrixf3x3 rotate3x3(float normal_x, float normal_y, float normal_z, float theta)
	{
		Matrixf3x3 rotationTransform;
		rotationTransform.identity();

		rotationTransform[0][0] = powf(normal_x, 2.0f) * (1.0f - cosf(theta)) + cosf(theta);
		rotationTransform[0][1] = normal_x * normal_y * (1.0f - cosf(theta)) + normal_z * sinf(theta);
		rotationTransform[0][2] = normal_x * normal_z * (1.0f - cosf(theta)) - normal_y * sinf(theta);

		rotationTransform[1][0] = normal_x * normal_y * (1.0f - cosf(theta)) - normal_z * sinf(theta);
		rotationTransform[1][1] = powf(normal_y, 2.0f) * (1.0f - cosf(theta)) + cosf(theta);
		rotationTransform[1][2] = normal_y * normal_z * (1.0f - cosf(theta)) + normal_x * sinf(theta);

		rotationTransform[2][0] = normal_x * normal_z * (1.0f - cosf(theta)) + normal_y * sinf(theta);
		rotationTransform[2][1] = normal_y * normal_z * (1.0f - cosf(theta)) - normal_x * sinf(theta);
		rotationTransform[2][2] = powf(normal_z, 2.0f) * (1.0f - cosf(theta)) + cosf(theta);

		return rotationTransform;
	}
	/*
		// Note that each coordinate will be negated before calculation for convenience.
		static Matrixf4x4 translate(float move_x, float move_y, float move_z)
		{
			Matrixf4x4 transltionTransform;
			transltionTransform.identity();

			transltionTransform[3][0] = -move_x;
			transltionTransform[3][1] = -move_y;
			transltionTransform[3][2] = -move_z;

			return transltionTransform;
		}
	*/

	// Note that each coordinate will be negated before calculation for convenience.
	static Matrixf4x4 translate2(float move_x, float move_y, float move_z)
	{
		Matrixf4x4 transltionTransform;
		transltionTransform.identity();

		transltionTransform[3][0] = move_x;
		transltionTransform[3][1] = move_y;
		transltionTransform[3][2] = move_z;

		return transltionTransform;
	}


	// Scaling
	static Matrixf4x4 scale(float nX, float nY, float nZ, float k)
	{
		Matrixf4x4 scalingTransform;
		scalingTransform.identity();

		scalingTransform[0][0] = powf(nX, 2.0f) * (k - 1.0f) + 1.0f;
		scalingTransform[0][1] = nX * nY * (k - 1.0f);
		scalingTransform[0][2] = nX * nZ * (k - 1.0f);

		scalingTransform[1][0] = nY * nX * (k - 1.0f);
		scalingTransform[1][1] = powf(nY, 2.0f) * (k - 1.0f) + 1.0f;
		scalingTransform[1][2] = nY * nZ * (k - 1.0f);

		scalingTransform[2][0] = nZ * nX * (k - 1.0f);
		scalingTransform[2][1] = nZ * nY * (k - 1.0f);
		scalingTransform[2][2] = powf(nZ, 2.0f) * (k - 1.0f) + 1.0f;

		return scalingTransform;
	}

	// Scaling
	static Matrixf4x4 scale(float k)
	{
		Matrixf4x4 scalingTransform;
		scalingTransform.identity();

		scalingTransform[0][0] = k;
		scalingTransform[1][1] = k;
		scalingTransform[2][2] = k;

		return scalingTransform;
	}

	// Scaling
	static Matrixf3x3 scale3x3(float k)
	{
		Matrixf3x3 scalingTransform;
		scalingTransform.identity();

		scalingTransform[0][0] = k;
		scalingTransform[1][1] = k;
		scalingTransform[2][2] = k;

		return scalingTransform;
	}

	template < typename T >
	class Quaternion {
	public:

		// Quaternion dimension.
		const static int len = 3;

		Quaternion() {
			x = 0.0f;
			y = 0.0f;
			z = 0.0f;
			w = 0.0f;
		}

		Quaternion( T inW, T inX, T inY, T inZ ) {
			w = inW;
			x = inX;
			y = inY;
			z = inZ;
		}

		Quaternion( T inW, VectorN<T, 3> v ) {
			x = v[0];
			y = v[1];
			z = v[2];
			this->w = inW;
		}

		Quaternion(T headingInDegree, T pitchInDegree, T bankInDegree){

			T heading = headingInDegree * ( PI / 180.0f );
			T pitch   = pitchInDegree   * ( PI / 180.0f );
			T bank    = bankInDegree    * ( PI / 180.0f );

			VectorN<T,3> normalX{ 1.0f, 0.0f, 0.0f };
			VectorN<T,3> normalY{ 0.0f, 1.0f, 0.0f };
			VectorN<T,3> normalZ{ 0.0f, 0.0f, 1.0f };

			Quaternion<T> qHeading{ cosf(heading / 2.0f), sinf(heading/ 2.0f) * normalY };
			Quaternion<T> qPitch  { cosf(pitch   / 2.0f), sinf(pitch  / 2.0f) * normalX };
			Quaternion<T> qBank   { cosf(bank    / 2.0f), sinf(bank   / 2.0f) * normalZ };

			Quaternion<T> qTotal = qHeading.product( qPitch ).product( qBank );

			w = qTotal.w;
			x = qTotal.x;
			y = qTotal.y;
			z = qTotal.z;
		}

		void GetEulerAngles(T& heading, T& pitch, T& bank) {

			T pTmp = -(2 * z * y - 2 * x * w);
			if (pTmp > 0.0f) {
				pTmp = std::min(1.0f, pTmp);
			}
			else {
				pTmp = std::max(-1.0f, pTmp);
			}

			T p = asin(pTmp);

			T h, b;

			if (fabs(pTmp) > 0.9999f) {

				h = atan2f(-(2*x*z - 2*y*w) , (1 - 2*powf(y, 2.0f) - 2*powf(z, 2.0f)));
				b = 0.0f;
			}
			else {

				h = atan2f( ( 2.0f*x*z + 2.0f*y*w) , ( 1 - 2.0f*powf(x,2.0f)- 2.0f*powf(y,2.0f)) );
				b = atan2f( ( 2.0f*x*y + 2.0f*z*w) , ( 1 - 2.0f*powf(x,2.0f)- 2.0f*powf(z,2.0f)) );
			}

			heading = h * ( 180.0f / PI );
			pitch   = p * ( 180.0f / PI );
			bank    = b * ( 180.0f / PI );

		}

		operator Matrixf3x3 () {
			
			Vectorf3 p = { 1.0f - 2.0f * powf(y,2.0f) - 2.0f * powf(z,2.0f), 2.0f *x*y + 2.0f *z*w                               , 2.0f *x*z - 2.0f *y*w                            };
			Vectorf3 q = { 2.0f *y*x - 2.0f *z*w                           , 1.0f - 2.0f * powf( x, 2.0f) - 2.0f * powf(z, 2.0f ), 2.0f *y*z + 2.0f *x*w                            };
			Vectorf3 r = { 2.0f *x*z + 2.0f *y*w                           , 2.0f *z*y - 2.0f *x*w                               , 1.0f - 2.0f * powf(x,2.0f) - 2.0f * powf(y,2.0f) };

			Matrixf3x3 rotMat{ p, q, r };
			return rotMat;
		}

		Quaternion( Vectorf3 v) {
			x = v[0];
			y = v[1];
			z = v[2];
			this->w = 0.0f;
		}

		Quaternion(const Quaternion<T>& q ) {
			assign( q );
		}

		void assign(const Quaternion<T>& q ) {
			x = q.x;
			y = q.y;
			z = q.z;
			w = q.w;
		}

		operator Vectorf3 () {

			Vectorf3 v = { x, y, z };
			return v;
		}

		const Quaternion<T> operator*(T v) const {

			Quaternion<T> ret = *this;
			ret.x *= v;
			ret.y *= v;
			ret.z *= v;
			ret.w *= v;

			return ret;
		}

		const Quaternion<T> operator+(Quaternion<T> q ) const {

			T wRet = this->w + q.w;
			T xRet = this->x + q.x;
			T yRet = this->y + q.y;
			T zRet = this->z + q.z;

			Quaternion<T> qRet { wRet, xRet, yRet, zRet };
			return qRet;
		}

		void conjugate() {

			x = -x;
			y = -y;
			z = -z;

		}

		void inverse() {

			conjugate();
			float denom = length();
			(*this) = (*this ) / denom;

		}

		const Quaternion<T> product( const Quaternion<T> q2 ) const {

			T w1 = this->w;
			T w2 = q2.w;
			Vectorf3 v1 { this->x, this->y, this->z };
			Vectorf3 v2 { q2.x, q2.y, q2.z };

			T newW = w1 * w2 - v1 * v2;
			Vectorf3 newV = w1 * v2 + w2 * v1 + v1.cross(v2);

			Quaternion<T> newQ{ newW, newV };

			return newQ;
		}

		T rotate(Vectorf3 p) {
			
			Quaternion<T> p{ 0, p[0], p[1], p[2] };
			Quaternion<T> qInversed = this->inverse();

			Quaternion<T> newP = (*this).product( p ).product( qInversed );

			return newP;
		}

		T difference( Quaternion<T> b ) {

			Quaternion<T> aInversed = this->inverse();
			Quaternion<T> qDifference = b.product(aInversed);

			return qDifference;
		}
/*
		T& operator[](const int h) {
			return data[h];
		}
*/
		const Quaternion<T> operator/(T d) {

			Quaternion<T> q = (*this);

			q.w = q.w / d;
			q.x = q.x / d;
			q.y = q.y / d;
			q.z = q.z / d;

			return q;
		}

		// Dot Product
		T operator*( const Quaternion<T>& that) {

			T ret = w * that.w + x * that.x + y * that.y + z * that.z;
			return ret;
		}

		void log() {

			Vectorf3 normal{ x, y, z };
			normal.normalize();

			T alpha = atan2(x, w * normal.getX());

			Vectorf3 v{ normal * alpha };

			w = 0.0f;
			x = v.getX();
			y = v.getY();
			z = v.getZ();

		}

		// exponential
		void exp() {

			Vectorf3 normal{ this->x, this->y, this->z };

			if (normal.length() == 0.0f) {

				w = 1.0f;
				x = 0.0f;
				y = 0.0f;
				z = 0.0f;
				return;
			}

			normal.normalize();

			T nume;
			T denom;

			//
			// Choose greatest denominator;
			//

			nume = x;
			denom = normal.getX();

			if ( normal.getY() > denom ){
				nume = y;
				denom = normal.getY();
			}
			if ( normal.getZ() > denom) {
				nume = z;
				denom = normal.getZ();
			}

			T newAlpha = nume / denom;

			Vectorf3 v{ normal * sinf(newAlpha) };

			w = cosf( newAlpha );
			x = v.getX();
			y = v.getY();
			z = v.getZ();

		}

		// exponentiation.
		void myPow(T t) {

			log();

			(*this) = (*this) * t;

			exp();
		}

		// exponentiation.
		void pow(T t) {

			if (abs(w) < 0.9999f) {

				float alpha = acosf(w);
				float newAlpha = alpha * t;

				w = cosf(newAlpha);

				float mult = sinf(newAlpha) / sinf(alpha);

				x *= mult;
				y *= mult;
				z *= mult;
			}
		}

		T length() {
			return sqrtf((*this) * (*this));
		}

		Quaternion<T> slerp(Quaternion<T> q1, T t) {

			Quaternion<T> q0 = (*this);
			Quaternion<T> q0_2 = (*this);

			q0_2.inverse();
			Quaternion<T> qTemp = q1.product(q0_2);
			qTemp.normalize();	// ノーマライズしないと場合によって-nanになってしまう。
			qTemp.pow(t);
			Quaternion<T> qRet = qTemp.product( q0 );
			return qRet;

		}

		Quaternion<T> mySlerp(Quaternion<T> q1, T t) {

			Quaternion<T> q0 = (*this);
			Quaternion<T> q0_2 = (*this);

			q0_2.inverse();
			Quaternion<T> qTemp = q1.product(q0_2);
			qTemp.normalize();	// ノーマライズしないと場合によって-nanになってしまう。
			qTemp.myPow(t);
			Quaternion<T> qRet = qTemp.product(q0);
			return qRet;

		}

		void normalize() {

			T len = length();
			w = w / len;
			x = x / len;
			y = y / len;
			z = z / len;

		}

	public:
		T x;
		T y;
		T z;
		T w;
	};

	typedef Quaternion<float> Quaternionf;

	template<typename T>
	Quaternion<T> slerp(Quaternion<T> q0, Quaternion<T> q1, T t) {

		VectorN<T, 3> v0{ q0.x, q0.y, q0.z };
		VectorN<T, 3> v1{ q1.x, q1.y, q1.z };

		v0.normalize();
		v1.normalize();

		T cosw = (v0 * v1) / (v0.length() * v1.length());
		if (cosw < 0) {
			// Choose shortest arc.
			v1 = v1 * -1.0f;
			cosw = -cosw;
		}

		T k0, k1;

		if (cosw > 0.9999f) {

			k0 = 1.0f - t;
			k1 = t;
		}
		else {

			T sinw = sqrtf(1.0f - cosw * cosw);
			T w = atan2f(sinw, cosw);

			T oneOverSinW = 1.0f / sinw;

			k0 = sinf( (1.0f - t) * w) * oneOverSinW;
			k1 = sinf(t * w) * oneOverSinW;
		}

		Quaternion<T> qRet{ q0 * k0 + q1 * k1 };
		return qRet;

	}


};

#endif
