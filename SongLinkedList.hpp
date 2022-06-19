// Matthew Delgado
// Assignment Chapter 3 - SongLinkedList
// ECE 2552 Software/Hardware Integration
// Dr. Smith
// 2-10-22

#ifndef SongLinkedList_hpp
#define SongLinkedList_hpp

#include <stdio.h>
#include <iostream>

// templated list class includes node struct and const/non-const iterator classes
template <typename T>
class cir_list{
private:
    struct cir_list_node{   // node struct, was attempted as a class but did not perform properly
        T data;
        cir_list_node* prev;    // pointer to previous node in list
        cir_list_node* next;    // pointer to next node in list
    };

public:
    /*---------------------- const iterator class (nested inside list class) -------------------- */
    class cir_list_it_const{
    public:
        cir_list_it_const() : currNd{nullptr} {} // constructor
        
        const T& operator*() const { return getElement(); } // overloaded * to return element, dereference

        // overloaded increment++ and decrement-- for const iterator
        cir_list_it_const& operator++(){
            currNd = currNd->next;
            return *this;
        }
        cir_list_it_const operator++(int){
            auto copy = *this;
            currNd = currNd->next;
            return copy;
        }
        cir_list_it_const& operator--(){
            currNd = currNd->prev;
            return *this;
        }
        cir_list_it_const operator--(int){
            auto copy = *this;
            currNd = currNd->prev;
            return *this;
        }

        // overloaded const == and !=
        bool operator==(const cir_list_it_const& rhs) const { return currNd == rhs.currNd; }
        bool operator!=(const cir_list_it_const& rhs) const { return currNd != rhs.currNd; }

    protected:
        cir_list_node* currNd; // pointer to cir_list_node in cir_list
        T& getElement() const { return currNd->data; } // get data from current element
        cir_list_it_const(cir_list_node* ptr) : currNd{ptr} {} // protected constructor for const iterator class
        friend class cir_list<T>;
    };
    
    /*---------------------- non-const iterator class (nested inside main cir_list class) -------------------- */
    class cir_list_it : public cir_list_it_const{
    public:
        cir_list_it() {}    // cir_list non-const constructor
        T& operator*() { return this->currNd->data; } // overloaded * to return element, dereference
        const T& operator*() const { return this->getElement(); } // overloaded * to return element, dereference

        // overloaded increment++ and decrement-- for non-const iterator
        cir_list_it& operator++(){
            this->currNd = this->currNd->next;
            return *this;
        }
        cir_list_it operator++(int){
            auto copy = *this;
            this->currNd = this->currNd->next;
            return copy;
        }
        cir_list_it& operator--(){
            this->currNd = this->currNd->prev;
            return *this;
        }
        cir_list_it operator--(int){
            auto copy = *this;
            this->currNd = this->currNd->prev;
            return *this;
        }

    protected:
        cir_list_it(cir_list_node* ptr) : cir_list_it_const{ptr} {} // protected constructor for iterator class
        friend class cir_list<T>;
    };
    
public:
    cir_list() { init(); }  // list constructor
    cir_list(const cir_list& rhs){  // copy constructor
        init();
        for(auto itr = rhs.begin(); itr != rhs.end(); ++itr)
            push_back(*itr);
    }
    cir_list(const std::initializer_list<T>& il){}  // really have no idea what to do with this, was required in lab instructions

    ~cir_list() { clear(); delete header; delete trailer; } // destructor deletes everything

    // overloaded =
    const cir_list& operator=(const cir_list& rhs) { auto copy = rhs; std::swap(*this, copy); return *this; }
    cir_list& operator=(cir_list&& rhs){
        init();
        *this = std::move(rhs);
        return *this;
    }
    
