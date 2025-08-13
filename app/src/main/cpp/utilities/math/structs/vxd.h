#pragma once
#include <cmath>
#include <algorithm>


template<typename P>
class V2Dg {
public:
	V2Dg() = default;
	V2Dg(P x, P y) : x(x), y(y) { }
	V2Dg(const V2Dg<P>&) = default;
	template <typename U>
	V2Dg(const V2Dg<U>& other) : x(static_cast<P>(other.x)), y(static_cast<P>(other.y)) {
		static_assert(std::is_arithmetic<U>::value, "V2Dg<type> conversion must be numeric");
	}
	static_assert(std::is_arithmetic<P>::value, "V2Dg<type> must be numeric");

	~V2Dg() = default;
	P x = 0;
	P y = 0;

	inline V2Dg<P>& operator=(const V2Dg<P>&) = default;
	template <typename U>
	inline V2Dg<P>& operator=(const V2Dg<U>& other) {
		static_assert(std::is_arithmetic<U>::value, "V2Dg<type> conversion must be numeric");
		this->x = static_cast<U>(other.x);
		this->y = static_cast<U>(other.y);
		return *this;
	}

	template <typename U>
	inline bool operator==(const V2Dg<U>& other) const {
		static_assert(std::is_arithmetic<U>::value, "V2Dg<type> conversion must be numeric");
		return this->x == other.x && this->y == other.y;
	}

	V2Dg<P> operator-(const V2Dg<P>& other) const {
		return V2Dg{ this->x - other.x, this->y - other.y };
	}
	template<typename U>
	auto operator-(const V2Dg<U>& other) const {
		return V2Dg<std::common_type_t<P, U>>{ this->x - other.x, this->y - other.y };
	}
	V2Dg<P> operator+(const V2Dg<P>& other) const {
		return V2Dg<P>{ (P)this->x + other.x, (P)this->y + other.y };
	}
	V2Dg<P>& operator+=(const V2Dg<P>& other) {
		this->x += other.x;
		this->y += other.y;
		return *this;
	}
	V2Dg<P> operator*(const V2Dg<P>& other) const {
		return V2Dg<P>{ this->x* other.x, this->y* other.y };
	}
	V2Dg<P> operator/(const V2Dg<P>& other) const {
		return V2Dg<P>{ this->x / other.x, this->y / other.y };
	}


	friend V2Dg<P> operator+(P scalar, const V2Dg<P>& other) {
		return V2Dg<P>{ other.x + scalar, other.y + scalar };
	}
	V2Dg<P> operator+(P scalar) const {
		return V2Dg<P>{ this->x + scalar, this->y + scalar };
	}
	friend V2Dg<P> operator-(P scalar, const V2Dg<P>& other) {
		return V2Dg<P>{ other.x - scalar, other.y - scalar };
	}
	V2Dg<P> operator-(P scalar)const {
		return V2Dg<P>{ this->x - scalar, this->y - scalar };
	}
	friend V2Dg<P> operator*(P scalar, const V2Dg<P>& other) {
		return V2Dg<P>{ other.x* scalar, other.y* scalar };
	}
	V2Dg<P> operator*(P scalar)const {
		return V2Dg<P>{ this->x * scalar, this->y* scalar };
	}
	friend V2Dg<P> operator/(P scalar, const V2Dg<P>& other) {
		return V2Dg<P>{ other.x / scalar, other.y / scalar };
	}
	V2Dg<P> operator/(P scalar)const {
		return V2Dg<P>{ (P)(this->x / scalar), (P)(this->y / scalar) };
	}



	V2Dg<P> operator-() const {
		return -1 * (*this);
	}

	inline P mag2XY() const { return this->x * this->x + this->y * this->y; }
	inline P mag2() const { return this->x * this->x + this->y * this->y; }
	inline P mag() const { return (P)std::sqrt(this->mag2()); }
	inline float fmag() const { return std::sqrt(float(this->mag2())); }

