/**
 * Facebook File System
 * Copyright (C) 2013 Wayne Chang <wayne@neverfear.org>
 *
 * This program can be distributed under the terms of the GNU GPLv3.
 * See the file COPYING.
 *
 * This code is derived from the Big Brother File System.
 * http://www.cs.nmsu.edu/~pfeiffer/fuse-tutorial/
 * Copyright (C) 2012 Joseph J. Pfeiffer, Jr., Ph.D. <pfeiffer@cs.nmsu.edu>
 * His code is licensed under the GNU GPLv3.
 *
 * The Facebook File System allows read/write access to Facebook through a
 * file system. It retrieves data using primarily the Facebook Graph API.
 *
 **/

#define FUSE_USE_VERSION 26

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fuse.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <attr/xattr.h>
#include <netinet/in.h>
#include <netdb.h>
#include <limits.h>
#include <dirent.h>

#include "fbfs.h"
#include "log.h"

static int fb_error(char *s)
{
        int rv = -errno;
        log_msg("    ERROR %s: %s\n", s, strerror(errno));
        return rv;
}

static void fb_fullpath(char fpath[PATH_MAX], const char *path)
{
        strcpy(fpath, FB_DATA->root_dir);
        strncat(fpath, path, PATH_MAX);
        log_msg("    fb_fullpath:  rootdir = \"%s\", "
                "path = \"%s\", fpath = \"%s\"\n",
                FB_DATA->root_dir, path, fpath);
}

int fb_getattr(const char *path, struct stat *statbuf)
{
        int rv = 0;
        char fpath[PATH_MAX];

        log_msg("\nfb_getattr(path=\"%s\", statbuf=0x%08x)\n",
                path, statbuf);
        fb_fullpath(fpath, path);

        rv = lstat(fpath, statbuf);
        if (rv == -1) {
                rv = fb_error("fb_getattr lstat");
        }

        return rv;
}

int fb_readlink(const char *path, char *link, size_t size)
{
        int rv = 0;
        char fpath[PATH_MAX];

        log_msg("fb_readlink(path=\"%s\", link=\"%s\", size=%d)\n",
                path, link, size);
        fb_fullpath(fpath, path);

        rv = readlink(fpath, link, size - 1);
        if (rv == -1) {
                rv = fb_error("fb_readlink readlink");
        } else {
                link[rv] = '\0';
                rv = 0;
        }

        return rv;
}

int fb_mknod(const char *path, mode_t mode, dev_t dev)
{
        int rv = 0;
        char fpath[PATH_MAX];

        log_msg("\nfb_mknod(path=\"%s\", mode=0%3o, dev=%lld)\n",
                  path, mode, dev);
        fb_fullpath(fpath, path);

        if (S_ISREG(mode)) {
                rv = open(fpath, O_CREAT | O_EXCL | O_WRONLY, mode);
                if (rv == -1)
                        rv = fb_error("fb_mknod open");
                else {
                        rv = close(rv);
                        if (rv == -1) {
                                rv = fb_error("fb_mknod close");
                        }
                }
        } else {
                if (S_ISFIFO(mode)) {
                        rv = mkfifo(fpath, mode);
                        if (rv == -1) {
                                rv = fb_error("fb_mknod mkfifo");
                        }
                } else {
                        rv = mknod(fpath, mode, dev);
                        if (rv == -1) {
                                rv = fb_error("fb_mknod mknod");
                        }
                }
        }

        return rv;
}

int fb_mkdir(const char *path, mode_t mode)
{
        int rv = 0;
        char fpath[PATH_MAX];

        log_msg("\nfb_mkdir(path=\"%s\", mode=0%3o)\n",
                path, mode);

        fb_fullpath(fpath, path);

        rv = mkdir(fpath, mode);
        if (rv == -1) {
                rv = fb_error("fb_mkdir mkdir");
        }

        return rv;
}

int fb_unlink(const char *path)
{
        int rv = 0;
        char fpath[PATH_MAX];

        log_msg("fb_unlink(path=\"%s\")\n",
                path);
        fb_fullpath(fpath, path);

        rv = unlink(fpath);
        if (rv == -1) {
                rv = fb_error("fb_unlink unlink");
        }

        return rv;

}

int fb_rmdir(const char *path)
{
        int rv = 0;
        char fpath[PATH_MAX];

        log_msg("fb_rmdir(path=\"%s\")\n",
                path);
        fb_fullpath(fpath, path);

        rv = rmdir(fpath);
        if (rv == -1) {
                rv = fb_error("fb_rmdir rmdir");
        }

        return rv;
}

