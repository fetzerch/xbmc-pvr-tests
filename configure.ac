AC_INIT([xbmc-pvr-tests], 1:0:0)

AC_CANONICAL_HOST
AC_CANONICAL_TARGET

AM_INIT_AUTOMAKE(AC_PACKAGE_NAME, AC_PACKAGE_VERSION, [http://www.xbmc.org])

m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([no])])

PREFIX_DEFAULT="/usr/local"
AC_PREFIX_DEFAULT($PREFIX_DEFAULT)

AC_PROG_CXX
AC_PROG_CPP
AC_PROG_LIBTOOL

AC_SEARCH_LIBS([pthread_create],[pthread],, AC_MSG_ERROR([required library 'pthread' is missing]))

BUILD_TYPE="debug"
if test "$use_release" = "yes"; then
  BUILD_TYPE="release"
fi

AC_CHECK_PROG(HAVE_PKG_CONFIG, pkg-config, yes)
if test "x$HAVE_PKG_CONFIG" != "xyes" ; then
  AC_MSG_WARN([pkg-config was not found on your system. external ffmpeg will be disabled])
  use_external_ffmpeg="no"
fi

OS="unknown"
ARCHITECTURE="unknown"
HOST_INCLUDES=""
HOST_CXXFLAGS=""
ARCH_DEFINES="-DTARGET_POSIX -DTARGET_LINUX -D_LINUX"
DYN_LIB_EXT="so"
TARGET_LDFLAGS=""
BIN_EXT=".pvr"
BINPREFIX=""
case "${host}" in
  arm-*-linux-androideabi)
    TARGET_LDFLAGS="-avoid-version -no-undefined"
    OS="android"
    ARCHITECTURE="arm"
    BIN_EXT=".so"
    BINPREFIX="lib"
    ;;
  arm*-*-linux*)
    OS="linux"
    ARCHITECTURE="arm"
    ;;
  powerpc-*-linux*)
    OS="linux"
    ARCHITECTURE="powerpc"
    ;;
  powerpc64-*-linux*)
    OS="linux"
    ARCHITECTURE="powerpc64"
    ;;
  mipsel-*-linux*)
    OS="linux"
    ARCHITECTURE="mipsel"
    ;;
  x86_64-*-linux*)
    OS="linux"
    ARCHITECTURE="x86_64"
    ;;
  *-*-linux*)
    OS="linux"
    ARCHITECTURE="i486"
    ;;
  arm-apple-darwin*)
    OS="ios"
    HOST_CXXFLAGS="-dynamiclib -single_module -undefined dynamic_lookup"
    host_os_osx="yes"
    ARCH_DEFINES="-DTARGET_POSIX -DTARGET_DARWIN -D_LINUX"
    DYN_LIB_EXT="dylib"
    ;;
  x86_64-apple-darwin*)
    OS="osx64"
    HOST_CXXFLAGS="-dynamiclib -single_module -undefined dynamic_lookup"
    host_os_osx="yes"
    ARCH_DEFINES="-DTARGET_POSIX -DTARGET_DARWIN -D_LINUX"
    DYN_LIB_EXT="dylib"
    ;;
  *-apple-darwin*)
    OS="osx32"
    HOST_CXXFLAGS="-dynamiclib -single_module -undefined dynamic_lookup"
    host_os_osx="yes"
    ARCH_DEFINES="-DTARGET_POSIX -DTARGET_DARWIN -D_LINUX"
    DYN_LIB_EXT="dylib"
    ;;
  *-freebsd*)
    OS="freebsd"
    ARCH_DEFINES="-DTARGET_POSIX -DTARGET_FREEBSD -D_LINUX"
    ;;
esac

# Check for mysql
AC_PATH_PROG(MYSQL_CONFIG, mysql_config, "no")
if test "x$MYSQL_CONFIG" != "xno"; then
  AC_DEFINE([HAVE_MYSQL],[1],["Define to 1 if you have the `mysql' library (-lmysqlclient)."])
  MYSQL_INCLUDES=`$MYSQL_CONFIG --include`
  MYSQL_LIBS=`$MYSQL_CONFIG --libs`
  AC_SUBST(MYSQL_INCLUDES)
  AC_SUBST(MYSQL_LIBS)
else
  AC_MSG_ERROR([mysql_config not found])
fi

# Check for mysql++
#AC_CHECK_HEADER([mysql++/mysql++.h], use_mysqlpp=yes, use_mysqlpp=no,)
#if test "$use_mysqlpp" = "yes"; then
#  AC_CHECK_LIB([mysqlpp], use_mysqlpp=yes, use_mysqlpp=no,)
#  if test "$use_mysqlpp" = "yes"; then
#    MYSQLPP_LIBS=-l libmysqlpp
#    AC_SUBST(MYSQLPP_LIBS)
#  else
#    AC_MSG_ERROR([mysqlpp includes not found])
#  fi
#else
#  AC_MSG_ERROR([mysqlpp lib not found])
#fi

# Checks for boost headers
AC_LANG_PUSH([C++])
AC_CHECK_HEADER([boost/shared_ptr.hpp], use_boost=yes, use_boost=no,)
AC_LANG_POP([C++])
if test "$use_boost" = "yes"; then
  AC_DEFINE([HAVE_BOOST],[1],["Define to 1 if we have the `boost' headers"])
else
  AC_MSG_ERROR([boost headers not found])
fi

# Check for intrinsics
AC_MSG_CHECKING([for __sync_add_and_fetch(temp, 1)])
AC_TRY_COMPILE([],[long* temp=0; __sync_add_and_fetch(temp, 1);],
                [have_builtin_sync_add_and_fetch=yes],
                [have_builtin_sync_add_and_fetch=no])
