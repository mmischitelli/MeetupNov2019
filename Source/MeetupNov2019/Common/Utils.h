#pragma once

namespace UDataStructs
{
	template<typename T>
	struct TRange
	{
		T Min;
		T Max;
	};
}

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
		for (int i = 0; i < n; ++i, ++first) {
			*first = g();
		}
		return first;
	}

	/**
	 * @brief Equivalent of std::for_each
	 * @tparam ForwardIt Forward iterator of the container you want to iterate over
	 * @tparam Function Function to call for each element of the container
	 * @param first Start of the sequence where f will be called upon
	 * @param last Last element of the sequence (excluded)
	 * @param f Function that will get called on the container's elements
	 */
	template<class ForwardIt, class Function>
	void TForEach(ForwardIt first, ForwardIt last, Function f)
	{
		while(first != last) {
			f(*first++);
		}
	}

	/**
	 * @brief Equivalent of std::for_each_n
	 * @tparam ForwardIt Forward iterator of the container you want to iterate over
	 * @tparam Function Function to call for each element of the container
	 * @param first Start of the sequence where f will be called upon
	 * @param n Number of elements to iterate over, starting from first
	 * @param f Function that will get called on the container's elements
	 */
	template<class ForwardIt, class Function>
	ForwardIt TForEach_N(ForwardIt first, int n, Function f)
	{
		for (int i = 0; i < n; ++i, ++first) {
			f(*first);
		}
		return first;
	}

	/**
	 * @brief Equivalent of std::accumulate
	 * @tparam ForwardIt Forward iterator of the container you want to iterate over
	 * @tparam T Type of the value that will be accumulated
	 * @param first Start of the range of elements to sum
	 * @param last End of the range of elements to sum (excluded)
	 * @param init Initial value of the sum
	 */
	template<class ForwardIt, class T>
	T TAccumulate(ForwardIt first, ForwardIt last, T init)
	{
		for (; first != last; ++first) {
			init = init + *first;
		}
		return init;
	}
	
	/**
	 * @brief Equivalent of std::accumulate_n
	 * @tparam ForwardIt Forward iterator of the container you want to iterate over
	 * @tparam T Type of the value that will be accumulated
	 * @param first Start of the range of elements to sum
	 * @param n Number of the container's elements to sum
	 * @param init Initial value of the sum
	 */
	template<class ForwardIt, class T>
	T TAccumulate_N(ForwardIt first, int n, T init)
	{
		for (int i = 0; i < n; ++i, ++first) {
			init = init + *first;
		}
		return init;
	}

	/**
	 * @brief Equivalent of std::accumulate
	 * @tparam ForwardIt Forward iterator of the container you want to iterate over
	 * @tparam T Type of the value that will be accumulated
	 * @param first Start of the range of elements to accumulate
	 * @param last End of the range of elements to accumulate (excluded)
	 * @param init Initial value of the accumulation
	 * @param op Binary operation that will be applied between current accumulated value and the next element in the accumulation range
	 */
	template<class ForwardIt, class T, class BinaryOperation>
	T TAccumulate(ForwardIt first, ForwardIt last, T init, BinaryOperation op)
	{
		for (; first != last; ++first) {
			init = op(init, *first);
		}
		return init;
	}

	/**
	 * @brief Equivalent of std::accumulate_n
	 * @tparam ForwardIt Forward iterator of the container you want to iterate over
	 * @tparam T Type of the value that will be accumulated
	 * @param first Start of the range of elements to accumulate
	 * @param n Number of the container's elements to accumulate
	 * @param init Initial value of the accumulation
	 * @param op Binary operation that will be applied between current accumulated value and the next element in the accumulation range
	 */
	template<class ForwardIt, class T, class BinaryOperation>
	T TAccumulate_N(ForwardIt first, int n, T init, BinaryOperation op)
	{
		for (int i = 0; i < n; ++i, ++first) {
				init = op(init, *first);
		}
		return init;
	}

	template<class InputIt, class OutputIt, class UnaryOperation>
	OutputIt TTransform(InputIt firstInput, InputIt lastInput, OutputIt firstOutput, UnaryOperation op)
	{
		while (firstInput != lastInput) {
			*firstOutput++ = op(*firstInput++);
		}
		return firstOutput;
	}
	template<class InputIt, class OutputIt, class UnaryOperation>
	OutputIt TTransform_N(InputIt firstInput, int n, OutputIt firstOutput, UnaryOperation op)
	{
		for (int i = 0; i < n; ++i, ++firstInput) {
			*firstOutput++ = op(*firstInput);
		}
		return firstOutput;
	}
}