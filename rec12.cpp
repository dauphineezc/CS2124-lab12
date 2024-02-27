// Zoe Dauphinee, lab 12, implementing a linked list

#include <cstdlib>
#include <iostream>
using namespace std;

class List {
    private:
        struct Node {
            Node* prior;
            Node* next;
            int data;
        };
        Node* header;
        Node* trailer;
        int list_size;

    public:
        List() : header(new Node{nullptr, nullptr, 0}), trailer(new Node{nullptr, nullptr, 0}), list_size(0) {
            header->next = trailer;
            trailer->prior = header;
        }

        void push_back(int data) {
            Node* curr_node = header;
            while (curr_node->next != trailer) {
                curr_node = curr_node->next;
            }
            Node* new_node = new Node{curr_node, trailer, data};
            curr_node->next = new_node;
            trailer->prior = new_node;
            ++list_size;
        }

        void push_front(int data) {
            Node* prev_first_node = header->next;
            Node* new_node = new Node{header, prev_first_node, data};
            header->next = new_node;
            prev_first_node->prior = new_node;
            ++list_size;
        }

        void pop_back() {
            Node* curr_node = header;
            while (curr_node->next != trailer) {
                curr_node = curr_node->next;
            }
            curr_node->prior->next = trailer;
            trailer->prior = curr_node->prior;
            delete curr_node;
            --list_size;
        }

        void pop_front() {
            Node* to_be_popped = header->next;
            header->next = to_be_popped->next;
            to_be_popped->next->prior = header;
            delete to_be_popped;
            --list_size;
        }

        int& front() {
            int& data = header->next->data;
            return data;
        }

        const int& front() const {
            const int& data = header->next->data;
            return data;
        }

        int& back() {
            int& data = trailer->prior->data;
            return data;
        }

        const int back() const {
            const int& data = trailer->prior->data;
            return data;
        }

        int size() const {
            return list_size;
        }

        bool retrieve_data(int position, int& data) const {
            const Node* curr_node = header;
            for (int index = 0; index <= position && curr_node; ++index) {
                curr_node = curr_node->next;
                if (index == position && curr_node != trailer) {
                    data = curr_node->data;
                    return true;
                }
            }
            return false;
        }

        void clear() {
            Node* curr_node = header->next;
            while (curr_node != trailer) {
                Node* node_to_delete = curr_node;
                curr_node = curr_node->next;
                delete node_to_delete;
            }
            header->next = trailer;
            trailer->prior = header;
            list_size = 0;
        }

        int& operator[](int index) {
            Node* curr_node = header->next;
            for (int i = 0; i < index; ++i) {
                if (curr_node == trailer) {
                    cout << "Index out of range";
                    break;
                }
                curr_node = curr_node->next;
            }
            return curr_node->data;
        }

        const int& operator[](int index) const {
            const Node* curr_node = header->next;
            for (int i = 0; i < index; ++i) {
                if (curr_node == trailer) {
                    cout << "Index out of range";
                    break;
                }
                curr_node = curr_node->next;
            }
            return curr_node->data;
        }


    friend ostream& operator<<(ostream& os, const List& list);

    class iterator {
        public:
            Node* current;

            iterator(Node* node) : current(node) {}

            iterator& operator++() {
                if (current) {
                    current = current->next;
                }
                return *this;
            }

            iterator& operator--() {
                if (current) {
                    current = current->prior;
                }
                return *this;
            }

            bool operator==(const iterator& other) const {
                return current == other.current;
            }

            bool operator!=(const iterator& other) const {
                return current != other.current;
            }

            int& operator*() const {
                return current->data;
            }
    };

    iterator begin() const {
        return iterator(header->next);
    }

    iterator end() const {
        return iterator(trailer);
    }

    iterator insert(const iterator& position, const int data) {
        if (position.current == header) {
            Node* new_node = new Node({nullptr, nullptr, data});
            new_node->next = header;
            if (header) {
                header->prior = new_node;
            }
                header = new_node;
                return iterator(new_node);
            } 
        else if (position.current == nullptr) {
            Node* new_node = new Node({nullptr, nullptr, data});
            if (trailer) {
                trailer->next = new_node;
                new_node->prior = trailer;
            } 
            else {
                header = new_node;
            }
            trailer = new_node;
            return iterator(new_node);
        } 
        else {
            Node* new_node = new Node({nullptr, nullptr, data});
            Node* current = position.current;
            new_node->prior = current->prior;
            new_node->next = current;
            if (current->prior) {
                current->prior->next = new_node;
            } 
            else {
                header = new_node;
            }
            current->prior = new_node;
            ++list_size;
            return iterator(new_node);
        }
    }
    
    iterator erase(const iterator& position) {
        if (!position.current) {
            return end();
        }

        Node* current = position.current;
        Node* next_node = current->next;

        if (current->prior) {
            current->prior->next = current->next;
        } 
        else {
            header = current->next;
        }
        if (current->next) {
            current->next->prior = current->prior;
        } 
        else {
            trailer = current->prior;
        }

        delete current;

        --list_size;
        return iterator(next_node);
    }
};


// Task 1
void printListInfo(const List& myList) {
        cout << "size: " << myList.size()
             << ", front: " << myList.front()
             << ", back(): " << myList.back()
             << ", list: " << myList << endl;
}

