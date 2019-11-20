make clean;
rm "image.txt";
clear;
make;
echo "############################################################";
./server -p 3000 -l -i 127.0.0.1;