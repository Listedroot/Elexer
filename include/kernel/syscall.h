#ifndef KERNEL_SYSCALL_H
#define KERNEL_SYSCALL_H

#include <stdint.h>
#include <stddef.h>

// System call numbers
enum {
    SYS_EXIT = 0,
    SYS_READ,
    SYS_WRITE,
    SYS_OPEN,
    SYS_CLOSE,
    SYS_FORK,
    SYS_EXECVE,
    SYS_WAITPID,
    SYS_GETPID,
    SYS_GETPPID,
    SYS_SBRK,
    SYS_MMAP,
    SYS_MUNMAP,
    SYS_IOCTL,
    SYS_ACCESS,
    SYS_KILL,
    SYS_GETCWD,
    SYS_CHDIR,
    SYS_GETDENTS,
    SYS_STAT,
    SYS_FSTAT,
    SYS_LSEEK,
    SYS_DUP,
    SYS_DUP2,
    SYS_PIPE,
    SYS_SLEEP,
    SYS_GETTID,
    SYS_YIELD,
    SYS_SCHED_YIELD = SYS_YIELD,
    SYS_GETPRIORITY,
    SYS_SETPRIORITY,
    SYS_SYNC,
    SYS_REBOOT,
    SYS_MOUNT,
    SYS_UMOUNT,
    SYS_IOCTL_TTY,
    SYS_FCNTL,
    SYS_READDIR,
    SYS_GETTIMEOFDAY,
    SYS_SETTIMEOFDAY,
    SYS_UTIMES,
    SYS_NANOSLEEP,
    SYS_CLOCK_GETTIME,
    SYS_CLOCK_SETTIME,
    SYS_CLOCK_GETRES,
    SYS_GETUID,
    SYS_GETEUID,
    SYS_GETGID,
    SYS_GETEGID,
    SYS_SETUID,
    SYS_SETEUID,
    SYS_SETGID,
    SYS_SETEGID,
    SYS_GETGROUPS,
    SYS_SETGROUPS,
    SYS_UNAME,
    SYS_SETHOSTNAME,
    SYS_GETHOSTNAME,
    SYS_GETPAGESIZE,
    SYS_MLOCK,
    SYS_MUNLOCK,
    SYS_MLOCKALL,
    SYS_MUNLOCKALL,
    SYS_TRUNCATE,
    SYS_FTRUNCATE,
    SYS_SYMLINK,
    SYS_READLINK,
    SYS_CHMOD,
    SYS_FCHMOD,
    SYS_CHOWN,
    SYS_FCHOWN,
    SYS_LCHOWN,
    SYS_UMASK,
    SYS_GETRLIMIT,
    SYS_SETRLIMIT,
    SYS_GETRUSAGE,
    SYS_TIMES,
    SYS_PTRACE,
    SYS_GETTIMEOFDAY_SEC,
    SYS_GETTIMEOFDAY_USEC,
    SYS_GETTID_ASM,
    SYS_TGKILL,
    SYS_TKILL,
    SYS_TGKILL_ASM,
    SYS_TKILL_ASM,
    SYS_MAX
};

// System call handler type
typedef int32_t (*syscall_handler_t)(uint32_t, uint32_t, uint32_t, uint32_t, uint32_t);

// System call parameter structure
typedef struct {
    uint32_t eax;  // System call number
    uint32_t ebx;  // First argument
    uint32_t ecx;  // Second argument
    uint32_t edx;  // Third argument
    uint32_t esi;  // Fourth argument
    uint32_t edi;  // Fifth argument
    uint32_t ebp;  // Sixth argument
} syscall_params_t;

// Initialize the system call interface
void syscall_init(void);

// Register a system call handler
void syscall_register(uint32_t num, syscall_handler_t handler);

// System call handler (called from assembly)
void syscall_handler(syscall_params_t* params);

// System call stubs (implemented in assembly)
void syscall0(uint32_t num);
uint32_t syscall1(uint32_t num, uint32_t arg1);
uint32_t syscall2(uint32_t num, uint32_t arg1, uint32_t arg2);
uint32_t syscall3(uint32_t num, uint32_t arg1, uint32_t arg2, uint32_t arg3);
uint32_t syscall4(uint32_t num, uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4);
uint32_t syscall5(uint32_t num, uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4, uint32_t arg5);

// Helper macros for system calls
#define SYSCALL0(num) syscall0((num))
#define SYSCALL1(num, arg1) syscall1((num), (uint32_t)(arg1))
#define SYSCALL2(num, arg1, arg2) syscall2((num), (uint32_t)(arg1), (uint32_t)(arg2))
#define SYSCALL3(num, arg1, arg2, arg3) syscall3((num), (uint32_t)(arg1), (uint32_t)(arg2), (uint32_t)(arg3))
#define SYSCALL4(num, arg1, arg2, arg3, arg4) syscall4((num), (uint32_t)(arg1), (uint32_t)(arg2), (uint32_t)(arg3), (uint32_t)(arg4))
#define SYSCALL5(num, arg1, arg2, arg3, arg4, arg5) syscall5((num), (uint32_t)(arg1), (uint32_t)(arg2), (uint32_t)(arg3), (uint32_t)(arg4), (uint32_t)(arg5))

// System call implementations
int32_t sys_exit(int status);
int32_t sys_read(int fd, void* buf, size_t count);
int32_t sys_write(int fd, const void* buf, size_t count);
int32_t sys_open(const char* pathname, int flags, int mode);
int32_t sys_close(int fd);
int32_t sys_fork(void);
int32_t sys_execve(const char* pathname, char* const argv[], char* const envp[]);
int32_t sys_waitpid(int pid, int* status, int options);
int32_t sys_getpid(void);
int32_t sys_getppid(void);
void* sys_sbrk(intptr_t increment);
void* sys_mmap(void* addr, size_t length, int prot, int flags, int fd, off_t offset);
int32_t sys_munmap(void* addr, size_t length);
int32_t sys_ioctl(int fd, unsigned long request, void* argp);
int32_t sys_access(const char* pathname, int mode);
int32_t sys_kill(int pid, int sig);
int32_t sys_getcwd(char* buf, size_t size);
int32_t sys_chdir(const char* path);
int32_t sys_getdents(unsigned int fd, struct dirent* dirp, unsigned int count);
int32_t sys_stat(const char* path, struct stat* statbuf);
int32_t sys_fstat(int fd, struct stat* statbuf);
off_t sys_lseek(int fd, off_t offset, int whence);
int32_t sys_dup(int oldfd);
int32_t sys_dup2(int oldfd, int newfd);
int32_t sys_pipe(int pipefd[2]);
unsigned int sys_sleep(unsigned int seconds);
int32_t sys_gettid(void);
void sys_yield(void);
int32_t sys_getpriority(int which, int who);
int32_t sys_setpriority(int which, int who, int prio);
void sys_sync(void);
int32_t sys_reboot(int magic1, int magic2, unsigned int cmd, void* arg);
int32_t sys_mount(const char* source, const char* target, const char* filesystemtype, unsigned long mountflags, const void* data);
int32_t sys_umount(const char* target);
int32_t sys_fcntl(int fd, int cmd, ...);

#endif // KERNEL_SYSCALL_H
