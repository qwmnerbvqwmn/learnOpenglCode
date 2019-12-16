for file in `ls`
do
 newfile=`echo $file | sed 's/LampLightPhoneTest/LampLightMaterilMapTest/g'`
 mv $file $newfile
done