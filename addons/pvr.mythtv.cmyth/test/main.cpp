/*
 *  Copyright (C) 2013 Christian Fetzer
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */

#include <gtest/gtest.h>

#include <client.h>

static char *getCmdOption(char **begin, char **end, const std::string &option)
{
  char **itr = std::find(begin, end, option);
  if (itr != end && ++itr != end)
  {
    return *itr;
  }
  return NULL;
}

static bool cmdOptionExists(char **begin, char **end, const std::string &option)
{
  return std::find(begin, end, option) != end;
}

int main(int argc, char** argv) {
  ::testing::InitGoogleMock(&argc, argv);

  char *mythtv_host = getCmdOption(argv, argv + argc, "--mythtvhost");
  if (mythtv_host)
  {
    g_szMythHostname = mythtv_host;
  }
  g_szDBHostname = g_szMythHostname;

  if (cmdOptionExists(argv, argv + argc, "-v"))
      g_bExtraDebug = true;

  return RUN_ALL_TESTS();
}
