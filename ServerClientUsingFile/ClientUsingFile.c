#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<Windows.h>

main()
{
	char buffer[1024] = { '\0' };
	FILE* fw;
	FILE *fr = fopen("C:\\Users\\Aravind Balla\\Documents\\Visual Studio 2013\\RnDSummerProjects\\ServerUsingFile\\ServerUsingFile\\serv.txt", "r+");
	FILE *ftemp;
	
	int fr_pos = -1;
	//FILE *fr = fopen();
	while (1)
	{
		gets(buffer);
		fw = fopen("clnt.txt", "a+");
		fprintf(fw, "%s\n", buffer);
		memset(&buffer, 0, sizeof(buffer));
		fclose(fw);

		Sleep(4000);

		ftemp = fopen("C:\\Users\\Aravind Balla\\Documents\\Visual Studio 2013\\RnDSummerProjects\\ServerUsingFile\\ServerUsingFile\\serv.txt", "r+");
		fseek(ftemp, 0, SEEK_END);
		//printf("%d\n", ftell(ftemp));
		if (fr_pos != ftell(ftemp))
		{
			fgets(&buffer, sizeof(buffer), fr);
			printf("server: %s\n", buffer);
			fr_pos = ftell(ftemp);
		}
		fclose(ftemp);
	}

	fclose(fw);
}