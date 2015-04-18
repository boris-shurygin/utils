/**
 * @file: list.h 
 * List-related functionality implementation
 * @defgroup List Two-way linked list
 *
 * Implementation of two-way linked list
 * @ingroup Utils
 */
/*
 * Utils library in compiler prototype project
 * Copyright (C) 2012  Boris Shurygin
 */
#ifndef LIST_H
#define LIST_H

#include <iterator>

namespace Utils
{

/**
 * @brief Types of direction in lists.
 * @ingroup List  
 *
 * Lists are built from left to right by default.
 * That means if you take next in default direction - it will be element to the right.
 */
enum ListDir
{
    /** Right direction */
    LIST_DIR_RIGHT = 0,
    /** Default direction */
    LIST_DIR_DEFAULT = LIST_DIR_RIGHT,
    /** Left Direction */
    LIST_DIR_LEFT = 1,
    /** Direction reverse to default */
    LIST_DIR_RDEFAULT = LIST_DIR_LEFT,
    /** Number of directions */
    LIST_DIR_NUM = 2
};

/**
 * Return direction that is reverse to given one
 * @ingroup List
 */
inline ListDir
ListRDir( ListDir dir)
{
    UTILS_ASSERTD( LIST_DIR_NUM == 2);
    return ( dir == LIST_DIR_DEFAULT)? LIST_DIR_RDEFAULT: LIST_DIR_DEFAULT; 
}

class ListDefaultTag {};

/**
 * @brief Tagged list item template. User class (e.g. class A) should
 *        inherit from ListItem (e.g. class A: public ListItem< A, AListClassTag>{})
 * @ingroup List
 */
template <typename tag = ListDefaultTag> class ListItem
{
    ListItem * peer[ LIST_DIR_NUM];
    
    /** Get neighbour */
    inline ListItem * peerInDir( ListDir dir) const
    {
        return peer[ dir];
    }
    
    /** Set neighbour */
    inline void setPeerInDir( ListItem *p, ListDir dir)
    {
        peer[ dir] = p;
    }
    /** Set all pointeers to peeers to zero */
    inline void zeroLinks()
    {
        setPeerInDir( NULL, LIST_DIR_DEFAULT);
        setPeerInDir( NULL, LIST_DIR_RDEFAULT);
    }

    /** Set next peer */
    inline void setNext( ListItem *n)
    {
        setPeerInDir( n, LIST_DIR_DEFAULT);
    }
    /** Set previous peer */
    inline void setPrev( ListItem *p)
    {
        setPeerInDir( p, LIST_DIR_RDEFAULT);
    }
public:
        
    /* Default peers gets */
    /** Return next peer in default direction */
    inline ListItem *next() const
    {
        return peerInDir( LIST_DIR_DEFAULT);
    }
    /** Return prev peer in default direction */
    inline ListItem *prev() const
    {
        return peerInDir( LIST_DIR_RDEFAULT);
    }
       
    /** Attach this item to peeer in give direction */
    inline void attachInDir(ListItem *p, ListDir dir)
    {
        ListDir rdir = ListRDir( dir);
        setPeerInDir( p, dir);
        setPeerInDir( NULL, rdir);

        /* Type conversion here are needed for disambiguation */
        if ( isNotNullP( p))
        {
            ListItem * rdir_peer =  ((ListItem* )p)->peerInDir( rdir);
            if ( isNotNullP( rdir_peer))
            {
                ((ListItem*)rdir_peer)->setPeerInDir( static_cast< ListItem *>(this), dir);
            }
            ( (ListItem*)p)->setPeerInDir( static_cast< ListItem *>(this), rdir);
            setPeerInDir( rdir_peer, rdir);
        }
    }
    
    /** Attach in default direction */
    inline void attach( ListItem * peer)
    {
        attachInDir( peer, LIST_DIR_DEFAULT);
    }

    /** Detach from neighbours */
    inline void detach()
    {
        /* Correct links in peers */
        if ( isNotNullP( peer[ LIST_DIR_DEFAULT]))
        {
            ((ListItem *)peer[ LIST_DIR_DEFAULT])
                ->setPeerInDir( peer[ LIST_DIR_RDEFAULT], LIST_DIR_RDEFAULT);
        }
        if ( isNotNullP( peer[ LIST_DIR_RDEFAULT]))
        {
            ((ListItem *)peer[ LIST_DIR_RDEFAULT])
                ->setPeerInDir( peer[ LIST_DIR_DEFAULT], LIST_DIR_DEFAULT);
        }
        setPeerInDir( NULL, LIST_DIR_DEFAULT);
        setPeerInDir( NULL, LIST_DIR_RDEFAULT);
    }

    /** Default constructor */
    ListItem()
    {
        zeroLinks();
    };

    /** Insert element before the given one */
    ListItem( ListItem *peer)
    {
        zeroLinks();
        attachInDir( peer, LIST_DIR_DEFAULT);
    }

    /** Insert element in given direction */
    ListItem( ListItem *peer, ListDir dir)
    {
        zeroLinks();
        attachInDir( peer, dir);
    }

    /** Destructor */
    virtual ~ListItem()
    {
        detach();
    }
};

/**
 * @brief Tagged intrusive list template. 
 *
 * @details
 * Intrusive list uses prev and next pointers built into the client's class. 
 * To add these pointers user class should inherit from ListItem. 
 * 
 * The tag option is used to enable user object's participation in multiple list. 
 * To do this the user class should inherit from ListItem multiple times, but with
 * different tag parameter
 *
 * Example:
 * @code

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

void foo()
{
    ...
    B item(NULL);

    List< B, BListTag> list;

    list.push_front(item);

    for (auto i = list.begin(), end = list.end(); i != end; ++i)
    {
        i->i = 10;
    }
    ...
}

 @endcode
 *
 * @ingroup List
 */
template <class Data, typename Tag = ListDefaultTag> class List
{
public:
    typedef ListItem< Tag> ItemType;
    
    /* General typedefs */
    typedef Data value_type;
    typedef Data *pointer;
    typedef Data& reference;

    /**
     * List iterator class
     */
    class iterator : public std::iterator < bidirectional_iterator_tag, value_type >
    {
    public:
        iterator(const iterator& it) :p(it.p){ };
        ~iterator() {}; // Does nothing

        iterator& operator=(const iterator&)
        {
            if (&it != this)
                return *this;
        };

        // Increments
        iterator& operator++()   { p = p->ItemType::next(); return *this; }                //< prefix increment
        iterator operator++(int) { auto tmp(*this); p = p->ItemType::next(); return tmp; } //< postfix increment
        
        // Decrements
        iterator& operator--()   { p = p->ItemType::prev(); return *this; }                //< prefix increment
        iterator operator--(int) { auto tmp(*this); p = p->ItemType::prev(); return tmp; } //< postfix increment

        // Data access
        reference operator*()    { return static_cast<reference>(*p); } //< Data type should be inherited from ListItem
        pointer operator->()     { return static_cast<pointer>(p);    } //< Data type should be inherited from ListItem
        
        //pointer operator->() const   { return p; }
        //value_type operator*() const { return *p; }
        
        // Operators are defined here for simplicity
        // Otherwise a lot of code is needed to predeclare templated operators so they can be used as friends
        
        /** Equality check */
        friend bool operator==(const iterator& i1, const iterator& i2)
        {
            return i1.p == i2.p;
        }
        /** Ineqaulity check */
        friend bool operator!=(const iterator& i1, const iterator& i2)
        {
            return i1.p != i2.p;
        }

        friend void swap(iterator& lhs, iterator& rhs); //Not implemented; used in C++11?
        friend class List < Data, Tag >;
        
    private:
        iterator(pointer p_) : p(p_){ };

        ItemType* p;
    };

public:
    /** List public interface */
    
    /** 
     * Put item in list
     * NOTE: argument is not const!
     *       as the list implementation is intrusive the 
     *       push_front routine modifies the stored objects
     */
    void push_front(value_type& val) 
    {
        val.ItemType::attach(first);
        first = &val;
    }

    /**
     * Get a reference to the first element in list
     */
    reference front()
    {
        return *first;
    }

    iterator begin() { return iterator(first); }
    iterator end()   { return iterator(nullptr); }
private:
    pointer first;
};

} // namespace Utils

#endif
