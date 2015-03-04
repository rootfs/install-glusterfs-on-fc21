#define _GNU_SOURCE
#define FUSE_MOUNT 0
#if FUSE_MOUNT
#define FUSE_USE_VERSION 26
#include <fuse.h>
#endif

#include <stdio.h>
#include <dlfcn.h>
#include <sys/mount.h>
#include <sched.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <errno.h>
#include <syslog.h>
#include <string.h>

/* Copied from sys-utils/nsenter.c */
static int open_target_fd(int *fd, const char *path)
{
	if (*fd >= 0)
		close(*fd);

	*fd = open(path, O_RDONLY);
	if (*fd < 0) {
        perror("open");
        return -1;
    }
    return 0;
}

int mount(const char *source, const char *target,
          const char *filesystemtype, unsigned long mountflags,
          const void *data)
{
    int (*orig_mount)(const char *, const char *,
                      const char *, unsigned long,
                      const void *);
    int targetfd = -1;

    orig_mount = dlsym(RTLD_NEXT, "mount");

    if (!strcmp("fuse.glusterfs", filesystemtype)) {
        if (!open_target_fd(&targetfd, "/proc/1/ns/mnt")){
            if (setns(targetfd, CLONE_NEWNS)){
                perror("setns");
            }
        }
        openlog ("mount.so", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
        syslog(LOG_NOTICE, "filesystem: %s", filesystemtype);
        closelog();
    }


    if (orig_mount) {
        return orig_mount(source, target, filesystemtype, mountflags, data);
    }else {
        return ENOENT;
    }
}

#if FUSE_MOUNT
struct fuse_chan* fuse_mount(const char *mountpoint,
                             struct fuse_args *args)
{
    struct fuse_chan* (*orig_mount)(const char *, struct fuse_args *);
    orig_mount = dlsym(RTLD_NEXT, "fuse_mount");
	if (1) {
		int targetfd = -1;
		if (!open_target_fd(&targetfd, "/proc/1/ns/mnt")){
            if (setns(targetfd, CLONE_NEWNS)){
                perror("setns");
            }
        }
	}
    fprintf(stderr, "fuse mount\n");
    return NULL;
    return orig_mount(mountpoint, args);
}
#endif
