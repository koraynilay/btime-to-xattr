#include "common.h"

#include <stdio.h>
#include <unistd.h>
#include <sysexits.h>

int main(int argc, char *argv[]) {
	if(argc < 2) {
		fprintf(stderr, "Usage: %s <path> [date arguments]\n", argv[0]);
		return EX_USAGE;
	}

	int ret = 0;
	char *path = argv[1];
	struct statx_timestamp btime;

	ret = getxattr(path, BTIME_XATTR_NAME, (void*)&btime, sizeof(btime));
	if(ret < 0) {
		perror("getxattr failed");
		return EX_NOINPUT;
	}

	printf("found btime: sec: %llu, nsec: %u\n", btime.tv_sec, btime.tv_nsec);

	char time[10+1+9];
	sprintf(time, "@%llu.%09u", btime.tv_sec, btime.tv_nsec);

	char *args[argc+3];
	int i = 0;
	args[i++] = "date";
	args[i++] = "-d";
	args[i++] = time;
	for(int j = 2; j < argc+3; j++) {
		args[i++] = argv[j];
	}
	args[i] = NULL;

	execvp("date", args);

	return EX_OK;
}
