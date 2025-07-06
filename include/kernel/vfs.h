#ifndef KERNEL_VFS_H
#define KERNEL_VFS_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

// File types
#define FT_NONE   0
#define FT_FILE   1
#define FT_DIR    2
#define FT_CHAR   3
#define FT_BLOCK  4
#define FT_PIPE   5
#define FT_SYMLINK 6
#define FT_SOCKET  7

// File flags
#define O_RDONLY    0x0000
#define O_WRONLY    0x0001
#define O_RDWR      0x0002
#define O_APPEND    0x0008
#define O_CREAT     0x0200
#define O_TRUNC     0x0400
#define O_EXCL      0x0800
#define O_NOFOLLOW  0x1000
#define O_PATH      0x2000
#define O_NONBLOCK  0x4000
#define O_DIRECTORY 0x8000

// File seek origins
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

// File mode bits
#define S_IFMT   0xF000  // File type mask
#define S_IFSOCK 0xC000  // Socket
#define S_IFLNK  0xA000  // Symbolic link
#define S_IFREG  0x8000  // Regular file
#define S_IFBLK  0x6000  // Block device
#define S_IFDIR  0x4000  // Directory
#define S_IFCHR  0x2000  // Character device
#define S_IFIFO  0x1000  // FIFO

#define S_ISUID  0x0800  // Set UID bit
#define S_ISGID  0x0400  // Set GID bit
#define S_ISVTX  0x0200  // Sticky bit

#define S_IRWXU  0x01C0  // User (file owner) has read, write, and execute permission
#define S_IRUSR  0x0100  // User has read permission
#define S_IWUSR  0x0080  // User has write permission
#define S_IXUSR  0x0040  // User has execute permission

#define S_IRWXG  0x0038  // Group has read, write, and execute permission
#define S_IRGRP  0x0020  // Group has read permission
#define S_IWGRP  0x0010  // Group has write permission
#define S_IXGRP  0x0008  // Group has execute permission

#define S_IRWXO  0x0007  // Others have read, write, and execute permission
#define S_IROTH  0x0004  // Others have read permission
#define S_IWOTH  0x0002  // Others have write permission
#define S_IXOTH  0x0001  // Others have execute permission

// File system node types
typedef struct vfs_node vfs_node_t;
typedef struct vfs_filesystem vfs_filesystem_t;
typedef struct vfs_dirent vfs_dirent_t;
typedef struct vfs_file_handle vfs_file_handle_t;

// File operations structure
typedef struct vfs_file_ops {
    int (*open)(vfs_node_t* node, vfs_file_handle_t* handle);
    int (*close)(vfs_file_handle_t* handle);
    ssize_t (*read)(vfs_file_handle_t* handle, void* buf, size_t count);
    ssize_t (*write)(vfs_file_handle_t* handle, const void* buf, size_t count);
    off_t (*lseek)(vfs_file_handle_t* handle, off_t offset, int whence);
    int (*ioctl)(vfs_file_handle_t* handle, unsigned long request, void* argp);
    int (*getdents)(vfs_file_handle_t* handle, void* dirp, unsigned int count);
    int (*readdir)(vfs_node_t* dir, vfs_dirent_t* dirent, unsigned int index);
    vfs_node_t* (*finddir)(vfs_node_t* dir, const char* name);
    int (*create)(vfs_node_t* dir, const char* name, int mode);
    int (*mkdir)(vfs_node_t* dir, const char* name, int mode);
    int (*unlink)(vfs_node_t* dir, const char* name);
    int (*rmdir)(vfs_node_t* dir, const char* name);
    int (*rename)(vfs_node_t* old_dir, const char* old_name, vfs_node_t* new_dir, const char* new_name);
    int (*stat)(vfs_node_t* node, struct stat* statbuf);
    int (*chmod)(vfs_node_t* node, mode_t mode);
    int (*chown)(vfs_node_t* node, uid_t uid, gid_t gid);
    int (*truncate)(vfs_node_t* node, off_t length);
    int (*sync)(vfs_node_t* node);
} vfs_file_ops_t;

