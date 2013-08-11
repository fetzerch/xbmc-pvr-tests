#!/bin/sh

find . -name "*.gcno" -exec rm -rf {} \;
find . -name "*.gcna" -exec rm -rf {} \;

# Make
./configure CPPFLAGS="--coverage" LIBS="--coverage"
make clean && make

echo "lcov zero"
lcov --directory . --zerocounters

echo "executing test"
addons/pvr.mythtv.cmyth/test_mythtv -v --mythtvhost $1

echo "lcov initial"
lcov --capture --initial \
     --directory addons/pvr.mythtv.cmyth/ \
     --directory pvr-addons/addons/pvr.mythtv.cmyth/ \
     --directory pvr-addons/lib/cmyth/ \
     --output-file all_base.info --no-external --rc lcov_branch_coverage=0

echo "lcov run"
lcov --capture --directory . --rc lcov_branch_coverage=1 \
     --output-file all.info --no-external

echo "lcov combining"
lcov --add-tracefile all_base.info \
     --add-tracefile all.info \
     --output-file all_total.info

# lcov --initial does not evaluate --no-external
echo "lcov stripping unneeded info"
lcov --remove all_total.info \
     "/usr/include/*" \
     "/mock/*" \
     "/lib/gmock/*" \
     "/lib/gtest/*" \
     "/pvr-addons/lib/platform/*" \
     "/pvr-addons/xbmc/*" \
     --output-file pvr.mythtv.cmyth.info

echo "genhtml generating report"
rm -rf coverage/
genhtml -o coverage pvr.mythtv.cmyth.info

firefox coverage/index.html

