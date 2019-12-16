for file in `ls`
do
 newfile=`echo $file | sed 's/LampLightMaterilTest/LampLightMaterilMapTest/g'`
 mv $file $newfile
done