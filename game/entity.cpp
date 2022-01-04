#include "entity.h"
#include <iostream>


int EntityClass::from_string (std::string str) {
    if (str == "inf") return EntityInfo::Class::inf;
    if (str == "cav") return EntityInfo::Class::cav;
    if (str == "ran") return EntityInfo::Class::ran;
    if (str == "sie") return EntityInfo::Class::sie;
    if (str == "bld") return EntityInfo::Class::bld;
}