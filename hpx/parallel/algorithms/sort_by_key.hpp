//  Copyright (c) 2015 John Biddiscombe
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(HPX_PARALLEL_ALGORITHM_SORT_BY_KEY_DEC_2015)
#define HPX_PARALLEL_ALGORITHM_SORT_BY_KEY_DEC_2015

#include <hpx/parallel/algorithms/sort.hpp>
#include <hpx/parallel/algorithms/detail/tuple_iterator.hpp>

namespace hpx { namespace parallel { HPX_INLINE_NAMESPACE(v1)
{
    ///////////////////////////////////////////////////////////////////////////
    // sort
    namespace detail
    {
        /// \cond NOINTERNAL
/*
        template <typename... T>
        auto sort_zip(T&... containers)
        -> boost::iterator_range<decltype(hpx::iterators::makeTupleIterator(std::begin(containers)...))> {
            return boost::make_iterator_range(hpx::iterators::makeTupleIterator(std::begin(containers)...),
                                              hpx::iterators::makeTupleIterator(std::end(containers)...));
        }
*/
        /// \endcond
    }

    //-----------------------------------------------------------------------------
    /// Sorts the elements in the range [first, last) in ascending order. The
    /// order of equal elements is not guaranteed to be preserved. The function
    /// uses the given comparison function object comp (defaults to using
    /// operator<()).
    ///
    /// \note   Complexity: O(Nlog(N)), where N = std::distance(first, last)
    ///                     comparisons.
    ///
    /// A sequence is sorted with respect to a comparator \a comp and a
    /// projection \a proj if for every iterator i pointing to the sequence and
    /// every non-negative integer n such that i + n is a valid iterator
    /// pointing to an element of the sequence, and
    /// INVOKE(comp, INVOKE(proj, *(i + n)), INVOKE(proj, *i)) == false.
    ///
    /// \tparam ExPolicy    The type of the execution policy to use (deduced).
    ///                     It describes the manner in which the execution
    ///                     of the algorithm may be parallelized and the manner
    ///                     in which it applies user-provided function objects.
    /// \tparam Iter        The type of the source iterators used (deduced).
    ///                     This iterator type must meet the requirements of a
    ///                     random access iterator.
    /// \tparam Comp        The type of the function/function object to use
    ///                     (deduced).
    /// \tparam Proj        The type of an optional projection function. This
    ///                     defaults to \a util::projection_identity
    ///
    /// \param policy       The execution policy to use for the scheduling of
    ///                     the iterations.
    /// \param first        Refers to the beginning of the sequence of elements
    ///                     the algorithm will be applied to.
    /// \param last         Refers to the end of the sequence of elements the
    ///                     algorithm will be applied to.
    /// \param comp         comp is a callable object. The return value of the
    ///                     INVOKE operation applied to an object of type Comp,
    ///                     when contextually converted to bool, yields true if
    ///                     the first argument of the call is less than the
    ///                     second, and false otherwise. It is assumed that comp
    ///                     will not apply any non-constant function through the
    ///                     dereferenced iterator.
    /// \param proj         Specifies the function (or function object) which
    ///                     will be invoked for each pair of elements as a
    ///                     projection operation before the actual predicate
    ///                     \a comp is invoked.
    ///
    /// \a comp has to induce a strict weak ordering on the values.
    ///
    /// The application of function objects in parallel algorithm
    /// invoked with an execution policy object of type
    /// \a sequential_execution_policy execute in sequential order in the
    /// calling thread.
    ///
    /// The application of function objects in parallel algorithm
    /// invoked with an execution policy object of type
    /// \a parallel_execution_policy or \a parallel_task_execution_policy are
    /// permitted to execute in an unordered fashion in unspecified
    /// threads, and indeterminately sequenced within each thread.
    ///
    /// \returns  The \a sort algorithm returns a
    ///           \a hpx::future<Iter> if the execution policy is of
    ///           type
    ///           \a sequential_task_execution_policy or
    ///           \a parallel_task_execution_policy and returns \a Iter
    ///           otherwise.
    ///           It returns \a last.
    //-----------------------------------------------------------------------------

    template <typename Proj = util::projection_identity,
      typename ExPolicy, typename RandomIt, typename RandomIt2,
      typename Compare = std::less<
        typename std::remove_reference<
          typename traits::projected_result_of<Proj,
            hpx::iterators::TupleIt<typename boost::tuples::tuple<RandomIt, RandomIt2> >
          >::type
        >::type
      >
    >

    typename util::detail::algorithm_result<ExPolicy, void>::type
    sort_by_key(
        ExPolicy && policy,
        RandomIt key_first, RandomIt key_last,
        RandomIt2 values_first,
        Compare && comp = Compare(), Proj && proj = Proj())
    {
        typedef typename std::iterator_traits<RandomIt>::iterator_category
            iterator_category;
        typedef typename std::iterator_traits<RandomIt2>::iterator_category
            iterator_category2;

        static_assert(
            (boost::is_base_of<
                std::random_access_iterator_tag, iterator_category
            >::value) ||
            (boost::is_base_of<
                std::random_access_iterator_tag, iterator_category2
            >::value),
            "Requires a random access iterator.");

        typedef is_sequential_execution_policy<ExPolicy> is_seq;

        RandomIt2 values_last = values_first;
        std::advance(values_last, std::distance(key_first, key_last));
        auto begins = hpx::iterators::makeTupleIterator(key_first, values_first);
        auto ends   = hpx::iterators::makeTupleIterator(key_last, values_last);

        return parallel::sort(
          std::forward<ExPolicy>(policy), begins, ends, std::forward<Compare>(comp), std::forward<Proj>(proj)  );
    }
}}}

#endif
