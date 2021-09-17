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

void delay(unsigned int i) {
    while (i) {
        i--;
    }
}

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
    delay(1000);
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

void display_1602_block_str() {
    char block[] = {"  "};
    char *blockPointer = block;
    while (*blockPointer) {
        write_dat(*blockPointer++);
    }
}

void display_1602_str(unsigned char row, unsigned char column, char str[]) {
    unsigned char address;
    // 计算起始地址
    address = (row - 1) * 0x40 + (column - 1);
    // 设置起始地址
    write_cmd(0x80 + address);
    char *copyStr = str;
    int len = strlen(str);
    if (len < 16) {
        while (*copyStr) {
            write_dat(*copyStr++);
        }
    } else {
        int n = 0, i = 0;
        while (*copyStr) {
            n++;
            write_dat(*copyStr++);
            if (n % 17 == 0) {
                n = 0;
                i++;
                if (i < len) {
                    copyStr = &str[i];
                    write_cmd(0x80 + address);
                } else {
                    i = 0;
                }
            }
            if (!*copyStr) {
                display_1602_block_str();
                copyStr = &str[0];
            }
            delay(3800);
        }
    }

}