#!/bin/sh

rm -rf autom4te.cache
rm -f aclocal.m4

rm -rf build
mkdir build
cd build

autoreconf -vif ..

if [ -z "$NOCONFIGURE" ]; then
  exec ../configure -C "$@"
fi
