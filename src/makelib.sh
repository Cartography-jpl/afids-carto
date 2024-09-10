#!/bin/sh

ar rv libcarto.a *.o
ar dv libcarto.a carto.o
cp libcarto.a ../x86-linux/lib/
