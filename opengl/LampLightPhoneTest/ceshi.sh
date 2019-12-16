for file in `ls`
do
 newfile=`echo $file | sed 's/LampLightTest/LampLightPhoneTest/g'`
 mv $file $newfile
done