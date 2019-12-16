for file in `ls`
do
 newfile=`echo $file | sed 's/LampLightMaterilMapTest/LampLightMoreLightModelTest/g'`
 mv $file $newfile
done