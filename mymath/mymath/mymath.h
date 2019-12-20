#pragma once

#ifndef _MYMATH_
#define _MYMATH_

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







	template< typename T, int w, int h >
	class MatrixNM {

	public:
		typedef class VectorN<T, h> vector_type;

		MatrixNM() {
			for (int i = 0; i < w; i++) {
				data[i] = vector_type{0};
			}
/*
			for (int i = 0; i < w; i++) {
				data[i][i] = 1;
			}
*/
		}

		MatrixNM(const T& v) {
			for (int i = 0; i < w; i++) {
				data[i] = v;
			}
		}

		MatrixNM(const MatrixNM& that) {
			assign(that);
		}

		MatrixNM(const vector_type& that) {
			for (int i = 0; i < w; i++) {
				data[i] = that;
			}
		}
	
		VectorN<T, h>& operator[](int w)  {
			return data[w];
		}

	protected:
		VectorN<T, h> data[w];

		void assign(const MatrixNM& that) {
			for (int i = 0; i < w; i++) {
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
			base::data[0] = { 1,0,0,0 };
			base::data[1] = { 0,1,0,0 };
			base::data[2] = { 0,0,1,0 };
			base::data[3] = { 0,0,0,1 };
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
	Vector4<float> operator*(const Vector4<float>& inThis, const Matrixf4x4& inThat) {
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



};

#endif
