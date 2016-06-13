#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>

typedef struct Student{
	int rollno;
	char name[24];
	char college[20];
} student; //48

typedef struct TeachingStaff{
	int staffid;
	char name[24];
	char dept[12];
	char college[24];
} teachingstaff; //64

typedef struct TLeafPage{
	int pagetype;
	int tableId;
	teachingstaff data[7];
	char unused[52];
	int index;
} tleafpage; //512

typedef struct LeafPage{
	int pagetype;
	int tableId;
	student data[10];
	char unused[20];
	int index;
} leafpage; //512

typedef struct NonLeafPage{
	int pagetype;
	int keys[63];
	int offsets[64];
} nonleafpage; //512

typedef struct TableSector{
	int tableIds[8];
	int rootPageIds[8];
} tablesector; //64

void PrintStudent(student stud)
{
	printf("%d %s %s\n", stud.rollno, stud.name, stud.college);
}

void PrintTeacher(teachingstaff stud)
{
	printf("%d %s %s %s\n", stud.staffid, stud.name, stud.college, stud.dept);
}

void BuildBtree(char* csv, char* bin)
{
	FILE* fp = fopen(csv, "rb+");
	FILE* fw = fopen(bin, "wb+");
	int i, j, lpo = 0;

	nonleafpage nlpage;
	memset(&nlpage, 0, sizeof(nonleafpage));
	fwrite(&nlpage, sizeof(nonleafpage), 1, fw);

	nlpage.pagetype = 2;
	j = 0;
	while (!feof(fp))
	{
		leafpage lpage;
		memset(&lpage, 0, sizeof(leafpage));

		lpage.pagetype = 1;

		for (i = 0; i < 10; i++)
		{
			fscanf(fp, "%d,%[^,],%[^\n]\n", &lpage.data[i].rollno, &lpage.data[i].name, &lpage.data[i].college);
			//PrintStudent(lpage.data[i]);
		}

		nlpage.offsets[lpo] = ftell(fw);
		printf("%d ", ftell(fw));
		fwrite(&lpage, sizeof(leafpage), 1, fw);
		lpo++;

		if (j > 0){
			nlpage.keys[j-1] = lpage.data[0].rollno;
			printf("%d ", lpage.data[0].rollno);
		}
		printf("\n");

		j++;
	}

	fseek(fw, 0, SEEK_SET);
	fwrite(&nlpage, sizeof(nonleafpage), 1, fw);

	fclose(fp);
	fclose(fw);

}

void BuildBtree2(char* csv, char* bin)
{
	FILE* fp = fopen(csv, "rb+");
	FILE* fw = fopen(bin, "wb+");
	FILE* ft;
	int i, j, lpo = 0;
	int recordCount = 0;

	fseek(fw, sizeof(tablesector), SEEK_SET); //first iteration: leaving 64 bytes

	tablesector tsec;
	memset(&tsec, 0, sizeof(tsec));

	tsec.rootPageIds[0] = 0; //offset of current non leaf page

	nonleafpage nlpage;
	memset(&nlpage, 0, sizeof(nonleafpage));
	nlpage.pagetype = 2;

	while (!feof(fp))
	{

		j = 0;
		while (j<=recordCount)
		{
			lpo = 0;
			leafpage lpage;
			memset(&lpage, 0, sizeof(leafpage));

			lpage.pagetype = 1;

			for (i = 0; i < 10; i++)
			{
				fscanf(fp, "%d,%[^,],%[^\n]\n", &lpage.data[i].rollno, &lpage.data[i].name, &lpage.data[i].college);
				PrintStudent(lpage.data[i]);
				lpage.index = i;
				recordCount++;
				if (feof(fp))
					break;
			}

			nlpage.offsets[lpo] = ftell(fw);
			printf("%d ", ftell(fw));
			fwrite(&lpage, sizeof(leafpage), 1, fw);
			lpo++;

			if (j > 0){
				nlpage.keys[j - 1] = lpage.data[0].rollno;
				printf("%d ", lpage.data[0].rollno);
			}
			printf("\n");

			tsec.rootPageIds[0] = ftell(fw);
			fwrite(&nlpage, sizeof(nonleafpage), 1, fw);

			//update the table sector
			ft = fopen(bin, "rb+");
			fwrite(&tsec, sizeof(tablesector), 1, ft);
			fclose(ft);

			j++;

			if (feof(fp)) break;
		}

	}

	fclose(fp);
	fclose(fw);
}

