if [ ${1,,} = "sarim" ]; then
	echo "Welcome!"
elif [ ${1,,} = "help" ]; then
	echo "Enter your username!"
else
	echo "Permission denied!"
fi

