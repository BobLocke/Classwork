#include "scheduler.h"
#include <aio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>

#include "async.h"

//#define DEBUG

#ifdef DEBUG
#include <assert.h>
#endif

/* Students to complete this read_wrap method */
ssize_t read_wrap(int fd, void* buf, size_t count)
{
	//Return an error if file descriptor is an error.
	if (fd < 0)
	{
		#ifdef DEBUG
			fprintf(stderr, "ERROR: Invalid file descriptor given to read_wrap()\n");
		#endif
		errno = EBADF;
		return -1;
	}
	// Return an error if there is no buffer
	if (buf == NULL)
	{
		#ifdef DEBUG
			fprintf(stderr, "ERROR: No buffer given to read_wrap()\n");
		#endif
		errno = EFAULT;
		return -1;
	}



	// Create the control block structure
	struct aiocb cb;
	memset(&cb, 0, sizeof(struct aiocb));
	cb.aio_nbytes = count;
	cb.aio_fildes = fd;
	//printf("seekcur before: %d", lseek(fd, 0, SEEK_CUR));
	cb.aio_offset = lseek(fd, 0, SEEK_CUR);
	//printf(" async: Offset: %d\n",cb.aio_offset);
	if (cb.aio_offset == -1 && errno == ESPIPE)
		cb.aio_offset = 0;
	cb.aio_buf = buf;

	// Call the read function
	if (aio_read(&cb) == -1)
	{
		#ifdef DEBUG
			fprintf(stderr, "ERROR: Call to aio_read() failed in read_wrap()\n");
		#endif
		errno = aio_error(&cb);
		return -1;
	}

	// Block until request is finished
	while (aio_error(&cb) == EINPROGRESS)
		yield();

	// Determine success of read
	ssize_t numBytes = aio_return(&cb);
	if (numBytes != -1)
	{
		// success
		lseek(fd, numBytes + cb.aio_offset, SEEK_SET);
		errno = aio_error(&cb);
		return numBytes;
	}

	// failed

	#ifdef DEBUG
		fprintf(stderr, "ERROR: Read failed in read_wrap()\n");
	#endif
	errno = aio_error(&cb);
	return -1;
}
