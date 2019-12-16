for file in `ls`
do
 newfile=`echo $file | sed 's/LampLightMoreLightModelTest/LampLightMoreLightModelTogetherTest/g'`
 mv $file $newfile
done