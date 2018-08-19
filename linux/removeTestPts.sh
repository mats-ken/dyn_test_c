for F
do
	sed -i -e 's/ TEST_PT([^(]\+);//g' $F
done
