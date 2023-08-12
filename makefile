CMPR = g++
PROJ = md5

$(PROJ):
	$(CMPR) src/main.cpp src/md5.cpp src/md5_consts.cpp -o bin/$(PROJ)