void BuildBTree3(char* csv, char* tcsv, char* bin)
{
	FILE* fp = fopen(csv, "rb+");
	FILE* fpt = fopen(tcsv, "rb+");
	FILE* fw = fopen(bin, "wb+");
	FILE* ft;
	int i, j, lpo = 0;
	int recordCount = 0, trecordCount = 0;

	fseek(fw, sizeof(tablesector), SEEK_SET); //first iteration: leaving 64 bytes

	tablesector tsec;
	memset(&tsec, 0, sizeof(tsec));

	tsec.rootPageIds[0] = 0; //offset of current non leaf page
	tsec.tableIds[0] = 1; //students
	tsec.tableIds[1] = 2; //teachers

	nonleafpage nlpage, tnlpage;
	memset(&nlpage, 0, sizeof(nonleafpage));
	nlpage.pagetype = 2;

	memset(&tnlpage, 0, sizeof(nonleafpage));
	tnlpage.pagetype = 2;

	while (!feof(fp) && !feof(fpt))
	{
		j = 0;
		while (j <= recordCount && j <= trecordCount)
		{
			lpo = 0;
			leafpage lpage;
			memset(&lpage, 0, sizeof(leafpage));
			lpage.tableId = 1;
			lpage.pagetype = 1;

			tleafpage tlpage;
			memset(&tlpage, 0, sizeof(tleafpage));
			tlpage.tableId = 2;
			tlpage.pagetype = 1;

			for (i = 0; i < 10; i++)
			{
				fscanf(fp, "%d,%[^,],%[^\n]\n", &lpage.data[i].rollno, &lpage.data[i].name, &lpage.data[i].college);
				PrintStudent(lpage.data[i]);
				lpage.index = i;
				recordCount++;
				if (feof(fp))
					break;
			}

			nlpage.offsets[lpo] = ftell(fw);
			printf("%d ", ftell(fw));
			fwrite(&lpage, sizeof(leafpage), 1, fw);
			lpo++;

			if (j > 0){
				nlpage.keys[j - 1] = lpage.data[0].rollno;
				printf("%d ", lpage.data[0].rollno);
			}
			printf("\n");

			tsec.rootPageIds[0] = ftell(fw);
			fwrite(&nlpage, sizeof(nonleafpage), 1, fw);

			//teachers
			for (i = 0; i < 7; i++)
			{
				fscanf(fpt, "%d,%[^,],%[^,],%[^\n]\n", &tlpage.data[i].staffid, &tlpage.data[i].name, &tlpage.data[i].college, &tlpage.data[1].dept);
				PrintTeacher(tlpage.data[i]);
				tlpage.index = i;
				trecordCount++;
				if (feof(fpt))
					break;
			}

			tnlpage.offsets[lpo] = ftell(fw);
			printf("%d ", ftell(fw));
			fwrite(&tlpage, sizeof(tleafpage), 1, fw);
			lpo++;

			if (j > 0){
				tnlpage.keys[j - 1] = tlpage.data[0].staffid;
				printf("%d ", tlpage.data[0].staffid);
			}
			printf("\n");

			tsec.rootPageIds[1] = ftell(fw);
			fwrite(&tnlpage, sizeof(nonleafpage), 1, fw);

			//update the table sector
			ft = fopen(bin, "rb+");
			fwrite(&tsec, sizeof(tablesector), 1, ft);
			fclose(ft);

			j++;

			if (feof(fp) || feof(fpt)) break;
		}

	}

	fclose(fp);
	fclose(fw);
}

main()
{
	//printf("%d %d %d %d", sizeof(teachingstaff), sizeof(tleafpage), sizeof(nonleafpage), sizeof(tablesector));

	BuildBTree3("640studs.csv", "teacherdata.csv", "store.bin");

	_getch();
}