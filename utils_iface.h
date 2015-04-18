/**
 * @file: utils_iface.h 
 * Interface of Utils library in compiler prototype project
 * @defgroup Utils Utils
 * @ingroup Core
 * @brief Low level utils
 */
/*
 * Utils library in SCL
 * Copyright (C) 2012  Boris Shurygin
 */
#pragma once

#ifndef UTILS_IFACE_H
#define UTILS_IFACE_H

/**
 * Debug assert with diagnostic info for Graph library
 * @ingroup GraphBase
 */
#if !defined(UTILS_ASSERTXD)
#    define UTILS_ASSERTXD(cond, what) ASSERT_XD(cond, "Utils", what)
#endif

/**
 * Simple debug assert for Graph library
 * @ingroup GraphBase
 */
#if !defined(UTILS_ASSERTD)
#    define UTILS_ASSERTD(cond) ASSERT_XD(cond, "Utils", "")
#endif

#include "host.h"

#include "types.h"
#include "misc.h"
#include "asrt.h"
#include "timer.h"
#include "print.h"
#include "utest.h"
//#include "techniques.h"

#include "list.h"
#include "singleton.h"
#include "log.h"
#include "mem.h"
#include "conf.h"


namespace Utils
{
    using namespace Print;
    using namespace Mem;

    /**
     * Test list classes
     */
    bool uTestList( UnitTest *utest_p);

    /**
     * Test memory manager
     */
    bool uTestMem( UnitTest *utest_p);

    /**
     * Test pool allocators with list
     */
    bool uTestMemPoolListAlloc( UnitTest *utest_p);

    /**
     * Test std allocators to get reference time
     */
    bool uTestStdListAlloc( UnitTest *utest_p);

    /**
     * Test pool allocator with map
     */
    bool uTestMemPoolMapAlloc( UnitTest *utest_p);
    
    /**
     * Test Utils package
     */
    bool uTest();
};

static const UInt32 FUNC_NAME_WIDTH = 30;

#endif