// File system operations structure
typedef struct vfs_fs_ops {
    int (*mount)(vfs_filesystem_t* fs, const char* source, unsigned long flags, const void* data);
    int (*unmount)(vfs_filesystem_t* fs);
    int (*statfs)(vfs_filesystem_t* fs, struct statfs* buf);
    int (*sync)(vfs_filesystem_t* fs);
} vfs_fs_ops_t;

// File system node structure
typedef struct vfs_node {
    char name[256];
    uint32_t inode;
    uint32_t size;
    uint32_t flags;
    uint32_t mode;
    uint32_t uid;
    uint32_t gid;
    uint32_t atime;
    uint32_t mtime;
    uint32_t ctime;
    uint32_t nlink;
    void* fs_private;
    vfs_filesystem_t* fs;
    vfs_file_ops_t* ops;
    vfs_node_t* parent;
    vfs_node_t* next;
    vfs_node_t* child;
} vfs_node_t;

// Directory entry structure
typedef struct vfs_dirent {
    uint32_t inode;
    uint16_t rec_len;
    uint8_t name_len;
    uint8_t file_type;
    char name[256];
} vfs_dirent_t;

// File handle structure
typedef struct vfs_file_handle {
    vfs_node_t* node;
    off_t pos;
    int flags;
    int refcount;
    void* private_data;
} vfs_file_handle_t;

// File system structure
typedef struct vfs_filesystem {
    char name[32];
    vfs_fs_ops_t* ops;
    vfs_node_t* root;
    void* private_data;
    vfs_filesystem_t* next;
} vfs_filesystem_t;

// Initialize the VFS
void vfs_init(void);

// Register a file system
int vfs_register_filesystem(const char* name, vfs_fs_ops_t* ops);

// Mount a file system
int vfs_mount(const char* source, const char* target, const char* fstype, unsigned long flags, const void* data);

// Unmount a file system
int vfs_unmount(const char* target);

// Look up a file or directory
vfs_node_t* vfs_lookup(const char* path);

// Open a file
vfs_file_handle_t* vfs_open(const char* path, int flags, int mode);

// Close a file
int vfs_close(vfs_file_handle_t* handle);

// Read from a file
ssize_t vfs_read(vfs_file_handle_t* handle, void* buf, size_t count);

// Write to a file
ssize_t vfs_write(vfs_file_handle_t* handle, const void* buf, size_t count);

// Seek in a file
off_t vfs_lseek(vfs_file_handle_t* handle, off_t offset, int whence);

// Read directory entries
int vfs_getdents(vfs_file_handle_t* handle, void* dirp, unsigned int count);

// Get file status
int vfs_stat(const char* path, struct stat* statbuf);

// Get file status by file handle
int vfs_fstat(vfs_file_handle_t* handle, struct stat* statbuf);

// Create a directory
int vfs_mkdir(const char* path, int mode);

// Remove a directory
int vfs_rmdir(const char* path);

// Create a file
int vfs_create(const char* path, int mode);

// Delete a file
int vfs_unlink(const char* path);

// Rename a file or directory
int vfs_rename(const char* oldpath, const char* newpath);

// Change file permissions
int vfs_chmod(const char* path, mode_t mode);

// Change file owner
int vfs_chown(const char* path, uid_t uid, gid_t gid);

// Truncate a file
int vfs_truncate(const char* path, off_t length);

// File system synchronization
int vfs_sync(void);

// Get the current working directory
const char* vfs_getcwd(char* buf, size_t size);

// Change the current working directory
int vfs_chdir(const char* path);

// Convert a path to an absolute path
char* vfs_abspath(const char* path, char* abs_path, size_t size);

// Get the root file system node
vfs_node_t* vfs_get_root(void);

// Get the current working directory node
vfs_node_t* vfs_get_cwd(void);

#endif // KERNEL_VFS_H
// FR MY HAND IS BLEEDING !!!