/*
// The following should not compile. Check that it does not.
void changeFrontAndBackConst(const List& theList) {
    theList.front() = 17;
    theList.back() = 42;
}
*/

void changeFrontAndBack(List& theList) {
    theList.front() = 17;
    theList.back() = 42;
}

// Task 4
void printListSlow(const List& myList) {
    for (size_t i = 0; i < myList.size(); ++i) {
        cout << myList[i] << ' ';
    }
    cout << endl;
}


//
int main() {
    // Task 1
    cout << "\n------Task One------\n";
    List myList;
    cout << "Fill empty list with push_back: i*i for i from 0 to 9\n";
    for (int i = 0; i < 10; ++i) {
        cout << "myList.push_back(" << i*i << ");\n";
        myList.push_back(i*i);
        printListInfo(myList);
    }
    cout << "===================\n";
    
    cout << "Modify the first and last items, and display the results\n";
    changeFrontAndBack(myList);
    printListInfo(myList);
    cout << "===================\n";

    cout << "Remove the items with pop_back\n";
    while (myList.size()) {
        printListInfo(myList);
        myList.pop_back();
    }
    cout << "===================\n";

    // Task 2
    cout << "\n------Task Two------\n";
    cout << "Fill empty list with push_front: i*i for i from 0 to 9\n";
    for (int i = 0; i < 10; ++i) {
        cout << "myList2.push_front(" << i*i << ");\n";
        myList.push_front(i*i);
        printListInfo(myList);
    }
    cout << "===================\n";
    cout << "Remove the items with pop_front\n";
    while (myList.size()) {
        printListInfo(myList);
        myList.pop_front();
    }
    printListInfo(myList);
    cout << "===================\n";

    // Task 3
    cout << "\n------Task Three------\n";
    cout << "Fill empty list with push_back: i*i for i from 0 to 9\n";
    for (int i = 0; i < 10; ++i) {
        myList.push_back(i*i);
    }
    printListInfo(myList);
    cout << "Now clear\n";
    myList.clear();
    cout << "Size: " << myList.size() << ", list: " << myList << endl;
    cout << "===================\n";

    // Task 4
    cout << "\n------Task Four------\n";
    cout << "Fill empty list with push_back: i*i for i from 0 to 9\n";
    for (int i = 0; i < 10; ++i)  myList.push_back(i*i);
    cout << "Display elements with op[]\n";
    for (size_t i = 0; i < myList.size(); ++i) cout << myList[i] << ' ';
    cout << endl;
    cout << "Add one to each element with op[]\n";
    for (size_t i = 0; i < myList.size(); ++i) myList[i] += 1;
    cout << "And print it out again with op[]\n";
    for (size_t i = 0; i < myList.size(); ++i) cout << myList[i] << ' ';
    cout << endl;
    cout << "Now calling a function, printListSlow, to do the same thing\n";
    printListSlow(myList);
    cout << "Finally, for this task, using the index operator to modify\n"
         << "the data in the third item in the list\n"
         << "and then using printListSlow to display it again\n";
    myList[2] = 42;
    printListSlow(myList);

    // Task 5
    cout << "\n------Task Five------\n";
    cout << "Fill empty list with push_back: i*i for i from 0 to 9\n";
    myList.clear();
    for (int i = 0; i < 10; ++i)  myList.push_back(i*i);
    printListInfo(myList);
    cout << "Now display the elements in a ranged for\n";
    for (int x : myList) cout << x << ' ';
    cout << endl;
    cout << "And again using the iterator type directly:\n";
    // Note you can choose to nest the iterator class or not, your choice.
    //for (iterator iter = myList.begin(); iter != myList.end(); ++iter) {
    for (List::iterator iter = myList.begin(); iter != myList.end(); ++iter) {
        cout << *iter << ' ';
    }
    cout << endl;
    cout << "WOW!!! (I thought it was cool.)\n";

    // Task 6
    cout << "\n------Task Six------\n";
    cout << "Filling an empty list with insert at end: i*i for i from 0 to 9\n";
    myList.clear();
    for (int i = 0; i < 10; ++i) myList.insert(myList.end(), i*i);
    printListInfo(myList);
    cout << "Filling an empty list with insert at begin(): "
         << "i*i for i from 0 to 9\n";
    myList.clear();
    for (int i = 0; i < 10; ++i) myList.insert(myList.begin(), i*i);
    printListInfo(myList);
    // ***Need test for insert other than begin/end***
    cout << "===================\n";

    // Task 7
    cout << "\n------Task Seven------\n";
    cout << "Filling an empty list with insert at end: i*i for i from 0 to 9\n";
    myList.clear();
    for (int i = 0; i < 10; ++i) myList.insert(myList.end(), i*i);
    cout << "Erasing the elements in the list, starting from the beginning\n";
    while (myList.size()) {
        printListInfo(myList);
        myList.erase(myList.begin());
    }
    // ***Need test for erase other than begin/end***
    cout << "===================\n";
    
}


ostream& operator<<(ostream& os, const List& list) {
    int data;
    int index = 0;
    while (list.retrieve_data(index, data)) {
        os << data << " ";
        ++index;
    }
    return os;
}