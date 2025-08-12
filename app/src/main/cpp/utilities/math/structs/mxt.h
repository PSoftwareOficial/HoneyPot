#pragma once
#include "vxd.h"



//Matrix structure that works with Matrix-Vector Multiplication on the left -> vA
template <typename P, size_t N>
struct Matrix {
	static_assert(N > 0);
	//Init all to 0
	P el[N][N] = {};

	
	//No index safety nothing. Pure wrapper;
	template<typename U>
	inline auto operator[](U index) {
		return el[index];
	}
	template<typename U>
	inline auto operator[](U index) const {
		return el[index];
	}


	inline Matrix<P, N> T() const {
		Matrix<P, N> temp;
		for (uint8_t r = 0; r < N; ++r) {
			for (uint8_t c = 0; c < r; ++c) {
				temp[r][c] = el[c][r];
				temp[c][r] = el[r][c];
			}
			temp[r][r] = el[r][r];
		}

	}
	
	constexpr static Matrix identity() {
        Matrix result;
        for (size_t i = 0; i < N; ++i)
            	result.el[i][i] = P(1);
        return result;
    	}
};


using M4 = Matrix<float, 4>;
