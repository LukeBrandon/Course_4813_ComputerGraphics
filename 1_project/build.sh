c++ -Wall src/read.cpp -o src/read
c++ -Wall src/graphs.cpp -lGL -lGLU -lglut -o src/graphs

./src/read
./src/graphs

rm -f src/graphs src/read