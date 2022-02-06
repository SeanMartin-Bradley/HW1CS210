#ifndef LIST_H
#define LIST_H

#include <algorithm>
using namespace std;

template <typename Object>
class List
{
private:
    // The basic doubly linked list node.
    // Nested inside of List, can be public
    // because the Node is itself private
    
    struct Node
    {
        
        Object  data;
        Node* prev;
        Node* next;

        Node(const Object& d = Object{ }, Node* p = nullptr, Node* n = nullptr)
            : data{ d }, prev{ p }, next{ n } { }

        Node(Object&& d, Node* p = nullptr, Node* n = nullptr)
            : data{ std::move(d) }, prev{ p }, next{ n } { }
    };

public:
    class const_iterator
    {
    public:

        // Public constructor for const_iterator.
        const_iterator() : current{ nullptr }
        { }

        // Return the object stored at the current position.
        // For const_iterator, this is an accessor with a
        // const reference return type.
        const Object& operator* () const
        {
            return retrieve();
        }

        const_iterator& operator++ ()
        {
            current = current->next;
            return *this;
        }

        const_iterator operator++ (int)
        {
            const_iterator old = *this;
            ++(*this);
            return old;
        }

        const_iterator& operator-- ()
        {
            current = current->prev;
            return *this;
        }

        const_iterator operator-- (int)
        {
            const_iterator old = *this;
            --(*this);
            return old;
        }

        bool operator== (const const_iterator& rhs) const
        {
            return current == rhs.current;
        }

        bool operator!= (const const_iterator& rhs) const
        {
            return !(*this == rhs);
        }

    protected:
        Node* current;

        // Protected helper in const_iterator that returns the object
        // stored at the current position. Can be called by all
        // three versions of operator* without any type conversions.
        Object& retrieve() const
        {
            return current->data;
        }

        // Protected constructor for const_iterator.
        // Expects a pointer that represents the current position.
        const_iterator(Node* p) : current{ p }
        { }

        friend class List<Object>;
    };

    class iterator : public const_iterator
    {
    public:

        // Public constructor for iterator.
        // Calls the base-class constructor.
        // Must be provided because the private constructor
        // is written; otherwise zero-parameter constructor
        // would be disabled.
        iterator()
        { }

        Object& operator* ()
        {
            return const_iterator::retrieve();
        }

        // Return the object stored at the current position.
        // For iterator, there is an accessor with a
        // const reference return type and a mutator with
        // a reference return type. The accessor is shown first.
        const Object& operator* () const
        {
            return const_iterator::operator*();
        }

        iterator& operator++ ()
        {
            this->current = this->current->next;
            return *this;
        }

        iterator operator++ (int)
        {
            iterator old = *this;
            ++(*this);
            return old;
        }

        iterator& operator-- ()
        {
            this->current = this->current->prev;
            return *this;
        }

        iterator operator-- (int)
        {
            iterator old = *this;
            --(*this);
            return old;
        }

    protected:
        // Protected constructor for iterator.
        // Expects the current position.
        iterator(Node* p) : const_iterator{ p }
        { }

        friend class List<Object>;
    };

public:
    int listSize = 0;
    List()
    {
        init();
    }

    ~List()
    {
        clear();
        delete head;
        delete tail;
    }


    /*      ONLY TOUCH CODE AFTER THIS LINE      */

    // Return mutable iterator representing beginning of list.
    iterator begin()
    {
        return iterator(head->next);
    }

    // Return constant iterator representing beginning of list.
    const_iterator begin() const
    {
        return const_iterator(head->next);
    }

    // Return iterator representing endmarker of list.
    // Mutator version is first, then accessor version.
    iterator end()
    {
        return iterator(tail->prev);
    }

    const_iterator end() const
    {
        return const_iterator(tail->prev);
    }

    // Return number of elements currently in the list.
    int size() const
    {
        return listSize;
    }

