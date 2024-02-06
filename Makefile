Md5:
	g++ -c Src/Md5.cpp
	ar rvs Md5.a Md5.o
	del Md5.o
	move Md5.a Bin/
	copy Src\Md5.hpp Bin\

TestMd5:
	g++ -o Md5 Test/main.cpp Bin/Md5.a
	move Md5.exe Test/Bin/
	Test/Bin/Md5.exe test1
	Test/Bin/Md5.exe test2
	Test/Bin/Md5.exe test3
	Test/Bin/Md5.exe test4
	Test/Bin/Md5.exe test5
	Test/Bin/Md5.exe test6