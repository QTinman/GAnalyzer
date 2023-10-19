#!/bin/sh
LD_LIBRARY_PATH=/home/john/Qt/5.13.1/Src/qtbase/lib${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}
export LD_LIBRARY_PATH
QT_PLUGIN_PATH=/home/john/Qt/5.13.1/Src/qtbase/plugins${QT_PLUGIN_PATH:+:$QT_PLUGIN_PATH}
export QT_PLUGIN_PATH
exec /home/john/Qt/5.13.1/Src/qtbase/bin/uic "$@"
