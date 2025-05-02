#!/bin/sh
rm -rf obj lib bin *.make Makefile

cd src
rm -rf obj lib bin *.make Makefile
cd ..

cd util
rm -rf obj lib bin *.make Makefile
cd ..

cd ..
