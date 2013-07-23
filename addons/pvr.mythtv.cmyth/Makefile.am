mythtv_addon = $(abs_top_srcdir)/pvr-addons/addons/pvr.mythtv.cmyth/src

INCLUDES = @HOST_INCLUDES@

AM_CPPFLAGS = -Wall -Wextra -g -O0 \
              -I$(abs_top_srcdir)/lib/gtest/include \
              -I$(abs_top_srcdir)/lib/gmock/include \
              -I$(abs_top_srcdir)/mock \
              -I$(abs_top_srcdir)/pvr-addons/lib \
              -I$(abs_top_srcdir)/pvr-addons/lib/cmyth/include \
              -I$(abs_top_srcdir)/pvr-addons/xbmc \
              -I$(mythtv_addon) \
              $(MYSQL_INCLUDES)

# Build addon sources into small library linked into the test cases
noinst_LTLIBRARIES = libmythtvcmyth-addon.la
libmythtvcmyth_addon_la_SOURCES = $(mythtv_addon)/client.cpp \
                               $(mythtv_addon)/fileOps.cpp \
                               $(mythtv_addon)/categories.cpp \
                               $(mythtv_addon)/pvrclient-mythtv.cpp \
                               $(mythtv_addon)/cppmyth/MythChannel.cpp \
                               $(mythtv_addon)/cppmyth/MythConnection.cpp \
                               $(mythtv_addon)/cppmyth/MythDatabase.cpp \
                               $(mythtv_addon)/cppmyth/MythEventHandler.cpp \
                               $(mythtv_addon)/cppmyth/MythFile.cpp \
                               $(mythtv_addon)/cppmyth/MythProgramInfo.cpp \
                               $(mythtv_addon)/cppmyth/MythRecorder.cpp \
                               $(mythtv_addon)/cppmyth/MythStorageGroupFile.cpp \
                               $(mythtv_addon)/cppmyth/MythSignal.cpp \
                               $(mythtv_addon)/cppmyth/MythRecordingRule.cpp \
                               $(mythtv_addon)/cppmyth/MythTimestamp.cpp \
                               $(mythtv_addon)/cppmyth/MythEPGInfo.cpp

# Build test
noinst_PROGRAMS = test_mythtv
test_mythtv_LDFLAGS = -L$(abs_top_srcdir)/lib/gtest/lib -lgtest \
                      -L$(abs_top_srcdir)/lib/gmock/lib -lgmock \
                      -lmysqlpp

test_mythtv_LDADD = libmythtvcmyth-addon.la \
                    @abs_top_srcdir@/pvr-addons/lib/cmyth/libcmyth.la \
                    $(MYSQL_LIBS)

test_mythtv_SOURCES = test/main.cpp \
                      test/mythtvfixture.cpp \
                      test/test_mythtv_connection.cpp \
                      test/test_mythtv_recordings.cpp \
                      test/test_mythtv_channels.cpp \
                      test/test_mythtv_timers.cpp \
                      test/test_mythtv_livetv.cpp
