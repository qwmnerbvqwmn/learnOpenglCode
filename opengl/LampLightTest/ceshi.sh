for file in `ls`
do
 newfile=`echo $file | sed 's/coordSystemReal/LampLightTest/g'`
 mv $file $newfile
done