int fb_symlink(const char *path, const char *link)
{
        int rv = 0;
        char flink[PATH_MAX];

        log_msg("\nfb_symlink(path=\"%s\", link=\"%s\")\n",
                path, link);
        fb_fullpath(flink, link);

        rv = symlink(path, flink);
        if (rv == -1) {
                rv = fb_error("fb_symlink symlink");
        }

        return rv;
}

int fb_rename(const char *path, const char *newpath)
{
        int rv = 0;
        char fpath[PATH_MAX];
        char fnewpath[PATH_MAX];

        log_msg("\nfb_rename(fpath=\"%s\", newpath=\"%s\")\n",
                        path, newpath);
        fb_fullpath(fpath, path);
        fb_fullpath(fnewpath, newpath);

        rv = rename(fpath, fnewpath);
        if (rv == -1) {
                rv = fb_error("fb_rename rename");
        }

        return rv;
}

int fb_link(const char *path, const char *newpath)
{
        int rv = 0;
        char fpath[PATH_MAX], fnewpath[PATH_MAX];

        log_msg("\nfb_link(path=\"%s\", newpath=\"%s\")\n",
                        path, newpath);
        fb_fullpath(fpath, path);
        fb_fullpath(fnewpath, newpath);

        rv = link(fpath, fnewpath);
        if (rv == -1) {
                rv = fb_error("fb_link link");
        }

        return rv;
}

int fb_chmod(const char *path, mode_t mode)
{
        int rv = 0;
        char fpath[PATH_MAX];

        log_msg("\nfb_chmod(fpath=\"%s\", mode=0%03o)\n",
                path, mode);
        fb_fullpath(fpath, path);

        rv = chmod(fpath, mode);
        if (rv == -1) {
                rv = fb_error("fb_chmod chmod");
        }

        return rv;
}

int fb_chown(const char *path, uid_t uid, gid_t gid)
{
        int rv = 0;
        char fpath[PATH_MAX];

        log_msg("\nfb_chown(path=\"%s\", uid=%d, gid=%d)\n",
                path, uid, gid);
        fb_fullpath(fpath, path);

        rv = chown(fpath, uid, gid);
        if (rv == -1) {
                rv = fb_error("fb_chown chown");
        }

        return rv;
}

int fb_truncate(const char *path, off_t newsize)
{
        int rv = 0;
        char fpath[PATH_MAX];

        log_msg("\nfb_truncate(path=\"%s\", newsize=%lld)\n",
            path, newsize);
        fb_fullpath(fpath, path);

        rv = truncate(fpath, newsize);
        if (rv == -1) {
                fb_error("fb_truncate truncate");
        }

        return rv;
}

int fb_utime(const char *path, struct utimbuf *ubuf)
{
        int rv = 0;
        char fpath[PATH_MAX];

        log_msg("\nfb_utime(path=\"%s\", ubuf=0x%08x)\n",
                        path, ubuf);
        fb_fullpath(fpath, path);

        rv = utime(fpath, ubuf);
        if (rv == -1) {
                rv = fb_error("fb_utime utime");
        }

        return rv;
}

int fb_open(const char *path, struct fuse_file_info *fi)
{
        int rv = 0;
        int fd;
        char fpath[PATH_MAX];

        log_msg("\nfb_open(path\"%s\", fi=0x%08x)\n",
                path, fi);
        fb_fullpath(fpath, path);

        fd = open(fpath, fi->flags);
        if (fd == -1) {
                rv = fb_error("fb_open open");
        }

        fi->fh = fd;

        return rv;
}

int fb_read(const char *path, char *buf, size_t size, off_t offset,
            struct fuse_file_info *fi)
{
        int rv = 0;

        log_msg("\nfb_read(path=\"%s\", buf=0x%08x, "
                "size=%d, offset=%lld, fi=0x%08x)\n",
                path, buf, size, offset, fi);

        /* no need to get fpath, get got a handle! */
        rv = pread(fi->fh, buf, size, offset);
        if (rv == -1) {
                rv = fb_error("fb_read read");
        }

        return rv;
}

int fb_write(const char *path, const char *buf, size_t size,
             off_t offset, struct fuse_file_info *fi)
{
        int rv = 0;

        log_msg("\nfb_write(path=\"%s\", buf=0x%08x, size=%d, "
                "offset=%lld, fi=0x%08x)\n",
                path, buf, size, offset, fi);

        /* no need to get fpath, we have a handle! */
        rv = pwrite(fi->fh, buf, size, offset);
        if (rv == -1) {
                rv = fb_error("fb_write pwrite");
        }

        return rv;
}

