#!/bin/sh

mkdir -p "config/m4"
if [ -z "$AUTOMAKE" ]; then
    AUTOMAKE="automake";
fi;
AUTOMAKE="$AUTOMAKE --foreign" autoreconf --force --install -I config

