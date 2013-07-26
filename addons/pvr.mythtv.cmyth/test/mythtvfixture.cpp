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

#include "mythtvfixture.h"
#include "transferutils.h"

#include "libXBMC_addon.h"
#include "libXBMC_pvr.h"

#include "client.h"
#include "pvrclient-mythtv.h"

using namespace ::std;
using namespace ::testing;

static void log(const ADDON::addon_log_t loglevel, char *buffer)
{
  (void)loglevel;
  cout << buffer << endl;
}

static void notify(const ADDON::queue_msg_t type, char *buffer)
{
  (void)type;
  cout << buffer << endl;
}

MythTVFixture::MythTVFixture()
{
}

MythTVFixture::~MythTVFixture()
{
}

void MythTVFixture::SetUp()
{
  // The expectations on the xbmc libs can be reconfigured
  // in the testcases, if needed.

  if (g_bExtraDebug)
    enableLoggingAndNotifications();
  else
    disableLoggingAndNotifications();
  disableFileCaching();
}

void MythTVFixture::TearDown()
{
}

void MythTVFixture::SetUpTestCase()
{
  // Initialize global variables used by PVRClientMythTV
  // normally, those would be loaded in client.cpp.

  m_xbmc_addon = new ADDON::CHelper_libXBMC_addon();
  XBMC = m_xbmc_addon;

  m_xbmc_pvr = new CHelper_libXBMC_pvr();
  PVR = m_xbmc_pvr;

  // We don't care about those functions beeing called
  // when initializing the test case.
  disableLoggingAndNotifications();
  disableFileCaching();

  // Establish MythTV connection
  m_myth = new PVRClientMythTV();
  bool connected = m_myth->Connect();
  ASSERT_TRUE(connected);
}

void MythTVFixture::TearDownTestCase()
{
  delete m_myth;
  m_myth = NULL;

  delete m_xbmc_addon;
  m_xbmc_addon = NULL;
  XBMC = NULL;

  delete m_xbmc_pvr;
  m_xbmc_pvr = NULL;
  PVR = NULL;
}

void MythTVFixture::disableLoggingAndNotifications()
{
  // Disable log and notification warning
  EXPECT_CALL(*m_xbmc_addon, MockedLog(_, _))
        .Times(AnyNumber());
  EXPECT_CALL(*m_xbmc_addon, MockedQueueNotification(_, _))
        .Times(AnyNumber());
}

void MythTVFixture::enableLoggingAndNotifications()
{
  // Disable log and notification warning
  EXPECT_CALL(*m_xbmc_addon, MockedLog(_, _))
        .WillRepeatedly(Invoke(log));
  EXPECT_CALL(*m_xbmc_addon, MockedQueueNotification(_, _))
        .WillRepeatedly(Invoke(notify));
}

void MythTVFixture::disableFileCaching()
{
  // Disable caching warning
  EXPECT_CALL(*m_xbmc_addon, DirectoryExists(_))
        .Times(AnyNumber())
        .WillRepeatedly(Return(false));
  EXPECT_CALL(*m_xbmc_addon, CreateDirectory(_))
        .Times(AnyNumber())
        .WillRepeatedly(Return(false));
  EXPECT_CALL(*m_xbmc_addon, FileExists(_, _))
        .Times(AnyNumber())
        .WillRepeatedly(Return(false));
  EXPECT_CALL(*m_xbmc_addon, OpenFileForWrite(_, _))
        .Times(AnyNumber())
        .WillRepeatedly(ReturnNull());
}

std::list<PVR_CHANNEL> MythTVFixture::GetChannels()
{
  TransferUtil<PVR_CHANNEL> transfer;
  EXPECT_CALL(*m_xbmc_pvr, TransferChannelEntry(_, _))
      .WillRepeatedly(WithArgs<1>(Invoke(&transfer, &TransferUtil<PVR_CHANNEL>::Add)));
  m_myth->GetChannels(NULL, false);
  return transfer.GetList();
}

std::list<PVR_CHANNEL_GROUP> MythTVFixture::GetChannelGroups()
{
  TransferUtil<PVR_CHANNEL_GROUP> transfer;
  EXPECT_CALL(*m_xbmc_pvr, TransferChannelGroup(_, _))
      .WillRepeatedly(WithArgs<1>(Invoke(&transfer, &TransferUtil<PVR_CHANNEL_GROUP>::Add)));
  m_myth->GetChannelGroups(NULL, false);
  return transfer.GetList();
}

std::list<PVR_RECORDING> MythTVFixture::GetRecordings()
{
  TransferUtil<PVR_RECORDING> transfer;
  EXPECT_CALL(*m_xbmc_pvr, TransferRecordingEntry(_, _))
      .WillRepeatedly(WithArgs<1>(Invoke(&transfer, &TransferUtil<PVR_RECORDING>::Add)));
  EXPECT_CALL(*m_xbmc_pvr, TriggerRecordingUpdate())
      .Times(AnyNumber());
  m_myth->GetRecordings(NULL);
  return transfer.GetList();
}

std::list<PVR_TIMER> MythTVFixture::GetTimers()
{
  TransferUtil<PVR_TIMER> transfer;
  EXPECT_CALL(*m_xbmc_pvr, TransferTimerEntry(_, _))
      .WillRepeatedly(WithArgs<1>(Invoke(&transfer, &TransferUtil<PVR_TIMER>::Add)));
  m_myth->GetTimers(NULL);
  return transfer.GetList();
}

PVRClientMythTV *MythTVFixture::m_myth = NULL;
ADDON::CHelper_libXBMC_addon *MythTVFixture::m_xbmc_addon = NULL;
CHelper_libXBMC_pvr *MythTVFixture::m_xbmc_pvr = NULL;
