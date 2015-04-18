/**
 * @file: list_utest.cpp 
 * Implementation of testing for lists 
 */
/*
 * Copyright (C) 2012  Boris Shurygin
 */
#include "utils_iface.h"

using namespace Utils;

static const int LIST_TEST_NUM_NODES = 1000;

class BListTag{}; // Tag for the second list

// List item, note that ListItem used with default tag
class A: public ListItem<>
{
    int a;
};

// Participates in multiple list
class B: public A, public ListItem< BListTag>
{
    typedef ListItem< BListTag> Item;
public:
    int i;

    B( B* ptr): Item( ptr){};

    // Using the next/prev pointers directly
    B *nextB(){ return static_cast< B*>( Item::next() );}
    B *prevB(){ return static_cast< B*>( Item::prev() );}
};

/**
 * Test tagged list
 */
static bool 
uTestTagList(UnitTest *utest_p)
{
    B *list = NULL;
    B *list_temp = NULL;

    /* Create */
    for (int i = 0; i < LIST_TEST_NUM_NODES; i++)
    {
        list = new B(list);
    }
    /* Iterate and access data */
    list_temp = list;
    while (isNotNullP(list_temp))
    {
        list_temp->i = 10;
        list_temp = list_temp->nextB();
    }
    /* Iterate and access data */
    list_temp = list;
    while (isNotNullP(list_temp))
    {
        B *next = list_temp->nextB();
        delete list_temp;
        list_temp = next;
    }

    B item(NULL);

    List< B, BListTag> main_list;

    main_list.push_front(item);

    for (auto i = main_list.begin(), end = main_list.end(); i != end; ++i)
    {
        i->i = 10; // set test value
    }
    
    UTEST_CHECK(utest_p, item.i == 10); // check test value
    
    // check front()
    UTEST_CHECK(utest_p, main_list.front().i == 10); 
    UTEST_CHECK(utest_p, &(main_list.front()) == &item);

    return utest_p->result();
}

/**
 * Test list classes operation
 */
bool Utils::uTestList( UnitTest *utest_p)
{
    uTestTagList( utest_p);

    return utest_p->result();
}

