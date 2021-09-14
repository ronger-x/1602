//
// Created by ronger on 2021/9/8.
//
#include "stdbool.h"
#include "compiler.h"
#include "STC89xx.h"

#define _P3 0xB0
SBIT(RS_1602, _P3, 6);
SBIT(RW_1602, _P3, 5);
SBIT(E_1602, _P3, 4);
#define MAX_ROW 2
#define MAX_COLUMN 16

unsigned char read_state() {
    unsigned char state;
    RS_1602 = 0;
    // 进入读 1602 状态模式
    RW_1602 = 1;
    // 拉高使能信号
    E_1602 = 1;
    // 将 1602 状态数据拉取
    state = P2;
    // 拉低使能，完成读操作
    E_1602 = 0;
    // 将状态值返回
    return state;
}

void ready() {
    P2 = 0xFF;
    while (read_state() & 0x80);
}

void write_cmd(unsigned char cmd) {
    ready();
    RS_1602 = 0;
    RW_1602 = 0;

    P2 = cmd;

    E_1602 = 1;
    E_1602 = 0;
}

void write_dat(unsigned char dat) {
    ready();
    RS_1602 = 1;
    RW_1602 = 0;

    P2 = dat;

    E_1602 = 1;
    E_1602 = 0;
}

void init_1602() {
    write_cmd(0x38);
    write_cmd(0x0C);
    write_cmd(0x06);
    write_cmd(0x01);
}

int strlen(char *str) {
    int n;
    for (n = 0; *str != '\0'; str++) {
        n++;
    }
    return n;
}

void delay() {
    unsigned int i = 10000;
    while (i) {
        i--;
    }
}

void display_1602_str(unsigned char row, unsigned char column, char str[]) {
    unsigned char address;
    // 计算起始地址
    address = (row - 1) * 0x40 + (column - 1);
    // 设置起始地址
    write_cmd(0x80 + address);

//    int i;
//    i = 0;
//    int n;
//    n = 0;
    char *copyStr = str;
//    int len = strlen(copyStr);
//    if (len > MAX_COLUMN) {
//        while (*copyStr) {
//            write_dat(*copyStr++);
//            ++i;
//            if (i > MAX_COLUMN) {
//                delay();
//                ++n;
//                i = 0;
//                if (n < len) {
//                    copyStr = str;
//                    *copyStr += n;
//                } else {
//                    copyStr = str;
//                }
//            }
//        }
//    } else {
//    }
    while (*copyStr) {
        write_dat(*copyStr++);
    }

}