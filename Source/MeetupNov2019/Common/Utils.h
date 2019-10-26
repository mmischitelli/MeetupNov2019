#pragma once

namespace UStd
{
	/**
	 * @brief Equivalent of std::generate
	 * @tparam ForwardIt Forward iterator of the container you want to generate into
	 * @tparam Generator Generator function invoked to generate elements in the container
	 * @param first Start of the sequence to generate
	 * @param last Last element of the sequence (excluded)
	 * @param g Generator function
	 */
	template<class ForwardIt, class Generator>
	void TGenerate(ForwardIt first, ForwardIt last, Generator g)
	{
		while (first != last) {
			*first++ = g();
		}
	}
	/**
	 * @brief Equivalent of std::generate_n
	 * @tparam ForwardIt Forward iterator of the container you want to generate into
	 * @tparam Generator Generator function invoked to generate elements in the container
	 * @param first Start of the sequence to generate
	 * @param n Number of elements to generate, starting from first
	 * @param g Generator function
	 */
	template<class ForwardIt, class Generator>
	ForwardIt TGenerate_N(ForwardIt first, int n, Generator g)
	{
		for (int i = 0; i < n; ++i) {
			*first++ = g();
		}
		return first;
	}
}