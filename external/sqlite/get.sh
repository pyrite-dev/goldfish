#!/bin/sh
YEAR="2025"
VERSION="3500000"
rm -rf sqlite.zip sqlite-*
curl -L https://www.sqlite.org/$YEAR/sqlite-amalgamation-$VERSION.zip > sqlite.zip
unzip sqlite.zip
cp sqlite-*/sqlite3.* ./
rm -rf sqlite.zip sqlite-*
