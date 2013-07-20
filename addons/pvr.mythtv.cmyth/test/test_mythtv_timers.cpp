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

class MythTVTimers : public MythTVFixture
{
};

TEST_F(MythTVTimers, MythTVGetTimers)
{
  int timersAmount = m_myth->GetTimersAmount();

  // Also the number of calls to TransferTimerEntry must match the
  // number of timers.
  EXPECT_CALL(*m_xbmc_pvr, TransferTimerEntry(_, _))
      .Times(timersAmount);
  PVR_ERROR result = m_myth->GetTimers(NULL);
  EXPECT_EQ(result, PVR_ERROR_NO_ERROR);
}
