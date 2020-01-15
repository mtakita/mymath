#pragma once

#ifndef _MYMATH_
#define _MYMATH_

#include <math.h>

namespace mymath {

	template < typename T, int len >
	class VectorN {
	public:

		VectorN() {
			for (int i = 0; i < len; i++) {
				data[i] = 0;
			}
		}

		VectorN(T v) {
			for (int i = 0; i < len; i++) {
				data[i] = v;
			}
		}

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

		const VectorN& operator*=(const T& v){

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

		T abs() {
			return sqrt((*this) * (*this));
		}

		VectorN normalize() {
			T t = abs();
			return (*this) / t;
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
	class Vector3 : public VectorN <T, 3 >{
		typedef VectorN <T, 3 > base;
		typedef Vector3<T> mytype;

	public:
		Vector3(T x, T y, T z) {
			base::data[0] = x;
			base::data[1] = y;
			base::data[2] = z;
		}

		Vector3( const base& that) : base( that ) {
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

		// Duplicate.  
		const Vector3 cross(const Vector3& that) {

			Vector3 ret = {
				base::data[1] * that.data[2] - base::data[2] * that.data[1],
				base::data[2] * that.data[0] - base::data[0] * that.data[2],
				base::data[0] * that.data[1] - base::data[1] * that.data[0],
			};

			return ret;
		}
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

			Vectorf3 ret = { base::data[0] / w, base::data[1] / w, base::data[2] / w };

			return ret;
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
	const VectorN<T,3> operator*(const T& v, const VectorN<T, 3>& that) {

		VectorN<T, 3> ret = that;

		for (int i = 0; i < 3; i++) {
			ret[i] = v * ret[i];
		}

		return ret;
	}







	template< typename T, int h, int w >
	class MatrixNM {

	public:
		typedef class VectorN<T, h> vector_type;

		MatrixNM() {
			for (int i = 0; i < h; i++) {
				data[i] = vector_type{0};
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
	
		VectorN<T, h>& operator[](int w)  {
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

		Matrix3x3() : MatrixNM< T, 3, 3>{}  {

		}

		Matrix3x3(const Vectorf3 p, const Vectorf3 q, const Vectorf3 r) {
			base::data[0] = p;
			base::data[1] = q;
			base::data[2] = r;
		}

		void identity() {
			base::data[0] = { 1, 0, 0 };
			base::data[1] = { 0, 1, 0 };
			base::data[2] = { 0, 0, 1 };

		}

		base::vector_type& operator[](const int w){
			return base::data[w];
		}

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
				(orth[2] * orth[1]) / (orth[1] * orth[1]) * orth[1] ;

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

		Matrix4x4 orthogonilize() const {

			Matrix4x4 orth = *this;

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

		Matrix4x4 transpose()  {

			Matrix4x4 trans = *this;

			for (int j = 0; j < 4; j++) {
				for (int i = 0; i < 4; i++) {
					trans[i][j] = (*this)[j][i];
				}
			}

			return trans;
		}

		Matrix4x4 normalize() {

			Matrix4x4 norm;

			for (int i = 0; i < 4; i++) {
				norm[i] = (*this)[i].normalizd();
			}

			return norm;

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

	static Matrixf4x4 frustum(float left, float right, float bottom, float top, float n, float f)
	{
		Matrixf4x4 perspectiveTransform;

		perspectiveTransform[0][0] = (2.0f * n) / (right - left);

		perspectiveTransform[1][1] = (2.0f * n) / (top - bottom);

		perspectiveTransform[2][0] = (right + left) / (right - left);
		perspectiveTransform[2][1] = (top + bottom) / (top - bottom);
		perspectiveTransform[2][2] = -(f + n) / (f - n);
		perspectiveTransform[2][3] = -1.0f;

		perspectiveTransform[3][2] = -(2.0f * f * n) / (f - n);

		return perspectiveTransform;
	}

	static Matrixf4x4 rotate(float normal_x, float normal_y, float normal_z, float theta)
	{
		Matrixf4x4 rotationTransform;
		rotationTransform.identity();

		rotationTransform[0][0] = pow(normal_x, 2) * (1 - cos(theta)) + cos(theta);
		rotationTransform[0][1] = normal_x * normal_y * (1 - cos(theta)) + normal_z * sin(theta);
		rotationTransform[0][2] = normal_x * normal_z * (1 - cos(theta)) - normal_y * sin(theta);

		rotationTransform[1][0] = normal_x * normal_y * (1 - cos(theta)) - normal_z * sin(theta);
		rotationTransform[1][1] = pow(normal_y, 2) * (1 - cos(theta)) + cos(theta);
		rotationTransform[1][2] = normal_y * normal_z * (1 - cos(theta)) + normal_x * sin(theta);

		rotationTransform[2][0] = normal_x * normal_z * (1 - cos(theta)) + normal_y * sin(theta);
		rotationTransform[2][1] = normal_y * normal_x * (1 - cos(theta)) - normal_x * sin(theta);
		rotationTransform[2][2] = pow(normal_z, 2) * (1 - cos(theta)) + cos(theta);

		return rotationTransform;
	}

	static Matrixf4x4 translate(float normal_x, float normal_y, float normal_z)
	{
		Matrixf4x4 transltionTransform;
		transltionTransform.identity();

		transltionTransform[3][0] = normal_x;
		transltionTransform[3][1] = normal_y;
		transltionTransform[3][2] = normal_z;

		return transltionTransform;
	}


};

#endif
