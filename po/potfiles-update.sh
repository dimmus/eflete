#!/bin/sh -e
# make sure you run it from toplevel source dir
if test -d ./po && test -f ./po/potfiles-update.sh; then
  find src/bin src/lib -name '*.[ch]' | sort > ./po/POTFILES.in
  echo "[PO] POTFILES.in file updated succesfully"
else
  echo "[PO] Please run this from the toplevel source directory."
  exit 77
fi
