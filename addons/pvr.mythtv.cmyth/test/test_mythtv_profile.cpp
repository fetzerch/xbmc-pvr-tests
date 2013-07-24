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

#include <unistd.h>

#include "mythtvfixture.h"

#include "pvrclient-mythtv.h"
#include "client.h"

using namespace ::std;
using namespace ::testing;

class DISABLED_MythTVProfile : public MythTVFixture
{
};

TEST_F(DISABLED_MythTVProfile, MythTVTestProfileGetChannels)
{
  m_myth->GetNumChannels();
  PVR_ERROR result = m_myth->GetChannels(NULL, false);
  EXPECT_EQ(result, PVR_ERROR_NO_ERROR);
}

TEST_F(DISABLED_MythTVProfile, MythTVTestProfileGetChannelGroups)
{
  m_myth->GetChannelGroupsAmount();
  PVR_ERROR result = m_myth->GetChannelGroups(NULL, false);
  EXPECT_EQ(result, PVR_ERROR_NO_ERROR);
}

TEST_F(DISABLED_MythTVProfile, MythTVTestProfileChannelGroupMembers)
{
  std::list<PVR_CHANNEL_GROUP> channelGroups = GetChannelGroups();
  std::list<PVR_CHANNEL_GROUP>::iterator channelGroupIt;
  for (channelGroupIt = channelGroups.begin(); channelGroupIt != channelGroups.end(); ++channelGroupIt)
  {
      m_myth->GetChannelGroupMembers(NULL, *channelGroupIt);
  }
}

TEST_F(DISABLED_MythTVProfile, MythTVTestProfileGetRecordings)
{
  m_myth->GetRecordingsAmount();
  PVR_ERROR result = m_myth->GetRecordings(NULL);
  EXPECT_EQ(result, PVR_ERROR_NO_ERROR);
}

TEST_F(DISABLED_MythTVProfile, MythTVTestProfileGetTimers)
{
  m_myth->GetTimersAmount();
  PVR_ERROR result = m_myth->GetTimers(NULL);
  EXPECT_EQ(result, PVR_ERROR_NO_ERROR);
}