AC_MSG_RESULT($have_builtin_sync_add_and_fetch)
if test "x$have_builtin_sync_add_and_fetch" = "xyes"; then
    AC_DEFINE(HAS_BUILTIN_SYNC_ADD_AND_FETCH, 1,
        [Define to 1 if your compiler supports the __sync_add_and_fetch() intrinsic.])
fi

AC_MSG_CHECKING([for __sync_sub_and_fetch(temp, 1)])
AC_TRY_COMPILE([],[long* temp=0; __sync_sub_and_fetch(temp, 1);],
                [have_builtin_sync_sub_and_fetch=yes],
                [have_builtin_sync_sub_and_fetch=no])
AC_MSG_RESULT($have_builtin_sync_sub_and_fetch)
if test "x$have_builtin_sync_sub_and_fetch" = "xyes"; then
    AC_DEFINE(HAS_BUILTIN_SYNC_SUB_AND_FETCH, 1,
        [Define to 1 if your compiler supports the __sync_sub_and_fetch() intrinsic.])
fi

AC_MSG_CHECKING([for __sync_val_compare_and_swap(temp, 1, 1)])
AC_TRY_COMPILE([],[long *temp = 0; __sync_val_compare_and_swap(temp, 1, 1);],
                [have_builtin_sync_val_compare_and_swap=yes],
                [have_builtin_sync_val_compare_and_swap=no])
AC_MSG_RESULT($have_builtin_sync_val_compare_and_swap)
if test "x$have_builtin_sync_val_compare_and_swap" = "xyes"; then
    AC_DEFINE(HAS_BUILTIN_SYNC_VAL_COMPARE_AND_SWAP, 1,
        [Define to 1 if your compiler supports the __sync_val_compare_and_swap() intrinsic.])
fi

HOST_CXXFLAGS="-Wall -Wextra -Wno-missing-field-initializers -Woverloaded-virtual -Wno-parentheses -fPIC $HOST_CXXFLAGS"

AC_SUBST(ARCHITECTURE)
AC_SUBST(BUILD_TYPE)
AC_SUBST(HOST_CXXFLAGS)
AC_SUBST(HOST_INCLUDES)
AC_SUBST(OS)
AC_SUBST(ARCH_DEFINES)
AC_SUBST(DYN_LIB_EXT)
AC_SUBST(TARGET_LDFLAGS)
AC_SUBST(BINPREFIX)
AC_SUBST(BIN_EXT)

if test "x$host_os_osx" = "xyes"; then
  AM_CONDITIONAL(HOST_IS_OSX, true)
else
  AM_CONDITIONAL(HOST_IS_OSX, false)
fi

AC_CONFIG_FILES([Makefile \
                 lib/Makefile \
                 addons/Makefile \
                 addons/pvr.mythtv.cmyth/Makefile])

# pushd and popd are not available in other shells besides bash, so implement
# our own pushd/popd functions
XB_DIRSTACK="$PWD"
xb_pushd()
{
  local dirname="$1"
  if [[ -d "$dirname" ]] && [[ -x "$dirname" ]]; then
    cd "$dirname"
    XB_DIRSTACK="$dirname ${XB_DIRSTACK:-$PWD}"
    return 0
  else
    AC_MSG_ERROR(xb_pushd: unable to change to $dirname)
  fi
}
xb_popd()
{
  if [[ -n "$XB_DIRSTACK" ]]; then
    XB_DIRSTACK="${XB_DIRSTACK#* }"
    cd "${XB_DIRSTACK%% *}"
    return 0
  else
    AC_MSG_ERROR(xb_popd: unable to go back to previous directory)
  fi
}

# Function to run the configure scripts in our submodules
# Consists of three paramaters, the path to the submodule, the configure command
# with appropriate arguments, and a third parameter set to 1 if we are to skip
# running the script, anything else if not.
AC_DEFUN([XB_CONFIG_MODULE],[
AC_CONFIG_COMMANDS_POST([
if [[ $3 != "1" ]]; then
    if [[ -d $1 ]]; then
      xb_pushd $1
      $2
      if [[ $? -ne 0 ]]; then
        xb_popd
        AC_MSG_ERROR([[Submodule $1 failed to configure]])
      else
        xb_popd
      fi
    else
      AC_MSG_ERROR([[Submodule $1 does not exist]])
    fi
else
    AC_MSG_NOTICE([[Skipping configuration of submodule $1.]])
fi
])
])

XB_CONFIG_MODULE([lib/gtest], [
  ./configure \
    CC="$CC" \
    CXX="$CXX" \
    CFLAGS="$CFLAGS" \
    --prefix="${prefix}" --includedir="${includedir}" --libdir="${libdir}" --datadir="${datadir}" \
    --host=$host_alias \
    --build=$build_alias \
    --target=$target_alias \
    --disable-shared \
    --enable-static \
    --with-pthreads
], [0])

XB_CONFIG_MODULE([lib/gmock], [
  ./configure \
    CC="$CC" \
    CXX="$CXX" \
    CFLAGS="$CFLAGS" \
    --prefix="${prefix}" --includedir="${includedir}" --libdir="${libdir}" --datadir="${datadir}" \
    --host=$host_alias \
    --build=$build_alias \
    --target=$target_alias \
    --disable-shared \
    --enable-static \
    --with-pthreads
], [0])

XB_CONFIG_MODULE([pvr-addons], [
  ./configure \
    --prefix="${prefix}" \
    --host=$host_alias \
    --build=$build_alias \
    --target=$target_alias \
    --enable-addons-with-dependencies \
    CC="$CC" \
    CXX="$CXX" \
    CFLAGS="$CFLAGS" \
    CXXFLAGS="$CXXFLAGS"
], [0])

AC_OUTPUT

