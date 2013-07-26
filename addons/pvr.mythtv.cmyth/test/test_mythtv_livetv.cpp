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

class MythTVLiveTV : public MythTVFixture
{
};

TEST_F(MythTVLiveTV, MythTVTestLiveTVChannelNotFound)
{
  PVR_CHANNEL channel;
  memset(&channel, 0, sizeof(PVR_CHANNEL));

  // Start playback
  bool res = m_myth->OpenLiveStream(channel);
  EXPECT_FALSE(res);

  if (res)
    m_myth->CloseLiveStream();
}

//int GetCurrentClientChannel();
//bool SwitchChannel(const PVR_CHANNEL &channelinfo);
//PVR_ERROR SignalStatus(PVR_SIGNAL_STATUS &signalStatus);

TEST_F(MythTVLiveTV, MythTVTestLiveTV)
{
  // Get channel
  PVR_CHANNEL channel = *GetChannels().begin();

  // We should receive file size updates for the Live TV recording
  EXPECT_CALL(*m_xbmc_pvr, TransferRecordingEntry(_, _))
      .Times(AtLeast(1));

  // Start playback
  bool res = m_myth->OpenLiveStream(channel);
  EXPECT_TRUE(res);

  // Position
  long long pos = m_myth->SeekLiveStream(0, SEEK_SET);
  EXPECT_EQ(pos, 0);

  // Read length
  long long length = m_myth->LengthLiveStream();
  EXPECT_GT(length, 0);

  // Read packets
  int len_read;
  unsigned char buffer[32*1024];
  for (int i = 0; i < 5; i++)
  {
    len_read = m_myth->ReadLiveStream(buffer, sizeof(buffer));
    EXPECT_LE(len_read, (int)sizeof(buffer));

    sleep(1); // TODO: Measure XBMC's real buffersize, and read frequency
  }

  // Close stream
  m_myth->CloseLiveStream();

  Mock::VerifyAndClear(m_xbmc_pvr);
}
