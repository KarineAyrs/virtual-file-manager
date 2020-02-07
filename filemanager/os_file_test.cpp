#include <stdio.h>
#include "os_file.h"

//************************************************
int my_create(int disk_size)
{
    return 1;
}

//************************************************
int my_destroy()
{
    return 1;
}

void my_get_cur_dir(char* dst) {
    strcpy(dst, "test");
}

void setup_file_manager(file_manager_t *fm) {
    fm->create = my_create;
    fm->destroy = my_destroy;
    fm->get_cur_dir = my_get_cur_dir;
}

//************************************************
//************************************************
int main()
{
    file_manager_t fm;

    setup_file_manager(&fm);
    int result = fm.create(10);
    result = fm.destroy();
    return 0;
}
