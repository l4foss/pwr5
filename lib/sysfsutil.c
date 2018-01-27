#include "sysfsutil.h"

/*
 * write a buffer to sysfs
 */
ssize_t util_write_buf_to_sysfs(char *fpath, char *buf, size_t size) {
	int fd;
	ssize_t written;

	fd = open(fpath, O_WRONLY);
	if (fd == -1) {
		return -1;
	}
#ifdef DEBUG_ON
	PWR5_DEBUGA("Opened fd %d to write\n", fd);
#endif
	written = write(fd, buf, size);
	if (written < 1) {
		close(fd);
		return -1;
	}

	close(fd);
	return written;
}

/*
 * read sysfs file and return a buffer
 */
char *util_read_buf_from_sysfs(char *fpath, size_t size) {
	int fd;
	ssize_t cread;

	char *buf = malloc(size + 1);
	if (!buf) {
		return NULL;
	}

	fd = open(fpath, O_RDONLY);
	if (fd == -1) {
		goto error;
	}
	
#ifdef DEBUG_ON
	PWR5_DEBUGA("Opened fd %d to read\n", fd);
#endif
	cread = read(fd, buf, size);
	if (cread < 1) {
		close(fd);
		goto error;
	}
	buf[size] = '\0';
	return buf;

error:
	free(buf);
	return NULL;
}

/*
 * read integer from sysfs
 */
ulong_t util_read_ulong_from_sysfs(char *fpath) {
	char *endp;
	ulong_t value;
	char *buf = util_read_buf_from_sysfs(fpath, MAX_LINE_LEN);
	if (!buf) {
		return 0;
	}
#ifdef DEBUG_ON
	PWR5_DEBUGA("buf is: %s\n", buf);
#endif

	value = strtoul(buf, &endp, 0);

	if (endp == buf || errno == ERANGE) {
		free(buf);
		return 0;
	}
	free(buf);
	return value;
}

/*
 * write an integer to sysfs file
 */
ssize_t util_write_ulong_to_sysfs(char *fpath, ulong_t num) {
	int ret;
	char buf[MAX_LINE_LEN];

	sprintf(buf, "%ld", num);
#ifdef DEBUG_ON
	PWR5_DEBUGA("buf is: %s\n", buf);
#endif

	ret = util_write_buf_to_sysfs(fpath, buf, sizeof(buf));
	if (ret < 0) {
		return -1;
	}
	return ret;
}
