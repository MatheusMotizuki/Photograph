#pragma once

struct Link {
    int id;
    int init_attr;
    int end_attr;

    inline static int link_next_id = 1;
};