    // Return true if the list is empty, false otherwise.
    bool empty() const
    {
        if (head->next->next == nullptr)
        {
            return true;
        }
        return false;
    }

    // Removes all elements from the list
    void clear()
    {
        while (head->next->next != nullptr) 
        {
            this->pop_front();
        }

    }

    /* front, back, push_front, push_back, pop_front, and pop_back
       are the basic double-ended queue operations. */
    

    //Returns the value stored in the first element of the list
    const Object& front() const
    {
        //note: these values need to be replaced, just here for compilation purposes
        return head->next->data;
    }

    //Returns the value stored in the last element of the list
    const Object& back() const
    {
        //note: these values need to be replaced, just here for compilation purposes
        return tail->prev->data;
    }

    //Inserts an object at the front of the list
    void push_front(const Object& x)
    {
        //create a pointer to node containing x, with the previous being the head, and the next value being the first node
        struct Node* newNode = new Node(x, head, head->next);
        //change the first node's previous value to be the node we're adding
        head->next->prev = newNode;
        //Make the head point to the new node next
        head->next = newNode;
        listSize++;
    }

    //Inserts an object at the back of the list
    void push_back(const Object& x)
    {
        //create a pointer to node containing x, with the previous node being the last node, and the next being the tail
        struct Node* newNode = new Node(x, tail->prev, tail);
        //make it so the last value's next value now points to the node being added to the end
        tail->prev->next = newNode;
        //make the tail point to the new node being added at the end of the list
        tail->prev = newNode;
        listSize++;
    }

    //Removes the first element in the list
    void pop_front()
    {
        //Create a pointer to the second node (which will become the first). 
        Node* nodePtr = head->next->next;
        //Cause the second node to have a previous of head.
        head->next->next->prev = head;
        //Delete the first node altogether.
        delete head->next;
        //Cause head to point to the new fist node.
        head->next = nodePtr;
        listSize--;
    }

    //Removes the last element in the list
    void pop_back()
    {
        //Create a pointer to the second to last node, which will become the last.
        Node* nodePtr = tail->prev->prev;
        //Cause the second to last node's next value to point to the tail, skipping the current last.
        tail->prev->prev->next = tail;
        //Delete the last node altogether.
        delete tail->prev;
        //Cause tail to point to the new last node.
        tail->prev = nodePtr;
        listSize--;
    }

    // Insert x before itr.
    iterator insert(iterator itr, const Object& x)
    {
       
        struct Node* newNode = new Node(x, itr->prev, itr); //Create a new node containing x, with the previous being the iterators previous, and next being the iterator's current space.
        itr->prev->next = newNode; //If a node is ordered 4, 5 (newly inserted), 6, 6's previous (4) should have a next of 5.
        itr->prev = newNode; //Set the iterator position, 6's, previous value to equal 5.
        listSize++;
        return newNode;
    }

    // Erase item at itr.
    iterator remove(iterator itr)
    {
        Node* nodePtrNext = itr->next; //Take record of the node after what is being deleted.
        Node* nodePtrPrev = itr->prev; //Take record of the node before what is being deleted.
        nodePtrPrev->next = nodePtrNext; //In a hypothetical 3, 4, 5, make 3's next 5. 
        nodePtrNext->prev = nodePtrPrev; //In a hypothetical 3, 4, 5, make 5's previous 3.
        delete itr; //Delete hypothetical 4.
        listSize--;
        return itr;
    }

    // Return the index of the node containing the matching value
    // Return -1 if no matching value
    int find(const Object& x)
    {
        int count = 0;
        Node* currentNode = head->next;
        while (currentNode->next != nullptr) //If the next is equal to nullpointer, the next is the tail.
        {
            if (currentNode->data == x)
            {
                return count;
            }
            currentNode = currentNode->next;
            count++;
        }
        return -1;
    }

private:
    Node* head = new Node();
    Node* tail = new Node();

    void init()
    {
        head->next = tail;
        tail->prev = head;
    }
};

#endif
