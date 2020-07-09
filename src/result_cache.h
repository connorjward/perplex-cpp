/*
 * Copyright (C) 2020 Connor Ward.
 *
 * This file is part of PerpleX-cpp.
 *
 * PerpleX-cpp is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PerpleX-cpp is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PerpleX-cpp.  If not, see <https://www.gnu.org/licenses/>.
 */


#ifndef PERPLEXCPP_RESULTCACHE_H
#define PERPLEXCPP_RESULTCACHE_H


#include <list>
#include <vector>

#include <perplexcpp/base.h>


namespace perplexcpp
{
  /**
   * ???
   */
  class ResultCache
  {
    public:

      /**
       * ???
       */
      ResultCache(const size_t capacity, const double rtol=0.0);


      /**
       * ???
       */
      const MinimizeResult *const
      get(const double pressure,
	  const double temperature,
	  const std::vector<double>& composition);


      /**
       * ???
       */
      void
      put(const MinimizeResult& result);


    private:

      /**
       * ???
       */
      const double rtol;


      /**
       * ???
       */
      std::list<MinimizeResult> items;


      /**
       * ???
       */
      bool 
      is_near_enough(const double x, const double y);


      /**
       * ???
       */
      bool 
      is_near_enough(const std::vector<double> xs, 
                     const std::vector<double> ys);
  };
}


#endif
