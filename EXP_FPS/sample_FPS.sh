#!/bin/sh
while [ 1 ]; do
  start=`adb shell service call SurfaceFlinger 1013 | grep -o -E \([a-fA-F0-9]+\ \)`
  sleep 1
  end=`adb shell service call SurfaceFlinger 1013 | grep -o -E \([a-fA-F0-9]+\ \)`
  python -c "print (int(0x$end) - int(0x$start)) / 1.0, 'fps'"
done
~

