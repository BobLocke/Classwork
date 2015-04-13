#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>

#include "scheduler.h"
#include "async.h"

#define BUFSIZE 1000

void test_read_wrap1(char* filename)
{
	int READSIZE = 100;
	int for_loop_i;
	int fd = open(filename, O_RDONLY);
	char buf[BUFSIZE];
	int a;
	int cur_file_offset;
	while ((a = read_wrap(fd, buf, READSIZE)) > 0)
	{
		int a_errno = errno;
		cur_file_offset = lseek(fd, 0, SEEK_CUR);
		printf("test_read_wrap1: ");
		printf("***read_wrap success***\n");
		printf("filename: %s\n", filename);
		printf("buffer size: %d\n", BUFSIZE);
		printf("max read byte: %d\n", READSIZE);
		printf("result: %d\n", a);
		printf("error: %d\n", a_errno);
		printf("buffer: ");
		for_loop_i = 0;
		while (for_loop_i < a)
			printf("%c",buf[(for_loop_i++)]);
		printf("\nfile_offset: %d\n", cur_file_offset);
	}
	int a_errno = errno;
	cur_file_offset = lseek(fd, 0, SEEK_CUR);
	printf("test_read_wrap1: ");
	if (a == 0)
		printf("***read_wrap done*** filename: %s\n", filename);
	else {
		printf("***read_wrap error***\n");
		printf("filename: %s\n", filename);
		printf("buffer size: %d\n", BUFSIZE);
		printf("max read byte: %d\n", READSIZE);
		printf("result: %d\n", a);
		printf("error: %d\n", a_errno);
	}
	close(fd);
	return;
}

void test_read_wrap2(char* filename)
{
	int READSIZE = 3;
	int for_loop_i;
	int fd = open(filename, O_RDONLY);
	char buf[BUFSIZE];
	int a;
	int cur_file_offset;
	while ((a = read_wrap(fd, buf, READSIZE)) > 0)
	{
		int a_errno = errno;
		cur_file_offset = lseek(fd, 0, SEEK_CUR);
		printf("test_read_wrap2: ");
		printf("***read_wrap success***\n");
		printf("filename: %s\n", filename);
		printf("buffer size: %d\n", BUFSIZE);
		printf("max read byte: %d\n", READSIZE);
		printf("result: %d\n", a);
		printf("error: %d\n", a_errno);
		printf("buffer: ");
		for_loop_i = 0;
		while (for_loop_i < a)
			printf("%c",buf[(for_loop_i++)]);
		printf("\nfile_offset: %d\n", cur_file_offset);
	}
	int a_errno = errno;
	cur_file_offset = lseek(fd, 0, SEEK_CUR);
	printf("test_read_wrap2: ");
	if (a == 0)
		printf("***read_wrap done*** filename: %s\n", filename);
	else {
		printf("***read_wrap error***\n");
		printf("filename: %s\n", filename);
		printf("buffer size: %d\n", BUFSIZE);
		printf("max read byte: %d\n", READSIZE);
		printf("result: %d\n", a);
		printf("error: %d\n", a_errno);
	}
	close(fd);
	return;
}

void test_read_wrap3(char* filename)
{
	int READSIZE = 3;
	int for_loop_i;
	int fd = open(filename, O_WRONLY);
	char buf[BUFSIZE];
	int a;
	int cur_file_offset;
	while ((a = read_wrap(fd, buf, READSIZE)) > 0)
	{
		int a_errno = errno;
		cur_file_offset = lseek(fd, 0, SEEK_CUR);
		printf("test_read_wrap3: ");
		printf("***read_wrap success***\n");
		printf("filename: %s\n", filename);
		printf("buffer size: %d\n", BUFSIZE);
		printf("max read byte: %d\n", READSIZE);
		printf("result: %d\n", a);
		printf("error: %d\n", a_errno);
		printf("buffer: ");
		for_loop_i = 0;
		while (for_loop_i < a)
			printf("%c",buf[(for_loop_i++)]);
		printf("\nfile_offset: %d\n", cur_file_offset);
	}
	int a_errno = errno;
	cur_file_offset = lseek(fd, 0, SEEK_CUR);
	printf("test_read_wrap3: ");
	if (a == 0)
		printf("***read_wrap done*** filename: %s\n", filename);
	else {
		printf("***read_wrap error***\n");
		printf("filename: %s\n", filename);
		printf("buffer size: %d\n", BUFSIZE);
		printf("max read byte: %d\n", READSIZE);
		printf("result: %d\n", a);
		printf("error: %d\n", a_errno);
	}
	close(fd);
	return;
}

