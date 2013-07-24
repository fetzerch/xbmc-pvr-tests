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
#include <mysql++/mysql++.h>

#include "mythtvfixture.h"

#include "pvrclient-mythtv.h"
#include "client.h"

using namespace ::std;
using namespace ::testing;

class MythTVChannels : public MythTVFixture
{
};

class ChannelChecker
{
public:
  ChannelChecker() : m_conn(mysqlpp::Connection(false))
  {
    m_valid = m_conn.connect(g_szDBName.c_str(), g_szDBHostname.c_str(),
                             g_szDBUser.c_str(), g_szDBPassword.c_str(), g_iDBPort);

    if (m_valid)
    {
      mysqlpp::Query query1 = m_conn.query("SET NAMES 'utf8'");
      query1.execute();
      mysqlpp::Query query = m_conn.query("SELECT * from channel");
      m_valid = m_res = query.store();
      m_it = m_res.begin();
    }
  }

  int numChannels()
  {
    return m_res.size();
  }

  void checkTransferChannelEntry(const PVR_CHANNEL *entry)
  {
    EXPECT_NE(m_it, m_res.end());

    // TODO: Check values depends on channel order
    //cout << (*m_it)["callsign"] << endl;
    //cout << entry->iUniqueId << entry->bIsRadio << entry->iChannelNumber << entry->strChannelName << entry->strIconPath << entry->bIsHidden << endl;

    // Check unimplemented values
    EXPECT_STREQ(entry->strInputFormat, "");
    EXPECT_STREQ(entry->strStreamURL, "");
    EXPECT_EQ(entry->iEncryptionSystem, 0u);

    // TODO
    // iUniqueId
    // bIsRadio
    // iChannelNumber
    // strChannelName
    // strIconPath
    // bIsHidden

    ++m_it;
  }

private:
  bool m_valid;
  mysqlpp::StoreQueryResult m_res;
  mysqlpp::Connection m_conn;
  mysqlpp::StoreQueryResult::const_iterator m_it;
};

TEST_F(MythTVChannels, MythTVGetNumChannels)
{
  int channelsAmount = m_myth->GetNumChannels();
  ChannelChecker checker;
  EXPECT_EQ(checker.numChannels(), channelsAmount);

  // Also the number of calls to TransferChannelEntry must match the
  // number of channels.
  EXPECT_CALL(*m_xbmc_pvr, TransferChannelEntry(_, _))
      .Times(channelsAmount);
  PVR_ERROR result = m_myth->GetChannels(NULL, false);
  EXPECT_EQ(result, PVR_ERROR_NO_ERROR);
}

TEST_F(MythTVChannels, MythTVGetChannels) {
  ChannelChecker checker;
  EXPECT_CALL(*m_xbmc_pvr, TransferChannelEntry(_, _))
      .WillRepeatedly(WithArgs<1>(Invoke(&checker, &ChannelChecker::checkTransferChannelEntry)));
  EXPECT_CALL(*m_xbmc_pvr, TriggerChannelUpdate())
      .Times(0);
  PVR_ERROR result = m_myth->GetChannels(NULL, false);
  EXPECT_EQ(result, PVR_ERROR_NO_ERROR);
}

TEST_F(MythTVChannels, MythTVGetChannelGroupsAmount) {
  int channelGroupsAmount = m_myth->GetChannelGroupsAmount();
  EXPECT_GT(channelGroupsAmount, 0);
}

TEST_F(MythTVChannels, MythTVGetChannelGroups) {
  PVR_ERROR result = m_myth->GetChannelGroups(NULL, false);
  EXPECT_EQ(result, PVR_ERROR_NO_ERROR);
}

TEST_F(MythTVChannels, MythTVGetChannelGroupMembers)
{
  std::list<PVR_CHANNEL_GROUP> channelGroups = GetChannelGroups();
  std::list<PVR_CHANNEL_GROUP>::iterator channelGroupIt;
  for (channelGroupIt = channelGroups.begin(); channelGroupIt != channelGroups.end(); ++channelGroupIt)
  {
    m_myth->GetChannelGroupMembers(NULL, *channelGroupIt);
  }
}
