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
#include <gmock/gmock.h>

#include "mythtvfixture.h"

#include "pvrclient-mythtv.h"
#include "client.h"

using namespace ::std;
using namespace ::testing;

class MythTVConnection : public MythTVFixture
{
};

TEST_F(MythTVConnection, MythTVTestBackendInformation)
{
  const char *backendName = m_myth->GetBackendName();
  EXPECT_STRNE(backendName, "");

  char *str30100 = const_cast<char *>("%d - %d");
  EXPECT_CALL(*m_xbmc_addon, GetLocalizedString(30100)).WillOnce(Return(str30100));
  const char *backendVersion = m_myth->GetBackendVersion();
  EXPECT_STRNE(backendVersion, "");

  const char *connectionString = m_myth->GetConnectionString();
  EXPECT_STRNE(connectionString, "");

  long long total, used;
  bool driveSpace = m_myth->GetDriveSpace(&total, &used);
  EXPECT_TRUE(driveSpace);
  EXPECT_GE(total, 0);
  EXPECT_GE(used, 0);
}
