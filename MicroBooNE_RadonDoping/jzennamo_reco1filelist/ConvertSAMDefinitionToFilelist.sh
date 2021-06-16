#! /bin/bash   


list1=`samweb list-definition-files zennamo_bipo_overlay_run3_goodruns_BetterDL_2300perEvent`
i=1
for file in $list1
do
    fullpath=`samweb locate-file $file`
    cutpath=`echo "${fullpath%(*}"/$file`
    echo "${cutpath##*:}"
    let i=i+1
done