void test_read_wrap4(char* filename)
{
	int READSIZE = 3;
	int for_loop_i;
	int fd = open(filename, O_RDONLY);
	char buf[BUFSIZE];
	int a;
	int cur_file_offset;
	while ((a = read_wrap(fd, buf, READSIZE)) > 0)
	{
		int a_errno = errno;
		cur_file_offset = lseek(fd, 0, SEEK_CUR);
		printf("test_read_wrap4: ");
		printf("***read_wrap success***\n");
		printf("filename: %s\n", filename);
		printf("buffer size: %d\n", BUFSIZE);
		printf("max read byte: %d\n", READSIZE);
		printf("result: %d\n", a);
		printf("error: %d\n", a_errno);
		printf("buffer: ");
		for_loop_i = 0;
		while (for_loop_i < a)
			printf("%c",buf[(for_loop_i++)]);
		printf("\nfile_offset: %d\n", cur_file_offset);
	}
	int a_errno = errno;
	cur_file_offset = lseek(fd, 0, SEEK_CUR);
	printf("test_read_wrap4: ");
	if (a == 0)
		printf("***read_wrap done*** filename: %s\n", filename);
	else {
		printf("***read_wrap error***\n");
		printf("filename: %s\n", filename);
		printf("buffer size: %d\n", BUFSIZE);
		printf("max read byte: %d\n", READSIZE);
		printf("result: %d\n", a);
		printf("error: %d\n", a_errno);
	}
	close(fd);
	return;
}

void test_read_wrap5(char* filename)
{
	int READSIZE = 3;
	int for_loop_i;
	int fd = open(filename, O_RDONLY);
	char *buf = NULL;
	int a;
	int cur_file_offset;
	while ((a = read_wrap(fd, buf, READSIZE)) > 0)
	{
		int a_errno = errno;
		cur_file_offset = lseek(fd, 0, SEEK_CUR);
		printf("test_read_wrap5: ");
		printf("***read_wrap success***\n");
		printf("filename: %s\n", filename);
		printf("buffer size: %d\n", BUFSIZE);
		printf("max read byte: %d\n", READSIZE);
		printf("result: %d\n", a);
		printf("error: %d\n", a_errno);
		printf("buffer: ");
		for_loop_i = 0;
		while (for_loop_i < a)
			printf("%c",buf[(for_loop_i++)]);
		printf("\nfile_offset: %d\n", cur_file_offset);
	}
	int a_errno = errno;
	cur_file_offset = lseek(fd, 0, SEEK_CUR);
	printf("test_read_wrap5: ");
	if (a == 0)
		printf("***read_wrap done*** filename: %s\n", filename);
	else {
		printf("***read_wrap error***\n");
		printf("filename: %s\n", filename);
		printf("buffer size: %d\n", BUFSIZE);
		printf("max read byte: %d\n", READSIZE);
		printf("result: %d\n", a);
		printf("error: %d\n", a_errno);
	}
	close(fd);
	return;
}

void test_read_wrap6(char* filename)
{
	int READSIZE = -2;
	int for_loop_i;
	int fd = open(filename, O_RDONLY);
	char *buf = NULL;
	int a;
	int cur_file_offset;
	while ((a = read_wrap(fd, buf, READSIZE)) > 0)
	{
		int a_errno = errno;
		cur_file_offset = lseek(fd, 0, SEEK_CUR);
		printf("test_read_wrap6: ");
		printf("***read_wrap success***\n");
		printf("filename: %s\n", filename);
		printf("buffer size: %d\n", BUFSIZE);
		printf("max read byte: %d\n", READSIZE);
		printf("result: %d\n", a);
		printf("error: %d\n", a_errno);
		printf("buffer: ");
		for_loop_i = 0;
		while (for_loop_i < a)
			printf("%c",buf[(for_loop_i++)]);
		printf("\nfile_offset: %d\n", cur_file_offset);
	}
	int a_errno = errno;
	cur_file_offset = lseek(fd, 0, SEEK_CUR);
	printf("test_read_wrap6: ");
	if (a == 0)
		printf("***read_wrap done*** filename: %s\n", filename);
	else {
		printf("***read_wrap error***\n");
		printf("filename: %s\n", filename);
		printf("buffer size: %d\n", BUFSIZE);
		printf("max read byte: %d\n", READSIZE);
		printf("result: %d\n", a);
		printf("error: %d\n", a_errno);
	}
	close(fd);
	return;
}

