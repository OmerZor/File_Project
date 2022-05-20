#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE
/*#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>*/ //uncomment this block to check for heap memory allocation leaks.
// Read https://docs.microsoft.com/en-us/visualstudio/debugger/finding-memory-leaks-using-the-crt-library?view=vs-2019
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
//19.5 23:24
typedef struct StudentCourseGrade
{
	char courseName[35];
	int grade;
}StudentCourseGrade;

typedef struct Student
{
	char name[35];
	StudentCourseGrade* grades; //dynamic array of courses
	int numberOfCourses;
}Student;


//Part A
void countStudentsAndCourses(const char* fileName, int** coursesPerStudent, int* numberOfStudents);
int countPipes(const char* lineBuffer, int maxCount);
char** makeStudentArrayFromFile(const char* fileName, int** coursesPerStudent, int* numberOfStudents);
void printStudentArray(const char* const* const* students, const int* coursesPerStudent, int numberOfStudents);
void factorGivenCourse(char** const* students, const int* coursesPerStudent, int numberOfStudents, const char* courseName, int factor);
void studentsToFile(char** students, int coursesPerStudent, int numberOfStudents);
int countline(char* filename);

//Part B
Student* transformStudentArray(char*** students, const int* coursesPerStudent, int numberOfStudents);
void writeToBinFile(const char* fileName, Student* students, int numberOfStudents);
Student* readFromBinFile(const char* fileName);

int main()
{
	//Part A
	int* coursesPerStudent = NULL;
	int numberOfStudents = 0;
	char*** students = makeStudentArrayFromFile("studentList.txt", &coursesPerStudent, &numberOfStudents);
	factorGivenCourse(students, coursesPerStudent, numberOfStudents, "Advanced Topics in C", +5);
	printStudentArray(students, coursesPerStudent, numberOfStudents);
	//studentsToFile(students, coursesPerStudent, numberOfStudents); //this frees all memory. Part B fails if this line runs. uncomment for testing (and comment out Part B)

	//Part B
	Student* transformedStudents = transformStudentArray(students, coursesPerStudent, numberOfStudents);
	writeToBinFile("students.bin", transformedStudents, numberOfStudents);
	Student* testReadStudents = readFromBinFile("students.bin");

	for (int i = 0; i < numberOfStudents; i++)
	{
		for  (int j = 0; j < *coursesPerStudent;j++)
		{
			free(transformedStudents[i].grades[j].courseName);
			free(testReadStudents[i].grades[j].courseName);
		}
	}


	// _CrtDumpMemoryLeaks(); / //uncomment this block to check for heap memory allocation leaks.
	// Read https://docs.microsoft.com/en-us/visualstudio/debugger/finding-memory-leaks-using-the-crt-library?view=vs-2019

	return 0;
}

void countStudentsAndCourses(const char* fileName, int** coursesPerStudent, int* numberOfStudents)
{
	int check = 0;
	char line[256];
	int i = 0, j = 0, k = 0;
	char baf = 'a';
	FILE* filePtr = fopen(fileName, "r");
	assert(filePtr);
	*numberOfStudents = countline(filePtr);

	*coursesPerStudent = (int*)malloc((*numberOfStudents) * sizeof(int));


	while (!feof(filePtr))
	{
		i = 0;

		while ((baf != '\n') && (baf != EOF))
		{
			baf = fgetc(filePtr);
			line[i] = baf;
			i++;
		}
		(*coursesPerStudent)[j] = countPipes(line, 256);
		for (int r = 0; r < 256; r++)
		{
			line[r] = '\0';
		}
		baf = fgetc(filePtr);
		j++;

	}
	fclose(filePtr);
}

int countPipes(const char* lineBuffer, int maxCount)
{
	int counter = 0;
	int i = 0;
	if (lineBuffer == NULL)
	{
		return -1;
	}
	if (maxCount <= 0)
	{
		return 0;
	}

	while ((lineBuffer[i]) != '\0' || maxCount > i)
	{
		i++;
		if ((lineBuffer[i]) == '|')
		{
			counter++;
		}
		*(lineBuffer + i);
	}
	return counter;
}

char** makeStudentArrayFromFile(const char* fileName, int** coursesPerStudent, int* numberOfStudents)
{
	int x = 0;
	int j = 0, k = 0, w = 0;
	char temp[50];
	char baf = 'a';
	FILE* filePtr = fopen(fileName, "r");
	countStudentsAndCourses(fileName, coursesPerStudent, numberOfStudents);
	char*** studentArr = (char***)malloc(sizeof(char*));
	assert(studentArr);
	x = *numberOfStudents;
	for (int i = 0; i < *numberOfStudents; i++)
	{

		studentArr[i] = (char**)malloc(x * sizeof(char*));
		assert(*studentArr[i]);
	}
	while (!feof(filePtr))
	{

		while ((baf != '\n') && (baf != EOF))
		{
			baf = 'a';
			while ((baf != '|') && (baf != ',') && (baf != '\n') && (baf != EOF))
			{
				baf = fgetc(filePtr);
				if (baf == EOF)
					break;
				if ((baf != '|') && (baf != ','))
					temp[j] = baf;

				j++;

			}
			temp[j] = '\0';
			studentArr[w][k] = (char**)malloc(j * sizeof(char*));
			strcpy(studentArr[w][k], temp);
			for (int t = 0; t < j; t++)
				temp[t] = '\0';
			j = 0;
			k++;

		}
		w++;
		k = 0;
		baf = 'a';

	}
	fclose(filePtr);
	return studentArr;

}

