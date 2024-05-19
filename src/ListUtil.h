#ifndef NSLIST_H
#define NSLIST_H

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