void test_read1(char *filename)
{
	int READSIZE = 100;
	int for_loop_i;
	int fd = open(filename, O_RDONLY);
	char buf[BUFSIZE];
	int b;
	int cur_file_offset;
	while ((b = read(fd, buf, READSIZE)) > 0)
	{
		int b_errno = errno;
		cur_file_offset = lseek(fd, 0, SEEK_CUR);
		printf("test_read1: ");
		printf("***read success***\n");
		printf("filename: %s\n", filename);
		printf("buffer size: %d\n", BUFSIZE);
		printf("max read byte: %d\n", READSIZE);
		printf("result: %d\n", b);
		printf("error: %d\n", b_errno);
		printf("buffer: ");
		for_loop_i = 0;
		while (for_loop_i < b)
			printf("%c",buf[(for_loop_i++)]);
		printf("\nfile_offset: %d\n", cur_file_offset);
	}
	int b_errno = errno;
	cur_file_offset = lseek(fd, 0, SEEK_CUR);
	printf("test_read1: ");
	if (b == 0)
		printf("***read done*** filename: %s\n", filename);
	else {
		printf("***read error***\n");
		printf("filename: %s\n", filename);
		printf("buffer size: %d\n", BUFSIZE);
		printf("max read byte: %d\n", READSIZE);
		printf("result: %d\n", b);
		printf("error: %d\n", b_errno);
	}
	close(fd);
}

void test_read2(char *filename)
{
	int READSIZE = 3;
	int for_loop_i;
	int fd = open(filename, O_RDONLY);
	char buf[BUFSIZE];
	int b;
	int cur_file_offset;
	while ((b = read(fd, buf, READSIZE)) > 0)
	{
		int b_errno = errno;
		cur_file_offset = lseek(fd, 0, SEEK_CUR);
		printf("test_read2: ");
		printf("***read success***\n");
		printf("filename: %s\n", filename);
		printf("buffer size: %d\n", BUFSIZE);
		printf("max read byte: %d\n", READSIZE);
		printf("result: %d\n", b);
		printf("error: %d\n", b_errno);
		printf("buffer: ");
		for_loop_i = 0;
		while (for_loop_i < b)
			printf("%c",buf[(for_loop_i++)]);
		printf("\nfile_offset: %d\n", cur_file_offset);
	}
	int b_errno = errno;
	cur_file_offset = lseek(fd, 0, SEEK_CUR);
	printf("test_read2: ");
	if (b == 0)
		printf("***read done*** filename: %s\n", filename);
	else {
		printf("***read error***\n");
		printf("filename: %s\n", filename);
		printf("buffer size: %d\n", BUFSIZE);
		printf("max read byte: %d\n", READSIZE);
		printf("result: %d\n", b);
		printf("error: %d\n", b_errno);
	}
	close(fd);
}

void test_read3(char *filename)
{
	int READSIZE = 3;
	int for_loop_i;
	int fd = open(filename, O_WRONLY);
	char buf[BUFSIZE];
	int b;
	int cur_file_offset;
	while ((b = read(fd, buf, READSIZE)) > 0)
	{
		int b_errno = errno;
		cur_file_offset = lseek(fd, 0, SEEK_CUR);
		printf("test_read3: ");
		printf("***read success***\n");
		printf("filename: %s\n", filename);
		printf("buffer size: %d\n", BUFSIZE);
		printf("max read byte: %d\n", READSIZE);
		printf("result: %d\n", b);
		printf("error: %d\n", b_errno);
		printf("buffer: ");
		for_loop_i = 0;
		while (for_loop_i < b)
			printf("%c",buf[(for_loop_i++)]);
		printf("\nfile_offset: %d\n", cur_file_offset);
	}
	int b_errno = errno;
	cur_file_offset = lseek(fd, 0, SEEK_CUR);
	printf("test_read3: ");
	if (b == 0)
		printf("***read done*** filename: %s\n", filename);
	else {
		printf("***read error***\n");
		printf("filename: %s\n", filename);
		printf("buffer size: %d\n", BUFSIZE);
		printf("max read byte: %d\n", READSIZE);
		printf("result: %d\n", b);
		printf("error: %d\n", b_errno);
	}
	close(fd);
}