void factorGivenCourse(char** const* students, const int* coursesPerStudent, int numberOfStudents, const char* courseName, int factor)
{
	if ((factor > 20) && (factor < -20))
	{
		printf("the factor is not in the pascal\n ");
		return -1;
	}


	char buffer[4];
	int grade = 0;
	for (int i = 0; i < numberOfStudents; i++)
	{
		for (int j = 0; j < ((coursesPerStudent[i]) * 2) + 1; j++)
		{
			if (strcmp(courseName, students[i][j]) == 0)
			{
				grade = atoi(students[i][j + 1]) + factor;
				if (grade > 100)
					grade = 100;
				if (grade < 0)
					grade = 0;
				strcpy(students[i][j + 1], itoa(grade, buffer, 10));



			}


		}

	}
}

void printStudentArray(const char* const* const* students, const int* coursesPerStudent, int numberOfStudents)
{
	for (int i = 0; i < numberOfStudents; i++)
	{
		printf("name: %s\n*****\n", students[i][0]);
		for (int j = 1; j <= 2 * coursesPerStudent[i]; j += 2)
		{
			printf("course: %s\n", students[i][j]);
			printf("grade: %s\n", students[i][j + 1]);
		}
		printf("\n");
	}
}

void studentsToFile(char*** students, int* coursesPerStudent, int numberOfStudents)
{

	int i = 0;
	FILE* filePtr = fopen("studentList.txt", "w");
	assert(filePtr);
	while (i != numberOfStudents)
	{
		for (int j = 0; j < ((*coursesPerStudent) * 2) + 1; j++)
		{
			fprintf(filePtr, "%s", *students[i][j]);
			fprintf(filePtr, "  ");
			; free(*students[i][j]);
		}
		fprintf(filePtr, "/n");
		i++;
	}

	free(students);
}

void writeToBinFile(const char* fileName, Student* students, int numberOfStudents)
{

	FILE* filePtr;
	filePtr= fopen(fileName,"wb");
	assert(filePtr);
	char buffer[35];
	fwrite(numberOfStudents, sizeof(numberOfStudents), 4, fileName);


	for (int i = 0; i < numberOfStudents; i++)
	{
		fwrite('	', 1, 1, fileName);
		strcpy(buffer, students[i].name);
		fwrite(buffer, sizeof(buffer), 1, fileName);
		fwrite(students[i].numberOfCourses, sizeof(students[i].numberOfCourses), 4, fileName);
		
		for (int j = 0; j < students[i].numberOfCourses; j++)
		{
			strcpy(buffer, students[i].grades->courseName[j]);
			fwrite('	', 1, 1, fileName);
			fwrite(buffer, sizeof(buffer), 1, fileName);
		}
	}
	fclose(fileName);

}

Student* readFromBinFile(const char* fileName)
{
	FILE* fPtr = fopen(fileName, "rb");
	assert(fPtr);
	int numberOfStudent=0;
	int numOfCurses=0;
	fread(&numberOfStudent,sizeof(int),sizeof(int), fPtr);
	Student* studArr = (Student*)malloc(numberOfStudent * sizeof(Student));
	assert(studArr);

	for (int i = 0; i < numberOfStudent; i++)
	{
		fread(studArr[i].name, sizeof(studArr[i].name), 1, fPtr);
		fread(numOfCurses, sizeof(numOfCurses), 4, fPtr);
		studArr[i].grades = (StudentCourseGrade*)malloc(numOfCurses * sizeof(StudentCourseGrade));
		assert(studArr[i].grades);
		for (int j = 0; j < numOfCurses; j++)
		{
			fread(studArr[i].grades[j].courseName, sizeof(studArr[i].grades->courseName), 1, fPtr);
		}
	}
	fclose(fPtr);
	return studArr;
}

Student* transformStudentArray(char*** students, const int* coursesPerStudent, int numberOfStudents)
{

	Student* studArr = (Student*)malloc(numberOfStudents * sizeof(Student));
	assert(studArr);
	for (int i = 0; i < numberOfStudents; i++)
	{
		strcpy(studArr[i].name, students[i][0]);
		studArr[i].numberOfCourses = coursesPerStudent[i];
		for (int j = 1; j < (coursesPerStudent[i] * 2) - 1; j += 2)
		{
			studArr[i].grades = (StudentCourseGrade*)malloc((coursesPerStudent[i]) * (sizeof(StudentCourseGrade)));
			assert(studArr[i].grades);
			strcpy(studArr[i].grades[j].courseName, students[i][j]);
			studArr[i].grades->grade = atoi(students[i][j + 1]);
		}
	}
	return studArr;
}

//prvite function
int countline(FILE* flPtr)
{
	char buffer;
	int rc = 0, counter = 0;

	if (!flPtr)
	{
		printf("the file failed to open\n");
		fclose(flPtr);
		return;
	}

	while (rc != EOF)
	{
		rc = fscanf(flPtr, "%c", &buffer);
		if (buffer == '\n')
			counter++;

	}

	if (counter != 0)
	{
		counter++;
	}
	rewind(flPtr);


	return counter;
}
