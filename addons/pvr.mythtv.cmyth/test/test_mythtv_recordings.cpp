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

#include <unistd.h>

#include "mythtvfixture.h"

#include "pvrclient-mythtv.h"
#include "client.h"

using namespace ::std;
using namespace ::testing;

class MythTVRecordings : public MythTVFixture
{
};

class RecordingsChecker
{
public:
  RecordingsChecker() : m_conn(mysqlpp::Connection(false))
  {
    m_valid = m_conn.connect(g_szDBName.c_str(), g_szDBHostname.c_str(),
                             g_szDBUser.c_str(), g_szDBPassword.c_str(), g_iDBPort);

    if (m_valid)
    {
      mysqlpp::Query query1 = m_conn.query("SET NAMES 'utf8'");
      query1.execute();
      mysqlpp::Query query = m_conn.query("SELECT * from recorded ORDER BY chanid, starttime");
      m_valid = m_res = query.store();
      m_it = m_res.begin();
    }
  }

  int numRecordings()
  {
    return m_res.size();
  }

  void checkTransferRecordingEntry(const PVR_RECORDING *entry)
  {
    EXPECT_NE(m_it, m_res.end());

    // RecordingId
    EXPECT_STRNE(entry->strRecordingId, "");

    // Title
    string title;
    (*m_it)["title"].to_string(title);
    string subtitle;
    (*m_it)["subtitle"].to_string(subtitle);
    if (!subtitle.empty())
      title.append(" - ").append(subtitle);
    EXPECT_EQ(entry->strTitle, title);

    // PlayCount
    int playcount = (int)((*m_it)["watched"]);
    EXPECT_EQ(entry->iPlayCount, playcount);

    // Check unimplemented values
    EXPECT_EQ(entry->iLifetime, 0);
    EXPECT_EQ(entry->iPriority, 0);
    EXPECT_STREQ(entry->strPlotOutline, "");
    EXPECT_STREQ(entry->strStreamURL, "");

    // TODO:
    // Directory (recgroup/title)
    // ChannelName
    // Description
    // Icon/Thumbnail/FanartPath
    // RecordingTime
    // Duration
    // GenreType, GenreSubType
    // LastPlayedPosition

    ++m_it;
  }

private:
    bool m_valid;
    mysqlpp::StoreQueryResult m_res;
    mysqlpp::Connection m_conn;
    mysqlpp::StoreQueryResult::const_iterator m_it;
};

TEST_F(MythTVRecordings, MythTVTestRecordingsAmount)
{
  // The implementation of GetRecordingsAmount or GetRecordings will
  // cache the recordings internally. Check if the same amount is
  // presented after two consecutively calls
  int recordingsAmount1 = m_myth->GetRecordingsAmount();
  RecordProperty("RecordingsAmount", recordingsAmount1);
  int recordingsAmount2 = m_myth->GetRecordingsAmount();
  RecordProperty("RecordingsAmount", recordingsAmount2);
  EXPECT_EQ(recordingsAmount1, recordingsAmount2);

  RecordingsChecker r;
  EXPECT_EQ(r.numRecordings(), recordingsAmount1);

  // Also the number of calls to TransferRecordingEntry must match the
  // number of recordings.
  EXPECT_CALL(*m_xbmc_pvr, TransferRecordingEntry(_, _))
      .Times(recordingsAmount1);
  EXPECT_CALL(*m_xbmc_pvr, TriggerRecordingUpdate())
      .Times(0);
  PVR_ERROR result = m_myth->GetRecordings(NULL);
  EXPECT_EQ(result, PVR_ERROR_NO_ERROR);

  Mock::VerifyAndClear(m_xbmc_pvr);
}

TEST_F(MythTVRecordings, MythTVTestGetRecordings)
{
  RecordingsChecker r;
  EXPECT_CALL(*m_xbmc_pvr, TransferRecordingEntry(_, _))
      .WillRepeatedly(WithArgs<1>(Invoke(&r, &RecordingsChecker::checkTransferRecordingEntry)));
  EXPECT_CALL(*m_xbmc_pvr, TriggerRecordingUpdate())
      .Times(0);
  PVR_ERROR result = m_myth->GetRecordings(NULL);
  EXPECT_EQ(result, PVR_ERROR_NO_ERROR);

  Mock::VerifyAndClear(m_xbmc_pvr);
}