	inline V2Dg<P> norm() const { P mag = this->mag(); return V2Dg<P>{ this->x / mag, this->y / mag }; }
	inline void normI() { P mag = this->mag(); this->x /= mag; this->y /= mag; }


	inline P dot(const V2Dg<P>& other) const { return this->x * other.x + this->y * other.y; }

	inline P max() const { return std::max(x, y); }
	inline P min() const { return std::min(x, y); }


	inline V2Dg<P> ceil() const { return V2Dg<P>{std::ceil(x), std::ceil(y)}; }
	inline V2Dg<P> floor() const { return V2Dg<P>{std::floor(x), std::floor(y)}; }
};
using V2D = V2Dg<float>;
using V2Di = V2Dg<int>;
using V2Di8 = V2Dg<char>;
using V2Du32 = V2Dg<unsigned int>;
using V2Du16 = V2Dg<unsigned short int>;
using V2Du8 = V2Dg<unsigned char>;

template<typename P>
class V3Dg {
public:
	V3Dg() = default;
	constexpr V3Dg(P x, P y, P z) : x(x), y(y), z(z) { }
	constexpr V3Dg(P val) : x(val), y(val), z(val) { }
	V3Dg(const V3Dg<P>&) = default;
	template <typename U>
	V3Dg(const V3Dg<U>& other) : x(static_cast<P>(other.x)), y(static_cast<P>(other.y)), z(static_cast<P>(other.z)) {
		static_assert(std::is_arithmetic<U>::value, "V3Dg<type> conversion must be numeric");
	}

	 //V3Dg(P x, P y, P z) : x(x), y(y), z(z) {}

	static_assert(std::is_arithmetic<P>::value, "V3Dg<type> must be numeric");
	
	~V3Dg() = default;
	P x = 0;
	P y = 0;
	P z = 0;

	inline V3Dg<P>& operator=(const V3Dg<P>& other) {
		this->x = other.x;
		this->y = other.y;
		this->z = other.z;
		return *this;
	}
	template <typename U>
	inline V3Dg<P>& operator=(const V3Dg<U>& other) {
		static_assert(std::is_arithmetic<U>::value, "V3Dg<type> conversion must be numeric");
		this->x = static_cast<U>(other.x);
		this->y = static_cast<U>(other.y);
		this->z = static_cast<U>(other.z);
		return *this;
	}

	template <typename U>
	inline bool operator==(const V3Dg<U>& other) {
		static_assert(std::is_arithmetic<U>::value, "V2Dg<type> conversion must be numeric");
		return this->x == other.x && this->y == other.y && this->z == other.z;
	}

	template<typename U>
	inline auto operator-(const V3Dg<U>& other) const {
		return V3Dg<std::common_type_t<P, U>>{ this->x - other.x, this->y - other.y, this->z - other.z };
	}
	inline V3Dg<P> operator+(const V3Dg<P>& other) const {
		return V3Dg<P>{(P)( this->x + other.x),(P)( this->y + other.y),(P)( this->z + other.z) };
	}
	inline V3Dg<P> operator+(const V2Dg<P>& other) const {
		return V3Dg<P>{ this->x + other.x, this->y + other.y, this->z };
	}
	inline V3Dg<P> operator*(const V3Dg<P>& other) const {
		return V3Dg<P>{ this->x* other.x, this->y* other.y , this->z* other.z };
	}
	inline V3Dg<P> operator/(const V3Dg<P>& other) const {
		return V3Dg<P>{ this->x / other.x, this->y / other.y, this->z / other.z };
	}


