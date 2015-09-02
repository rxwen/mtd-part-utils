#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/limits.h>
#include <string.h>
#include <stdbool.h>

#define PREFIX_SYS_MTD          "/sys/class/mtd/mtd"
#define PARTITION_NAME_MAX_LEN    64

void usage() {
    printf("%s\n", "mtd-part-offset PARTITION_LABEL");
    exit(0);
}

bool partition_label_match(int part_index, const char* label) {
    char name_path[PATH_MAX] = {0};
    char name_buff[PARTITION_NAME_MAX_LEN] = {0};
    size_t len = 0;
    FILE* name_file = NULL;
    snprintf(name_path, PATH_MAX, "%s%d/name", PREFIX_SYS_MTD, part_index);
    name_file = fopen(name_path, "r");
    fread(name_buff, PARTITION_NAME_MAX_LEN, 1, name_file);
    
    return 0 == strncmp(name_buff, label, strlen(label));
}

long long int partition_size(int part_index) {
    char size_path[PATH_MAX] = {0};
    char size_buff[PARTITION_NAME_MAX_LEN] = {0};
    size_t len = 0;
    FILE* size_file = NULL;
    snprintf(size_path, PATH_MAX, "%s%d/size", PREFIX_SYS_MTD, part_index);
    size_file = fopen(size_path, "r");
    fread(size_buff, PARTITION_NAME_MAX_LEN, 1, size_file);
    
    return strtoll(size_buff, NULL, 10);
}

long long int get_offset_of_partition(const char* label) {
    int part_index = 0, i = 0;
    long long int offset = 0;
    for(i = 0; i < 10 ; ++i) {
        if(partition_label_match(i, label)) {
            break;
        }
        offset += partition_size(i);
    }

    return offset;
}

int main(int argc, const char *argv[]) {
    if(argc != 2) {
        usage();
    }

    printf("0x%llx\n", get_offset_of_partition(argv[1]));

    return 0;
}
