#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/gmon_out.h>

#define GMON_SIZE	8*1024*1024
#define GMON_HDR	(struct gmon_hdr) {GMON_MAGIC, GMON_VERSION, 0, }

int main(int argc, char* argv[])
{
	char* gmon_out;
	char* buf;

	FILE * fd_in;
	int fd_out;

	unsigned long from, self;
	int count;
	int num_record = 0;

	gmon_out = (char*) malloc(GMON_SIZE);

	*(struct gmon_hdr*) gmon_out = GMON_HDR;
	buf = gmon_out + sizeof(struct gmon_hdr);

	/* dummy histogram record */
	*buf++ = GMON_TAG_TIME_HIST;
	*(unsigned long *) buf = 0; buf += sizeof(long);	/*lowpc */
	*(unsigned long *) buf =-1; buf += sizeof(long);	/* highpc */
	*(int *) buf = 0; buf += 4;				/* count */
	*(int *) buf = 1024; buf += 4;				/* prorate */
	strncpy(buf, "seconds", 15); buf += 15;
	*buf++='s';

	fd_in = fopen(argv[1], "r");
	fgets(buf, 10, fd_in);

	while (fscanf(fd_in, "%lx, %lx, %d\n", &from, &self, &count) == 3) {
		num_record++;

		*buf++ = GMON_TAG_CG_ARC;
		memcpy(buf, &from, sizeof(from)); buf += sizeof(from);
		memcpy(buf, &self, sizeof(self)); buf += sizeof(self);
		memcpy(buf, &count, sizeof(count)); buf+= sizeof(count);
	}

	fd_out = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC, 0666);
	write(fd_out, gmon_out, buf-gmon_out);

	printf("%d record scanned\n", num_record);
}
