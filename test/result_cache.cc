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


#include "result_cache.h"

#include <gtest/gtest.h>


using namespace perplexcpp;


TEST(ResultCacheTest, GetReturnsNullPtrWithoutRtol)
{
  auto cache = ResultCache(3);
  cache.put(2.3, 3.1, std::vector<double>(2, 4.7));

  auto res = cache.get(2.2, 3.1, std::vector<double>(2, 4.7));

  EXPECT_TRUE(res == nullptr);
}



TEST(ResultCacheTest, GetReturnsNullPtrWithRtol)
{
  auto cache = ResultCache(3, 0.1);
  cache.put(2.3, 3.1, std::vector<double>(2, 4.7));

  auto res = cache.get(2.9, 3.1, std::vector<double>(2, 4.7));

  EXPECT_TRUE(res == nullptr);
}



TEST(ResultCacheTest, GetReturnsExactMatch)
{
  auto cache = ResultCache(3);
  cache.put(2.3, 3.1, std::vector<double>(2, 4.7));

  auto res = cache.get(2.3, 3.1, std::vector<double>(2, 4.7));

  EXPECT_TRUE(res != nullptr);
}



TEST_F(ResultCacheTest, GetReturnsNearMatch)
{

}



TEST_F(ResultCacheTest, GetUpdatesItemOrder)
{

}



TEST_F(ResultCacheTest, PutRemovesLastItem)
{

}



TEST_F(ResultCacheTest, PutAddsItemToStart)
{

}
