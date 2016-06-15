#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<Windows.h>

char buffer[1024] = { '\0' };
int fr_pos = -1;

DWORD WINAPI ThreadFunc(void* data) {
	FILE *fr = fopen("C:\\Users\\Aravind Balla\\Documents\\Visual Studio 2013\\RnDSummerProjects\\ServerUsingFile\\ServerUsingFile\\serv.txt", "r+");
	FILE *ftemp;
	while (1)
	{
		ftemp = fopen("C:\\Users\\Aravind Balla\\Documents\\Visual Studio 2013\\RnDSummerProjects\\ServerUsingFile\\ServerUsingFile\\serv.txt", "r+");
		fseek(ftemp, 0, SEEK_END);
		//printf("%d\n", ftell(ftemp));
		if (fr_pos != ftell(ftemp))
		{
			fgets(&buffer, sizeof(buffer), fr);
			printf("server: %s\n", buffer);
			fr_pos = ftell(ftemp);
			memset(&buffer, 0, sizeof(buffer));
		}
		fclose(ftemp);
	}
	return 0;
}

main()
{
	FILE* fw;

	gets(buffer);
	fw = fopen("clnt.txt", "a+");
	fprintf(fw, "%s\n", buffer);
	memset(&buffer, 0, sizeof(buffer));
	fclose(fw);

	HANDLE thread = CreateThread(NULL, 0, ThreadFunc, NULL, 0, NULL);
	while (1)
	{
		gets(buffer);
		fw = fopen("clnt.txt", "a+");
		fprintf(fw, "%s\n", buffer);
		memset(&buffer, 0, sizeof(buffer));
		fclose(fw);
	}

	fclose(fw);
}