    // overloaded == operator
    friend bool operator==(const cir_list<T>& l, const cir_list<T>& r){
        bool isEqual = true;
        if(l.size() == r.size()){
            auto r_itr = r.begin();
            for(auto l_itr = l.begin(); l_itr != l.end(); ++l_itr){
                if(*l_itr != *r_itr)
                   isEqual = false; break;
                ++r_itr;
            }
            return isEqual;
        }
        else
            return false;
    }
    // overloade != operator
    friend bool operator!=(const cir_list<T>& lhs, const cir_list<T>& rhs) { return !(lhs == rhs); }

    int size() const { return lstSize; }    // returns size of list
    bool empty() const { return !size(); }  // returns if list is empty
    void clear(){   // clears list, deletes all nodes
        while(!empty())
            pop_back();
    }

    T& firstNode() { return *begin(); } // returns first node
    const T& firstNode() const { return *begin(); }
    T& lastNode() { return *(--end()); }    // returns last node, end points past last node
    const T & lastNode() const { return *(--end()); }

    void push_front(const T& val) { insert(begin(), val); } // insert in the beginning of list
    void push_back(const T& val) { insert(end(), val); }    // insert in the end of the list
    
    void pop_front() { erase(begin()); }    // delete node from beginning
    void pop_back() { erase(--end()); } // delete node from end

    void remove(const T& val){  // remove node with specific value
        for(auto it = begin(); it != end(); ++it){
            if(*it == val)
                erase(it);
        }
    }

    cir_list_it begin(){    // returns it of first node
        if(!empty()){
            cir_list_it it{header->next};
            return it;
        }
        return header;
    }
    cir_list_it_const begin() const{
        if(!empty()){
            cir_list_it_const const_it{header->next};
            return const_it;
        }
        return header;
    }
    
    cir_list_it end(){  // returns iterator of trailer node
        cir_list_it it{ trailer };
        return it;
    }
    cir_list_it_const end() const{
        cir_list_it_const const_it{trailer};
        return const_it;
    }
    
    cir_list_it insert(cir_list_it it, const T& val){  // inserts node
        auto* ptr = it.currNd;
        lstSize++;
        auto* newPtr = new cir_list_node{val, ptr->prev, ptr};
        ptr->prev->next = newPtr;
        ptr->prev = newPtr;
        cir_list_it iter{newPtr};
        return iter;
    }
    cir_list_it insert(cir_list_it it, T&& val){
        auto* ptr = it.currNd;
        lstSize++;
        auto* newPtr = new cir_list_node{ std::move(val), ptr->prev, ptr};
        ptr->prev->next = newPtr;
        ptr->prev = newPtr;
        cir_list_it iter{newPtr};
        return iter;
    }
    
    /* "auto" uses initialization of declared variable to figure out its type. */
    cir_list_it erase(cir_list_it start, cir_list_it end){  // erases node
        for(auto it = start; it != end;)
            it = erase(it);
        return end;
    }
    
    /* "auto" uses initialization of declared variable to figure out its type. */
    cir_list_it erase(cir_list_it it){
        auto* ptr = it.currNd;
        cir_list_it value{ptr->next};
        ptr->prev->next = ptr->next;
        ptr->next->prev = ptr->prev;
        delete ptr;
        lstSize--;
        return value;
    }

private:
    int lstSize; // size of each list, # of nodes
    cir_list_node* header; // first node in list
    cir_list_node* trailer; // last node in list

    // private function to inialize all private data members, less clutter in code
    void init(){ lstSize = 0; header = new cir_list_node; trailer = new cir_list_node; header->next = trailer; trailer->prev = header;}
}; /*---------------------- end of list class -------------------- */

/*--------- start of Playlist class (acts as container of cir_list class -------------- */
template <typename T>
class Playlist : public cir_list<T>{
public:    
    void loopOnce(){    // prints the playlist from firstNode to end
        for(auto iter = this->begin(); iter != this->end(); ++iter)
            std::cout << *iter << " ";
    }
}; /*---------------------- end of Playlist class -------------------- */

#endif /* SongLinkedList_hpp */
