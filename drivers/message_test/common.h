#ifndef COMMON_H_
# define COMMON_H_

#define FLAG_BLOCKING		(1)
#define FLAG_NONBLOCKING	(0)

t_uint32		syscall_async_send(t_uint32* node, t_uint32 tag, void* ptr, t_uint32 size);
t_uint32		syscall_sync_send(t_uint32* node, t_uint32 tag, void* ptr, t_uint32 size);
t_uint32		syscall_async_recv(t_uint32 tag, void* ptr, t_uint32 size);
t_uint32		syscall_sync_recv(t_uint32 tag, void* ptr, t_uint32 size, t_uint32 flags);

#endif /* !COMMON_H_ */