int fb_statfs(const char *path, struct statvfs *statv)
{
        int rv = 0;
        char fpath[PATH_MAX];

        log_msg("\nfb_statfs(path=\"%s\", statv=0x%08x)\n",
                        path, statv);
        fb_fullpath(fpath, path);

        /* get stats for underlying filesystem */
        rv = statvfs(fpath, statv);
        if (rv == -1) {
                rv = fb_error("fb_statfs statvfs");
        }

        return rv;
}

int fb_flush(const char *path, struct fuse_file_info *fi) {
        int rv = 0;

        log_msg("\nfb_flush(path=\"%s\", fi=0x%08x)\n", path, fi);
        return rv;
}

int fb_release(const char *path, struct fuse_file_info *fi) {
        int rv = 0;

        log_msg("\nfb_release(path=\"%s\", fi=0x%08x)\n",
                path, fi);

        /* free any resources used */
        rv = close(fi->fh);

        return rv;
}

int fb_fsync(const char *path, int datasync, struct fuse_file_info *fi)
{
        int rv = 0;

        log_msg("\nfb_fsync(path=\"%s\", datasync=%d, fi=0x%08x)\n",
                        path, datasync, fi);

        if (datasync) {
                rv = fdatasync(fi->fh);
        } else {
                rv = fsync(fi->fh);
        }

        if (rv == -1) {
                fb_error("fb_fsync fsync");
        }

        return rv;
}

int fb_setxattr(const char *path, const char *name, const char *value,
                size_t size, int flags)
{
        int rv = 0;
        char fpath[PATH_MAX];

        log_msg("\nfb_setxattr(path=\"%s\", name=\"%s\", value=\"%s\", "
                "size=%d, flags=0x%08x)\n",
                path, name, value, size, flags);
        fb_fullpath(fpath, path);

        rv = lsetxattr(fpath, name, value, size, flags);
        if (rv == -1) {
                rv = fb_error("fb_setxattr lsetxattr");
        }

        return rv;
}

int fb_getxattr(const char *path, const char *name, char *value, size_t size)
{
        int rv = 0;
        char fpath[PATH_MAX];

        log_msg("\nfb_getxattr(path = \"%s\", name = \"%s\", "
                "value = 0x%08x, size = %d)\n",
                path, name, value, size);
        fb_fullpath(fpath, path);

        rv = lgetxattr(fpath, name, value, size);
        if (rv == -1) {
                rv = fb_error("fb_getxattr lgetxattr");
        } else {
                log_msg("        value = \"%s\"\n", value);
        }

        return rv;
}

int fb_listxattr(const char *path, char *list, size_t size)
{
        int rv = 0;
        char fpath[PATH_MAX];
        char *ptr;

        log_msg("fb_listxattr(path=\"%s\", list=0x%08x, size=%d)\n",
                        path, list, size
                        );
        fb_fullpath(fpath, path);

        rv = llistxattr(fpath, list, size);
        if (rv == -1) {
                rv = fb_error("fb_listxattr llistxattr");
        }

        log_msg("        returned attributes (length %d):\n", rv);
        for (ptr = list; ptr < list + rv; ptr += strlen(ptr)+1)
                log_msg("        \"%s\"\n", ptr);

        return rv;
}

int fb_removexattr(const char *path, const char *name)
{
        int rv = 0;
        char fpath[PATH_MAX];

        log_msg("\nfb_removexattr(path=\"%s\", name=\"%s\")\n",
                        path, name);
        fb_fullpath(fpath, path);

        rv = lremovexattr(fpath, name);
        if (rv == -1) {
                rv = fb_error("fb_removexattr lrmovexattr");
        }

        return rv;
}

int fb_opendir(const char *path, struct fuse_file_info *fi)
{
        int rv = 0;
        char fpath[PATH_MAX];
        DIR *dp;

        log_msg("\nfb_opendir(path=\"%s\", fi=0x%08x)\n",
                path, fi);
        fb_fullpath(fpath, path);

        dp = opendir(fpath);
        if (dp == NULL) {
                rv = fb_error("fb_opendir opendir");
        }

        fi->fh = (intptr_t) dp;

        return rv;
}

int fb_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
               off_t offset, struct fuse_file_info *fi)
{
        int rv = 0;
        DIR *dp;
        struct dirent *de;

        log_msg("\nfb_readdir(path=\"%s\", buf=0x%08x, "
                "filler=0x%08x, offset=%lld, fi=0x%08x)\n",
                path, buf, filler, offset, fi);

        /* no need for fullpath, we got a handle! */
        dp = (DIR *) (uintptr_t) fi->fh;

        /* check for ./ and ../ */
        de = readdir(dp);
        if (de == 0) {
                rv = fb_error("fb_readdir readdir");
                return rv;
        }

        /* use the filler to load our directory names */
        do {
                log_msg("calling filler with name %s\n", de->d_name);
                if (filler(buf, de->d_name, NULL, 0) != 0) {
                        log_msg("    ERROR fb_readdir filler:  buffer full");
                        return -ENOMEM;
                }
        } while ((de = readdir(dp)) != NULL);

        return rv;
}

