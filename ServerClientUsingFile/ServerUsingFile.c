#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<Windows.h>


main()
{
	char buffer[1024] = { '\0' };
	FILE* fw;
	FILE *fr = fopen("C:\\Users\\Aravind Balla\\Documents\\Visual Studio 2013\\RnDSummerProjects\\ClientUsingFile\\ClientUsingFile\\clnt.txt", "r+");
	int fr_pos = -1;
	FILE *ftemp;

	
	while (1)
	{
		ftemp = fopen("C:\\Users\\Aravind Balla\\Documents\\Visual Studio 2013\\RnDSummerProjects\\ClientUsingFile\\ClientUsingFile\\clnt.txt", "r+");
		fseek(ftemp, 0, SEEK_END);
		//printf("%d\n", ftell(ftemp));
		if (fr_pos != ftell(ftemp))
		{
			fgets(&buffer, sizeof(buffer), fr);
			printf("client: %s\n", buffer);
			fr_pos = ftell(ftemp);
		}
		fclose(ftemp);

		memset(&buffer, 0, sizeof(buffer));
		gets(buffer);
		fw = fopen("serv.txt", "a+");
		fprintf(fw, "%s\n", buffer);
		memset(&buffer, 0, sizeof(buffer));
		fclose(fw);

		Sleep(4000);
	}

	fclose(fw);
	fclose(fr);
}