TEST_F(MythTVRecordings, MythTVTestRecordingPlayCount)
{
  PVR_RECORDING recording = *GetRecordings().begin();
  int originalPlayCount = recording.iPlayCount;

  EXPECT_CALL(*m_xbmc_pvr, TriggerRecordingUpdate()).Times(3);

  PVR_ERROR result = m_myth->SetRecordingPlayCount(recording, originalPlayCount > 0 ? 0 : 1);
  EXPECT_EQ(result, PVR_ERROR_NO_ERROR);
  result = m_myth->SetRecordingPlayCount(recording, originalPlayCount);
  EXPECT_EQ(result, PVR_ERROR_NO_ERROR);

  // Test to set it again to the same value, since this was an issue in older versions
  result = m_myth->SetRecordingPlayCount(recording, originalPlayCount);
  EXPECT_EQ(result, PVR_ERROR_NO_ERROR);

  Mock::VerifyAndClear(m_xbmc_pvr);
}

// TODO: SetRecordingLastPlayedPosition leads to TriggerRecordingUpdate,
//       that need to update the list of recordings.
TEST_F(MythTVRecordings, DISABLED_MythTVTestRecordingLastPlayedPosition)
{
  PVR_RECORDING recording = *GetRecordings().begin();
  int originalPlayedPosition = recording.iLastPlayedPosition;
  sleep(5);

  int lastPlayedPosition = m_myth->GetRecordingLastPlayedPosition(recording);
  EXPECT_EQ(originalPlayedPosition, lastPlayedPosition);

  int newPlayedPosition = originalPlayedPosition > 0 ? 0 : 10;
  PVR_ERROR result = m_myth->SetRecordingLastPlayedPosition(recording, newPlayedPosition);
  sleep(5);
  lastPlayedPosition = m_myth->GetRecordingLastPlayedPosition(recording);
  EXPECT_EQ(result, PVR_ERROR_NO_ERROR);
  EXPECT_EQ(lastPlayedPosition, newPlayedPosition);

  result = m_myth->SetRecordingLastPlayedPosition(recording, originalPlayedPosition);
  sleep(5);
  lastPlayedPosition = m_myth->GetRecordingLastPlayedPosition(recording);
  EXPECT_EQ(lastPlayedPosition, originalPlayedPosition);
  EXPECT_EQ(result, PVR_ERROR_NO_ERROR);
}

TEST_F(MythTVRecordings, MythTVTestGetRecordingsEdl)
{
  PVR_RECORDING recording = *GetRecordings().begin();
  PVR_EDL_ENTRY edl[128];
  int size = sizeof(edl);

  PVR_ERROR result = m_myth->GetRecordingEdl(recording, edl, &size);
  EXPECT_EQ(result, PVR_ERROR_NO_ERROR);
}

TEST_F(MythTVRecordings, MythTVTestPlayRecordingNotFound)
{
  PVR_RECORDING recording;
  memset(&recording, 0, sizeof(PVR_RECORDING));

  // Start playback
  bool res = m_myth->OpenRecordedStream(recording);
  EXPECT_FALSE(res);

  if (res)
    m_myth->CloseRecordedStream();
}

TEST_F(MythTVRecordings, MythTVTestPlayRecording)
{
  // Get recording
  PVR_RECORDING recording = *GetRecordings().begin();

  // Start playback
  bool res = m_myth->OpenRecordedStream(recording);
  EXPECT_TRUE(res);

  // Position
  long long pos = m_myth->SeekRecordedStream(0, SEEK_SET);
  EXPECT_EQ(pos, 0);

  // Read length
  long long length = m_myth->LengthRecordedStream();
  EXPECT_GT(length, 0);

  // Read packets
  int len_read;
  unsigned char buffer[32*1024];
  for (int i = 0; i < 5; i++)
  {
    len_read = m_myth->ReadRecordedStream(buffer, sizeof(buffer));
    EXPECT_LE(len_read, (int)sizeof(buffer));

    usleep(1000); // TODO: Measure XBMC's real buffersize, and read frequency
  }

  // Close stream
  m_myth->CloseRecordedStream();
}
