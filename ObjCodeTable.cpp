//
// Created by Ean McGilvery on 12/6/20.
//

#pragma once
#include <string>

struct ObjCodeTable
{
    int address;
    int operand;
    std::string op;

    ObjCodeTable() : address(0), operand(0), op("") {}
};
