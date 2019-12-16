
# find ./Light | xargs perl -pi -e 's|\bCamera\b|Light|g'






for file in `ls`
do
 newfile=`echo $file | sed 's/Camera/Light/g'`
 mv $file $newfile
done