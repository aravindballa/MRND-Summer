#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<Windows.h>

char buffer[1024] = { '\0' };
int fr_pos = -1;

DWORD WINAPI ThreadFunc(void* data) {
	FILE *fr = fopen("clnt.txt", "r+");

	while (1)
	{
		FILE *ftemp = fopen("clnt.txt", "r+");
		fseek(ftemp, 0, SEEK_END);
		//printf("%d\n", ftell(ftemp));
		if (fr_pos != ftell(ftemp))
		{
			fgets(&buffer, sizeof(buffer), fr);
			printf("client: %s\n", buffer);
			fr_pos = ftell(ftemp);
			memset(&buffer, 0, sizeof(buffer));
		}
		fclose(ftemp);
	}

	fclose(fr);
	return 0;
}

main()
{
	FILE* fw;

	memset(&buffer, 0, sizeof(buffer));
	gets(buffer);
	fw = fopen("serv.txt", "a+");
	fprintf(fw, "%s\n", buffer);
	memset(&buffer, 0, sizeof(buffer));
	fclose(fw);

	HANDLE thread = CreateThread(NULL, 0, ThreadFunc, NULL, 0, NULL);

	while (1)
	{
		memset(&buffer, 0, sizeof(buffer));
		gets(buffer);
		fw = fopen("serv.txt", "a+");
		fprintf(fw, "%s\n", buffer);
		memset(&buffer, 0, sizeof(buffer));
		fclose(fw);
	}

	fclose(fw);
	
}