#!/bin/sh

for i in *.png ; do
  ../../../stm32plus/utils/bm2rgbi/bm2rgbi/bin/Debug/bm2rgbi.exe $i ${i%.png}.262 ili9325 262
done
