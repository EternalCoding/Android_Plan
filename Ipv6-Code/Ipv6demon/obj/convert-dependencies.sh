#!/bin/sh
# AUTO-GENERATED FILE, DO NOT EDIT!
if [ -f $1.org ]; then
  sed -e 's!^D:/Java/cygwin/lib!/usr/lib!ig;s! D:/Java/cygwin/lib! /usr/lib!ig;s!^D:/Java/cygwin/bin!/usr/bin!ig;s! D:/Java/cygwin/bin! /usr/bin!ig;s!^D:/Java/cygwin/!/!ig;s! D:/Java/cygwin/! /!ig;s!^Z:!/cygdrive/z!ig;s! Z:! /cygdrive/z!ig;s!^X:!/cygdrive/x!ig;s! X:! /cygdrive/x!ig;s!^W:!/cygdrive/w!ig;s! W:! /cygdrive/w!ig;s!^E:!/cygdrive/e!ig;s! E:! /cygdrive/e!ig;s!^D:!/cygdrive/d!ig;s! D:! /cygdrive/d!ig;s!^C:!/cygdrive/c!ig;s! C:! /cygdrive/c!ig;' $1.org > $1 && rm -f $1.org
fi