int fb_releasedir(const char *path, struct fuse_file_info *fi)
{
        int rv = 0;

        log_msg("\nfb_releasedir(path=\"%s\", fi=0x%08x)\n",
                path, fi);

        closedir((DIR *) (uintptr_t) fi->fh);

        return rv;
}

int fb_fsyncdir(const char *path, int datasync, struct fuse_file_info *fi)
{
        int rv = 0;

        log_msg("\nfb_fsyncdir(path=\"%s\", datasync=%d, fi=0x%08x)\n",
                path, datasync, fi);

        return rv;
}

void *fb_init(struct fuse_conn_info *conn)
{
    log_msg("\nfb_init()\n");

    return FB_DATA;
}

void fb_destroy(void *userdata)
{
    log_msg("\nfb_destroy(userdata=0x%08x)\n", userdata);
}



int fb_access(const char *path, int mask)
{
        int rv = 0;
        char fpath[PATH_MAX];

        log_msg("\nfb_access(path=\"%s\", mask=0%o)\n",
                path, mask);
        fb_fullpath(fpath, path);

        rv = access(fpath, mask);

        if (rv == -1) {
                rv = fb_error("fb_access access");
        }

        return rv;
}

int fb_create(const char *path, mode_t mode, struct fuse_file_info *fi) {
        int rv = 0;
        char fpath[PATH_MAX];
        int fd;

        log_msg("\nfb_create(path=\"%s\", mode=0%03o, fi=0x%08x)\n",
                path, mode, fi);
        fb_fullpath(fpath, path);

        fd = creat(fpath, mode);
        if (fd == -1) {
                rv = fb_error("fb_create creat");
        }

        fi->fh = fd;

        return rv;
}

int fb_ftruncate(const char *path, off_t offset, struct fuse_file_info *fi)
{
        int rv = 0;

        log_msg("\nfb_ftruncate(path=\"%s\", offset=%lld, fi=0x%08x)\n",
                path, offset, fi);

        rv = ftruncate(fi->fh, offset);
        if (rv == -1) {
                rv = fb_error("fb_ftruncate ftruncate");
        }

        return rv;
}

int fb_fgetattr(const char *path, struct stat *statbuf,
                struct fuse_file_info *fi)
{
        int rv = 0;

        log_msg("\nfb_fgetattr(path=\"%s\", statbuf=0x%08x, fi=0x%08x)\n",
                        path, statbuf, fi);

        rv = fstat(fi->fh, statbuf);
        if (rv == -1) {
                        rv = fb_error("fb_fgetattr fstat");
        }

        return rv;
}

static struct fuse_operations fb_oper = {
        .getattr = fb_getattr,
        .readlink = fb_readlink,
        .getdir = NULL,
        .mknod = fb_mknod,
        .mkdir = fb_mkdir,
        .unlink = fb_unlink,
        .rmdir = fb_rmdir,
        .symlink = fb_symlink,
        .rename = fb_rename,
        .link = fb_link,
        .chmod = fb_chmod,
        .chown = fb_chown,
        .truncate = fb_truncate,
        .utime = fb_utime,
        .open = fb_open,
        .read = fb_read,
        .write = fb_write,
        .statfs = fb_statfs,
        .flush = fb_flush,
        .release = fb_release,
        .setxattr = fb_setxattr,
        .getxattr = fb_getxattr,
        .listxattr = fb_listxattr,
        .removexattr = fb_removexattr,
        .opendir = fb_opendir,
        .readdir = fb_readdir,
        .releasedir = fb_releasedir,
        .fsyncdir = fb_fsyncdir,
        .init = fb_init,
        .destroy = fb_destroy,
        .access = fb_access,
        .create = fb_create,
        .ftruncate = fb_ftruncate,
        .fgetattr = fb_fgetattr
};

int main(int argc, char *argv[]) {
        int rv;
        fb_state *fb_data;

        fb_data = malloc(sizeof(fb_state));
        if (fb_data == NULL) {
                perror("malloc");
                exit(-1);
        }

        fb_data->root_dir = realpath(argv[argc-2], NULL);
        argv[argc-2] = argv[argc-1];
        argv[argc-1] = NULL;
        argc--;

        fb_data->log_file = log_open();

        fprintf(stderr, "calling fuse_main\n");
        rv = fuse_main(argc, argv, &fb_oper, fb_data);
        fprintf(stderr, "back form fuse_main, rv = %d\n", rv);

        return rv;
}

