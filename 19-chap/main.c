#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    int fd = inotify_init();
    int wd = inotify_add_watch(fd, "/home/pi/Source/19-chap", IN_ALL_EVENTS);
    
    uint8_t buf[sizeof(struct inotify_event) + NAME_MAX + 1];
    while(1)
    {
        int read_cnt = read(fd, buf, sizeof(buf));
        printf("Number of Bytes read: %d\n", read_cnt);
        
        if (read_cnt <= 0)
        {
            printf("read() failure\n");
            return EXIT_FAILURE;
        }
        
        uint16_t buf_idx = 0;
        while (buf_idx < read_cnt)
        {
            struct inotify_event* event = (struct inotify_event*)&buf[buf_idx];
            
            // Print the event information.
            if (event->mask & IN_ACCESS)        printf("IN_ACCESS\n");
            if (event->mask & IN_ATTRIB)        printf("IN_ATTRIB\n");
            if (event->mask & IN_CLOSE_NOWRITE) printf("IN_CLOSE_NOWRITE\n");
            if (event->mask & IN_CLOSE_WRITE)   printf("IN_CLOSE_WRITE\n");
            if (event->mask & IN_CREATE)        printf("IN_CREATE\n");
            if (event->mask & IN_DELETE)        printf("IN_DELETE\n");
            if (event->mask & IN_DELETE_SELF)   printf("IN_DELETE_SELF\n");
            if (event->mask & IN_IGNORED)       printf("IN_IGNORED\n");
            if (event->mask & IN_ISDIR)         printf("IN_ISDIR\n");
            if (event->mask & IN_MODIFY)        printf("IN_MODIFY\n");
            if (event->mask & IN_MOVE_SELF)     printf("IN_MOVE_SELF\n");
            if (event->mask & IN_MOVED_FROM)    printf("IN_MOVED_FROM\n");
            if (event->mask & IN_MOVED_TO)      printf("IN_MOVED_TO\n");
            if (event->mask & IN_OPEN)          printf("IN_OPEN\n");
            if (event->mask & IN_Q_OVERFLOW)    printf("IN_Q_OVERFLOW\n");
            if (event->mask & IN_UNMOUNT)       printf("IN_UNMOUNT\n");
            
            buf_idx += sizeof(struct inotify_event) + event->len;
        }
    }
}
