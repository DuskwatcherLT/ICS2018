#include "common.h"
#include "syscall.h"

extern char _end;

typedef size_t ssize_t;
typedef size_t off_t;
int fs_open(const char* pathname, int flags, int mode);
ssize_t fs_read(int fd, void* buf, size_t len);
ssize_t fs_close(int fd);
ssize_t fs_lseek(int fd, off_t offset, int whence);

extern _Context* do_syscall(_Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;	//type 
	a[1] = c->GPR2;	//parameter1:status
	a[2] = c->GPR3;
	a[3] = c->GPR4;

  switch (a[0]) {
		case SYS_yield: _yield();
									 	c->GPRx = 0; 
										//Log("call sys_yield!");
										break;
		case SYS_exit:  Log("call sys_exit!");
										_halt(a[2]);
									  break;
		case SYS_write: //Log("call sys_write!");
										//int fd = (int)a[1];
										//char* buf = (char*)a[2];
										//size_t count = (size_t)a[3];
										if(a[1] == 1 || a[1] == 2){
											char* buf = (char*)a[2];
											for(int i = 0; i < a[3]; ++i) _putc(buf[i]);
										}
										c->GPRx = (size_t)a[3];
										break;
		case SYS_brk:		Log("call sys_brk!");
										_heap.start = &_end;
										_heap.end = (void*)a[1];
										c->GPRx = 0;
										break;
		case SYS_read: 	Log("call sys_read!");
									 	c->GPRx = fs_read(a[1], (void*)a[2], a[3]);
										 break;
		case SYS_open: 	Log("call sys_open!");
									 	c->GPRx = fs_open((void*)a[1], a[2], a[3]);
									 	break;
		case SYS_close: Log("call sys_close!");
										c->GPRx = fs_close(a[1]);
									  break;
		case SYS_lseek: Log("call sys_lseek!");
										c->GPRx = fs_lseek(a[1], a[2], a[3]);
										break;	
    default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return c;
}
