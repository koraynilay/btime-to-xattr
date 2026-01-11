#include "common.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sysexits.h>

void usage(char *progname) {
	fprintf(stderr, "Usage: %s <path> [rR]\n", progname);
}

int main(int argc, char *argv[]) {
	if(argc < 2) {
		usage(argv[0]);
		return EX_USAGE;
	}

	int xattr_flags = XATTR_CREATE; // strictly create
	if(argc == 3) {
		switch(argv[2][0]) {
			case 'r': // strictly replace
				xattr_flags = XATTR_REPLACE;
				break;
			case 'R': // replace or create
				xattr_flags = 0;
				break;
			default:
				usage(argv[0]);
				return EX_USAGE;
		}
	}

	int ret = 0;
	char *path = argv[1];
	struct statx *statxbuf = calloc(1, sizeof(struct statx));

	ret = statx(AT_FDCWD, path, AT_SYMLINK_NOFOLLOW | AT_NO_AUTOMOUNT, STATX_BTIME, statxbuf);
	if(ret < 0) {
		if(errno == ENOENT)
			fprintf(stderr, "Could not find '%s'", path);
		else
			perror("statx failed");
		return EX_NOINPUT;
	}

	if((statxbuf->stx_mask & STATX_BTIME) == 0) {
		fprintf(stderr, "btime not supported or not found\n");
		return EX_UNAVAILABLE;
	}

	struct statx_timestamp *btime = &statxbuf->stx_btime;

	printf("setting btime: sec: %llu, nsec: %u\n", btime->tv_sec, btime->tv_nsec);

	ret = setxattr(path, BTIME_XATTR_NAME, btime, sizeof(*btime), xattr_flags);
	if(ret < 0) {
		if(errno == EEXIST) {
			printf("btime xattr already set\n");
			return EX_SOFTWARE;
		} else if(errno == ENODATA) {
			printf("btime xattr doesn't exist\n");
			return EX_SOFTWARE;
		} else {
			perror("setxattr failed");
			return EX_NOINPUT;
		}
	}

	printf("btime set successfully in %s for %s\n", BTIME_XATTR_NAME, path);
	return EX_OK;
}