void test_read4(char *filename)
{
	int READSIZE = 3;
	int for_loop_i;
	int fd = open(filename, O_RDONLY);
	char buf[BUFSIZE];
	int b;
	int cur_file_offset;
	while ((b = read(fd, buf, READSIZE)) > 0)
	{
		int b_errno = errno;
		cur_file_offset = lseek(fd, 0, SEEK_CUR);
		printf("test_read4: ");
		printf("***read success***\n");
		printf("filename: %s\n", filename);
		printf("buffer size: %d\n", BUFSIZE);
		printf("max read byte: %d\n", READSIZE);
		printf("result: %d\n", b);
		printf("error: %d\n", b_errno);
		printf("buffer: ");
		for_loop_i = 0;
		while (for_loop_i < b)
			printf("%c",buf[(for_loop_i++)]);
		printf("\nfile_offset: %d\n", cur_file_offset);
	}
	int b_errno = errno;
	cur_file_offset = lseek(fd, 0, SEEK_CUR);
	printf("test_read4: ");
	if (b == 0)
		printf("***read done*** filename: %s\n", filename);
	else {
		printf("***read error***\n");
		printf("filename: %s\n", filename);
		printf("buffer size: %d\n", BUFSIZE);
		printf("max read byte: %d\n", READSIZE);
		printf("result: %d\n", b);
		printf("error: %d\n", b_errno);
	}
	close(fd);
}

void test_read5(char *filename)
{
	int READSIZE = 3;
	int for_loop_i;
	int fd = open(filename, O_RDONLY);
	char *buf = NULL;
	int b;
	int cur_file_offset;
	while ((b = read(fd, buf, READSIZE)) > 0)
	{
		int b_errno = errno;
		cur_file_offset = lseek(fd, 0, SEEK_CUR);
		printf("test_read5: ");
		printf("***read success***\n");
		printf("filename: %s\n", filename);
		printf("buffer size: %d\n", BUFSIZE);
		printf("max read byte: %d\n", READSIZE);
		printf("result: %d\n", b);
		printf("error: %d\n", b_errno);
		printf("buffer: ");
		for_loop_i = 0;
		while (for_loop_i < b)
			printf("%c",buf[(for_loop_i++)]);
		printf("\nfile_offset: %d\n", cur_file_offset);
	}
	int b_errno = errno;
	cur_file_offset = lseek(fd, 0, SEEK_CUR);
	printf("test_read5: ");
	if (b == 0)
		printf("***read done*** filename: %s\n", filename);
	else {
		printf("***read error***\n");
		printf("filename: %s\n", filename);
		printf("buffer size: %d\n", BUFSIZE);
		printf("max read byte: %d\n", READSIZE);
		printf("result: %d\n", b);
		printf("error: %d\n", b_errno);
	}
	close(fd);
}

void test_read6(char *filename)
{
	int READSIZE = -2;
	int for_loop_i;
	int fd = open(filename, O_RDONLY);
	char *buf = NULL;
	int b;
	int cur_file_offset;
	while ((b = read(fd, buf, READSIZE)) > 0)
	{
		int b_errno = errno;
		cur_file_offset = lseek(fd, 0, SEEK_CUR);
		printf("test_read6: ");
		printf("***read success***\n");
		printf("filename: %s\n", filename);
		printf("buffer size: %d\n", BUFSIZE);
		printf("max read byte: %d\n", READSIZE);
		printf("result: %d\n", b);
		printf("error: %d\n", b_errno);
		printf("buffer: ");
		for_loop_i = 0;
		while (for_loop_i < b)
			printf("%c",buf[(for_loop_i++)]);
		printf("\nfile_offset: %d\n", cur_file_offset);
	}
	int b_errno = errno;
	cur_file_offset = lseek(fd, 0, SEEK_CUR);
	printf("test_read6: ");
	if (b == 0)
		printf("***read done*** filename: %s\n", filename);
	else {
		printf("***read error***\n");
		printf("filename: %s\n", filename);
		printf("buffer size: %d\n", BUFSIZE);
		printf("max read byte: %d\n", READSIZE);
		printf("result: %d\n", b);
		printf("error: %d\n", b_errno);
	}
	close(fd);
}





/* students to complete this file to test for correct blocking behavior */
int main(void)
{
	char input1[20] = "input1.txt";
	char input2[20] = "input2.txt";
	char input3[20] = "input3";
	char input4[20] = "input4.txt";
	scheduler_begin();
	thread_fork(test_read1, (void*) input1);
	thread_fork(test_read2, (void*) input1);
	thread_fork(test_read3, (void*) input1);
	thread_fork(test_read4, (void*) input3);
	thread_fork(test_read5, (void*) input1);
	thread_fork(test_read6, (void*) input1);
	thread_fork(test_read_wrap1, (void*) input1);
	thread_fork(test_read_wrap2, (void*) input1);
	thread_fork(test_read_wrap3, (void*) input1);
	thread_fork(test_read_wrap4, (void*) input3);
	thread_fork(test_read_wrap5, (void*) input1);
	thread_fork(test_read_wrap6, (void*) input1);
	scheduler_end();
}
