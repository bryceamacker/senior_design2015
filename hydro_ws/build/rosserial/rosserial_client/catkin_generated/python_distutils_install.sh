#!/bin/sh -x

if [ -n "$DESTDIR" ] ; then
    case $DESTDIR in
        /*) # ok
            ;;
        *)
            /bin/echo "DESTDIR argument must be absolute... "
            /bin/echo "otherwise python's distutils will bork things."
            exit 1
    esac
    DESTDIR_ARG="--root=$DESTDIR"
fi

cd "/home/dexter/senior_design2015/hydro_ws/src/rosserial/rosserial_client"

# Note that PYTHONPATH is pulled from the environment to support installing
# into one location when some dependencies were installed in another
# location, #123.
/usr/bin/env \
    PYTHONPATH="/home/dexter/senior_design2015/hydro_ws/install/lib/python2.7/dist-packages:/home/dexter/senior_design2015/hydro_ws/build/lib/python2.7/dist-packages:$PYTHONPATH" \
    CATKIN_BINARY_DIR="/home/dexter/senior_design2015/hydro_ws/build" \
    "/usr/bin/python" \
    "/home/dexter/senior_design2015/hydro_ws/src/rosserial/rosserial_client/setup.py" \
    build --build-base "/home/dexter/senior_design2015/hydro_ws/build/rosserial/rosserial_client" \
    install \
    $DESTDIR_ARG \
    --install-layout=deb --prefix="/home/dexter/senior_design2015/hydro_ws/install" --install-scripts="/home/dexter/senior_design2015/hydro_ws/install/bin"
