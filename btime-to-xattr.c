#include "common.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char *argv[]) {
	if(argc < 2) {
		fprintf(stderr, "Usage: %s <path>\n", argv[0]);
		return 1;
	}

	int ret = 0;
	char *path = argv[1];
	struct statx *statxbuf = calloc(1, sizeof(struct statx));

	ret = statx(AT_FDCWD, path, AT_SYMLINK_NOFOLLOW | AT_NO_AUTOMOUNT, STATX_BTIME, statxbuf);
	if(ret < 0) {
		perror("statx failed");
		return 1;
	}

	if((statxbuf->stx_mask & STATX_BTIME) == 0) {
		fprintf(stderr, "btime not supported or not found\n");
		return 1;
	}

	struct statx_timestamp *btime = &statxbuf->stx_btime;

	printf("setting btime: sec: %llu, nsec: %u\n", btime->tv_sec, btime->tv_nsec);

	ret = setxattr(path, BTIME_XATTR_NAME, btime, sizeof(*btime), XATTR_CREATE);
	if(ret < 0) {
		if(errno == EEXIST) {
			printf("nevermind, btime xattr already set\n");
			return 0;
		} else {
			perror("setxattr failed");
			return 1;
		}
	}

	printf("btime set successfully in %s for %s\n", BTIME_XATTR_NAME, path);
	return 0;
}
