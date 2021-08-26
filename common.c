#include "bplustree.h"

/*
判断是否为叶子节点
*/
int is_leaf(struct bplus_node *node) {
    return node->type == BPLUS_TREE_LEAF;
}

/*
字符串转16进制
*/
off_t str_to_hex(char *c, int len) {
    off_t offset = 0;
    while (len-- > 0) {
        if (isdigit(*c)) {
            offset = offset * 16 + *c - '0';
        } else if (isxdigit(*c)) {
            if (islower(*c)) {
                offset = offset * 16 + *c - 'a' + 10;
            } else {
                offset = offset * 16 + *c - 'A' + 10;
            }
        }
        c++;
    }
    return offset;
}

/*
16进制转字符串
*/
void hex_to_str(off_t offset, char *buf, int len) {
    const static char *hex = "0123456789ABCDEF";
    while (len-- > 0) {
        buf[len] = hex[offset & 0xf];
        offset >>= 4;
    }
}

/*
加载文件数据，每16位记录一个信息
*/
//where used
off_t offset_load(char *t_ptr, off_t *offset) {
    char buf[ADDR_STR_WIDTH];
    char *p = memcpy(buf, t_ptr + *offset, sizeof(buf));
    *offset = *offset + ADDR_STR_WIDTH;
    return p != NULL ? str_to_hex(buf, sizeof(buf)) : INVALID_OFFSET;
}

/*
存储B+相关数据
*/
//where used
void offset_store(char *t_ptr, off_t offset) {
    char buf[ADDR_STR_WIDTH];
    hex_to_str(offset, buf, sizeof(buf));
    //return write(fd, buf, sizeof(buf));
    //todo
    memcpy(t_ptr + offset, buf, sizeof(buf));
}

/*
获取B+树文件大小
*/

off_t get_tree_size(char *tree_boot_addr) {
    off_t offset = 5 * ADDR_STR_WIDTH;
    offset = offset_load(tree_boot_addr, &offset);
    return offset;
}

off_t get_boot_size(char *tree_boot_addr) {
    off_t offset = 0;
    offset = offset_load(tree_boot_addr, &offset);
    return offset;
}


int get_file_size(char *filename) {
    struct stat temp;
    stat(filename, &temp);
    return temp.st_size;
}


char *mmap_btree_file(char *file_name) {

    int fd, file_size;
    char *p_map;

    file_size = get_file_size(file_name);
    fd = open(file_name, O_RDWR, 0644);

    p_map = (char *) mmap(NULL, file_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    close(fd);
    return p_map;

};

void munmap_btree_file(char *m_ptr, off_t file_size) {
    munmap(m_ptr, file_size);
}