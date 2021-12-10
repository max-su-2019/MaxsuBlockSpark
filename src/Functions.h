#pragma once
namespace MaxsuBlockSpark
{
	class SparkLocalizer
	{
		using shieldEdg = std::pair<RE::NiAVObject*, RE::NiPoint3>;
	public:
		static bool GetShieldSparkPos(const RE::NiPoint3& hitPos, RE::NiAVObject* shieldNode, RE::NiPoint3& result);

	private:
		template <typename T>
		struct matrix_size;

		template <typename T, size_t N>
		struct matrix_size<T[N][N]>
		{
			typedef T type;
			static size_t const value = N;
		};
		 
		template <typename Array>
		static bool InverseMatrix(Array& out, Array const& in_)
		{
			typedef typename matrix_size<Array>::type type;
			size_t const n = matrix_size<Array>::value;
			Array in;
			// 复制输入，并初始化out
			for (size_t i = 0; i != n; ++i) {
				for (size_t j = 0; j != n; ++j) {
					in[i][j] = in_[i][j];
					out[i][j] = 0;
				}
				out[i][i] = 1;	// 将out赋值为单位阵
			}

			for (size_t c = 0; c != n; ++c) {
				// 选取列主元，这样算法更稳定
				size_t rr = 0;
				type maximum = 0;
				for (size_t r = c; r != n; ++r) {
					type const tmp = std::abs(in[r][c]);
					if (tmp > maximum) {
						maximum = tmp;
						rr = r;
					}
				}
				if (maximum == 0)
					return false;  // 不可逆

				// 交换c, rr两行
				for (size_t i = c; i != n; ++i) std::swap(in[c][i], in[rr][i]);
				for (size_t i = 0; i != n; ++i) std::swap(out[c][i], out[rr][i]);

				// 正规化
				for (size_t i = c + 1; i != n; ++i) {
					out[c][i] /= in[c][c];
				}
				for (size_t i = 0; i != n; ++i) {
					out[c][i] /= in[c][c];
				}

				// 行变换消元
				in[c][c] = 0;
				for (size_t r = 0; r != n; ++r) {
					for (size_t i = 0; i != n; ++i) {
						in[r][i] += in[c][i] * -in[r][c];
						out[r][i] += out[c][i] * -in[r][c];
					}
				}
			}

			return true;
		}
	};

}