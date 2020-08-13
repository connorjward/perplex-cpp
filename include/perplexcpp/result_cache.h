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
      const size_t capacity;


      /**
       * ???
       */
      ResultCache(const size_t capacity, const double rtol=0.0);


      /**
       * ???
       */
      int
      get(const double pressure,
	  const double temperature,
	  const std::vector<double> &composition,
	  MinimizeResult &out);


      /**
       * ???
       */
      void
      put(const MinimizeResult& result);


      /**
       * @return The size of the cache.
       */
      size_t
      size() const;


      /**
       * Reset the hit and miss counters to zero.
       */
      void
      reset_counters();


      inline unsigned int
      get_n_hits() const { return this->n_hits; }


      inline unsigned int
      get_n_misses() const { return this->n_misses; }

    private:

      /**
       * ???
       */
      const double rtol;


      /**
       * The number of hits.
       */
      unsigned int n_hits = 0;


      /**
       * The number of misses.
       */
      unsigned int n_misses = 0;


      /**
       * List of the items stored in the cache. A linked list rather than vector is
       * used for efficient reordering.
       */
      std::list<MinimizeResult> items;


      /**
       * ???
       */
      bool 
      is_near_enough(const double x, const double y) const;


      /**
       * ???
       */
      bool 
      is_near_enough(const std::vector<double> xs, 
                     const std::vector<double> ys) const;
  };
}


#endif
