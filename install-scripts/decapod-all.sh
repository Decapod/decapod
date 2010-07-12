
if [ "$1" = "remove" ]; then
    OPERATION=$1
elif [ "$1" = "clean" ]; then
    rm -rf build
    OPERATION=$2 
else
    OPERATION="install"
fi

./decapod-apt.sh $OPERATION
./decapod-gphoto.sh $OPERATION
./decapod-ocropus.sh $OPERATION
./decapod-decapod.sh $OPERATION
