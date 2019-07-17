#pragma comment(lib, "shlwapi.lib")
#include <Windows.h>
#include <Shlwapi.h>
#include <iostream>
#include <fstream>

int main(int argc, char* argv[])
{
	if (!argv[1])
	{
		printf("You must drag and drop your binary onto this .exe \n");
		printf("Press any key to exit . . .");
		getchar();
		return 0;
	}

	//create input file stream
	std::ifstream binary(argv[1], std::ios::binary | std::ios::ate);

	//tellg = end of file becuz ios::ate
	auto size = binary.tellg();

	//create aob to store file bytes
	unsigned char* aob = new unsigned char[size];

	//seek to begin of file
	binary.seekg(0, std::ios::beg);

	//read file into aob and close
	binary.read((char*)aob, size);
	binary.close();

	//Get path and create a .h with same filename
	char path[MAX_PATH];
	strcpy_s(path, MAX_PATH, argv[1]);
	PathRenameExtension(path, ".h");
	CloseHandle(CreateFile(path, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0));

	//open header file stream
	std::ofstream header(path, std::ios::out | std::ios::binary);

	header << "//https://guidedhacking.com\n\n";
	header << "unsigned char binary[] = { ";

	//Write contents of aob to header file
	for (unsigned int i = 0; i < size; i++)
	{
		header << "0x";
		if ((unsigned int)aob[i] == 0)
		{
			header << "00";
		}
		else
		{
			header << std::hex << (unsigned int)aob[i];
		}
		header << ", ";
	}
	header << "};";

	header.close();
	delete[] aob;
	printf("AOB Header File Created!\n");
	printf("Press Any Key To Exit");
	getchar();
	return 0;
}