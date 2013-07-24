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

#ifndef MYTHTVFIXTURE_H
#define MYTHTVFIXTURE_H

#include "client.h"

#include <gtest/gtest.h>

#include <list>

class PVRClientMythTV;
namespace ADDON {
    class CHelper_libXBMC_addon;
}
class CHelper_libXBMC_pvr;

class MythTVFixture : public ::testing::Test
{
protected:
  static void SetUpTestCase();
  static void TearDownTestCase();

  static void disableLoggingAndNotifications();
  static void enableLoggingAndNotifications();
  static void disableFileCaching();

  MythTVFixture();
  virtual ~MythTVFixture();

  virtual void SetUp();
  virtual void TearDown();

  std::list<PVR_CHANNEL> GetChannels();
  std::list<PVR_CHANNEL_GROUP> GetChannelGroups();
  std::list<PVR_RECORDING> GetRecordings();
  std::list<PVR_TIMER> GetTimers();

  static PVRClientMythTV              *m_myth;
  static ADDON::CHelper_libXBMC_addon *m_xbmc_addon;
  static CHelper_libXBMC_pvr          *m_xbmc_pvr;
};

#endif // MYTHTVFIXTURE_H
