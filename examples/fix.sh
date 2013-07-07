#!/bin/sh

for i in `find . -name .project`; do
		j=`dirname $i`
		cd $j
		k=`basename $j`
		echo $k
		sed s/mc2pa8201/$k/g < .project > .project.tmp
		mv .project.tmp .project
	cd ..
done
