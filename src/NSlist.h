#ifndef NSLIST_H
#define NSLIST_H

#include "Error.h"

template<class T>
struct NSlistNode {
    NSlistNode(T* o, NSlistNode<T>* p, NSlistNode<T>* n, int i)
    {
        object = o;
        next = n;
        previous = p;
        id = i;
    }
    ~NSlistNode() {}
    T* object;
    int id;
    NSlistNode<T>* next;
    NSlistNode<T>* previous;
    T* remove();
};

template<class T>
class NSlist {
    public:
        int size = 0;
        int lastID = 0;
        NSlistNode<T>* front;
        NSlistNode<T>* back;
        NSlist()
        {
            front = new NSlistNode<T>(nullptr,nullptr,nullptr,0);
            back = new NSlistNode<T>(nullptr,front,nullptr,0);
            front->next = back;
        }
        NSlistNode<T>* get_node(int i)
        {
            if (i >= size || i < 0) { NSEngine::error("Index out of bounds for indice " + std::to_string(i)); return nullptr; }

            NSlistNode<T>* n = front->next;
            for (int j = 0; j < i; j++)
            {
                if (n->next == nullptr) return n;
                n = n->next;
            }
            return n;
        }
        NSlistNode<T>* find_node(int id)
        {
            NSlistNode<T>* n = front->next;
            while (n != nullptr)
            {
                if (n->id == id && n != back) return n;
                n = n->next;
            }
            NSEngine::warning("undefined ID " + std::to_string(id));
            return nullptr;
        }

        T* get(int i) { return get_node(i)->object; }
        T* find(int id) { return find_node(id)->object; }
        int insert_before(T* object, int i)
        {
            NSlistNode<T>* n1 = get_node(i);
            NSlistNode<T>* n0 = get_node(i-1);
            NSlistNode<T>* n = new NSlistNode<T>(object,n0,n1,lastID);
            n0->next = n;
            n1->previous = n;
            size++;
            return lastID++;
        }
        int insert_after(T* object, int i)
        {
            NSlistNode<T>* n1 = get_node(i+1);
            NSlistNode<T>* n0 = get_node(i);
            NSlistNode<T>* n = new NSlistNode<T>(object,n0,n1,lastID);
            n0->next = n;
            n1->previous = n;
            size++;
            return lastID++;
        }
        int insert_front(T* object)
        {
            NSlistNode<T>* n1 = front->next;
            NSlistNode<T>* n = new NSlistNode<T>(object,front,n1,lastID);
            front->next = n;
            n1->previous = n;
            size++;
            return lastID++;
        }
        int insert_back(T* object)
        {
            NSlistNode<T>* n1 = back->previous;
            NSlistNode<T>* n = new NSlistNode<T>(object,n1,back,lastID);
            n1->next = n;
            back->previous = n;
            size++;
            return lastID++;
        }
        bool empty()
        {
            return front->next == back;
        }
        void clear()
        {
            for (NSlistNode<T>* n = front; n != back; )
            {
                n = n->next;
                delete n->previous;
            }
            front->next = back;
            back->previous = front;
        }
        T* remove(int i)
        {
            NSlistNode<T>* n = get_node(i);
            size--;
            n->previous->next = n->next;
            n->next->previous = n->previous;
            return n->object;
            delete n;
        }
        T* removeId(int id)
        {
            NSlistNode<T>* n = find_node(id);
            if (n==nullptr) return nullptr;
            n->previous->next = n->next;
            n->next->previous = n->previous;
            size--;
            return n->object;
            delete n;
        }
        void del(int i) 
        {
            NSlistNode<T>* n = get_node(i);
            size--;
            if (n == nullptr) return;
            n->previous->next = n->next;
            n->next->previous = n->previous;
            delete n->object;
            delete n;
        }
        void delId(int id) { delete removeId(id); }
        T* operator[](int i) { return get(i); }
        
        struct iterator;
        iterator begin() { return iterator(front->next); }
        iterator end() { return iterator(back); }
        struct iterator {
            iterator(NSlistNode<T>* n) { node = n; }
            NSlistNode<T>* node;
            operator T**() const { return &(node->object); }
            iterator& operator++() { if (node->next != nullptr) node = node->next; return *this; }
            iterator operator++(int) { iterator i = *this; ++*this; return i; }
            bool operator!=(const iterator& i) { return i.node != node; }
        };


};

class ListUtil {
    public:
        template<typename DoubleListNode_t, typename ValueType_t>
        static DoubleListNode_t* listInsertAfter(DoubleListNode_t* listNode, ValueType_t* val)
        {
            DoubleListNode_t* newnode = new DoubleListNode_t();
            if (listNode->next) listNode->next->previous = newnode;
            newnode->next = listNode->next;
            listNode->next = newnode;
            newnode->previous = listNode;
            newnode->value = val;
            return newnode;
        }

        template<typename DoubleListNode_t>
        static DoubleListNode_t* listInsertAfter(DoubleListNode_t* listNode, DoubleListNode_t* val)
        {
            if (listNode->next) listNode->next->previous = val;
            val->next = listNode->next;
            listNode->next = val;
            val->previous = listNode;
            return val;
        }

        template<typename DoubleListNode_t, typename ValueType_t>
        static DoubleListNode_t* listInsertBefore(DoubleListNode_t* listNode, ValueType_t* val)
        {
            DoubleListNode_t* newnode = new DoubleListNode_t();
            if (listNode->previous) listNode->previous->next = newnode;
            newnode->previous = listNode->previous;
            listNode->previous = newnode;
            newnode->next = listNode;
            newnode->value = val;
            return newnode;
        }

        template<typename DoubleListNode_t>
        static DoubleListNode_t* listInsertBefore(DoubleListNode_t* listNode, DoubleListNode_t* val)
        {
            if (listNode->previous) listNode->previous->next = val;
            val->previous = listNode->previous;
            listNode->previous = val;
            val->next = listNode;
            return val;
        }

        template<typename DoubleListNode_t>
        static DoubleListNode_t* listRemoveNode(DoubleListNode_t* listNode)
        {
            if (listNode->previous) listNode->previous->    next = listNode->    next;
            if (listNode->    next) listNode->    next->previous = listNode->previous;
            listNode->previous = nullptr;
            listNode->    next = nullptr;
            return listNode;
        }
};
#endif