	inline friend V3Dg<P> operator+(P scalar, const V3Dg<P>& other) {
		return V3Dg<P>{ other.x + scalar, other.y + scalar, other.z + scalar };
	}
	inline V3Dg<P> operator+(P scalar) const {
		return V3Dg<P>{ this->x + scalar, this->y + scalar, this->z + scalar };
	}
	inline friend V3Dg<P> operator-(P scalar, const V3Dg<P>& other) {
		return V3Dg<P>{ other.x - scalar, other.y - scalar, other.z - scalar };
	}
	inline V3Dg<P> operator-(P scalar)const {
		return V3Dg<P>{ this->x - scalar, this->y - scalar, this->z - scalar };
	}
	inline friend V3Dg<P> operator*(P scalar, const V3Dg<P>& other) {
		return V3Dg<P>{ other.x* scalar, other.y* scalar, other.z* scalar };
	}
	inline V3Dg<P> operator*(P scalar)const {
		return V3Dg<P>{ this->x* scalar, this->y* scalar, this->z* scalar };
	}
	template<typename U>
	inline friend V3Dg<P> operator/(U scalar, const V3Dg<P>& other) {
		return V3Dg<P>{ (P)other.x / scalar, (P)other.y / scalar, (P)other.z / scalar };
	}
	template<typename U>
	inline V3Dg<P> operator/(U scalar)const {
		return V3Dg<P>{ static_cast<P>(this->x / scalar), static_cast<P>(this->y / scalar), static_cast<P>(this->z / scalar) };
	}



	inline V3Dg<P>& operator-=(const V3Dg<P>& other) {
		this->x -= other.x;
		this->y -= other.y;
		this->z -= other.z;
		return *this;
	}
	inline V3Dg<P>& operator+=(const V3Dg<P>& other) {
		this->x += other.x;
		this->y += other.y;
		this->z += other.z;
		return *this;
	}
	inline V3Dg<P>& operator*=(const V3Dg<P>& other) {
		this->x *= other.x;
		this->y *= other.y;
		this->z *= other.z;
		return *this;
	}
	inline V3Dg<P>& operator/=(const V3Dg<P>& other) {
		this->x /= other.x;
		this->y /= other.y;
		this->z /= other.z;
		return *this;
	}


	inline V3Dg<P> operator-() const {
		return -1 * (*this);
	}

	inline P mag2XY() const { return this->x * this->x + this->y * this->y ; }
	inline P mag2() const { return this->x * this->x + this->y * this->y + this->z * this->z; }
	inline P mag() const { return (P)std::sqrt(this->mag2()); }
	inline float fmag() const { return std::sqrt(float(this->mag2())); }

	inline V3Dg<P> norm() const { P mag = this->mag(); return V3Dg<P>{ this->x / mag, this->y / mag, this->z / mag }; }
	inline void normI() { P mag = this->mag(); this->x /= mag; this->y /= mag; this->z /= mag;}


	inline V3Dg<P> cross(const V3Dg<P>& other) const { return V3Dg<P>{ this->y * other.z - this->z * other.y, this->z * other.x - this->x * other.z, this->x * other.y - this->y * other.x }; }

	inline P dot(const V3Dg<P>& other) const { return this->x * other.x + this->y * other.y + this->z * other.z; }
	inline P dotE(const V3Dg<P> other) const { return this->x * other.x + this->y * other.y + this->z * other.z; }


	inline P max() const { return std::max(std::max(x, y), z); }
	inline P min() const { return std::min(std::min(x, y), z); }


	inline V3Dg<P> ceil() const { return V3Dg<P>{std::ceil(x), std::ceil(y), std::ceil(z)}; }
	inline V3Dg<P> floor() const { return V3Dg<P>{std::floor(x), std::floor(y), std::floor(z)}; }
};



using V3D = V3Dg<float>;
using V3Di = V3Dg<int>;
using V3Du32 = V3Dg<unsigned int>;
using V3Du16 = V3Dg<unsigned short int>;
using V3Du8 = V3Dg<unsigned char>;




// Specialize std::hash for V2Dg<U>
namespace std {
	template <>
	struct hash<V2Dg<int>> {
		size_t operator()(const V2Dg<int>& v) const noexcept {
			// Combine hashes (example combining method)
			return (size_t)v.x << 32 | v.y;
		}
	};
}
