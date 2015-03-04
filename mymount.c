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
        openlog ("mount.so", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
        if (!open_target_fd(&targetfd, "/proc/1/ns/mnt")){
            if (setns(targetfd, CLONE_NEWNS)){
                syslog(LOG_NOTICE, "setns failed for filesystem: %s", filesystemtype);
            }else {
                syslog(LOG_NOTICE, "setns succeeded for filesystem: %s", filesystemtype);
            }
        }else {
            syslog(LOG_NOTICE, "failed to open ns for filesystem: %s", filesystemtype);
        }
        closelog();
    }


    if (orig_mount) {
        return orig_mount(source, target, filesystemtype, mountflags, data);
    }else {
        return -ENOENT;
    }
}
