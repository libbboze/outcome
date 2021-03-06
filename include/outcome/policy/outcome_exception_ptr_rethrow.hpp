/* Policies for result and outcome
(C) 2017 Niall Douglas <http://www.nedproductions.biz/> (59 commits)
File Created: Oct 2017


Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License in the accompanying file
Licence.txt or at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.


Distributed under the Boost Software License, Version 1.0.
(See accompanying file Licence.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef OUTCOME_POLICY_OUTCOME_EXCEPTION_PTR_RETHROW_HPP
#define OUTCOME_POLICY_OUTCOME_EXCEPTION_PTR_RETHROW_HPP

#include "result_exception_ptr_rethrow.hpp"

OUTCOME_V2_NAMESPACE_EXPORT_BEGIN

namespace policy
{
  /*! Policy interpreting `EC` or `E` as a type for which `trait::has_exception_ptr_v<EC|E>` is true.
  Any wide attempt to access the successful state where there is none causes:
  `std::rethrow_exception(policy::exception_ptr(.error()|.exception()))` appropriately.
  */
  template <class T, class EC, class E> struct exception_ptr_rethrow : base
  {
    /*! Performs a wide check of state, used in the value() functions
    \effects If outcome does not have a value, if it has an exception it rethrows that exception via `std::rethrow_exception()`,
    if it has an error it rethrows that error via `std::rethrow_exception()`, else it throws `bad_outcome_access`.
    */
    template <class Impl> static constexpr void wide_value_check(Impl &&self)
    {
      if(!base::_has_value(std::forward<Impl>(self)))
      {
        if(base::_has_exception(std::forward<Impl>(self)))
        {
          detail::_rethrow_exception<trait::is_exception_ptr_available<E>::value>{base::_exception<T, EC, E, exception_ptr_rethrow>(std::forward<Impl>(self))};
        }
        if(base::_has_error(std::forward<Impl>(self)))
        {
          detail::_rethrow_exception<trait::is_exception_ptr_available<EC>::value>{base::_error(std::forward<Impl>(self))};
        }
        OUTCOME_THROW_EXCEPTION(bad_outcome_access("no value"));  // NOLINT
      }
    }
    /*! Performs a wide check of state, used in the error() functions
    \effects If outcome does not have an error, it throws `bad_outcome_access`.
    */
    template <class Impl> static constexpr void wide_error_check(Impl &&self)
    {
      if(!base::_has_error(std::forward<Impl>(self)))
      {
        OUTCOME_THROW_EXCEPTION(bad_outcome_access("no error"));  // NOLINT
      }
    }
    /*! Performs a wide check of state, used in the exception() functions
    \effects If result does not have an exception, it throws `bad_outcome_access`.
    */
    template <class Impl> static constexpr void wide_exception_check(Impl &&self)
    {
      if(!base::_has_exception(std::forward<Impl>(self)))
      {
        OUTCOME_THROW_EXCEPTION(bad_outcome_access("no exception"));  // NOLINT
      }
    }
  };
}  // namespace policy

OUTCOME_V2_NAMESPACE_END

#endif
