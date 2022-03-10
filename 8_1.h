#ifndef CPP_HW8_8_1_H
#define CPP_HW8_8_1_H

#include<atomic>
#include <iostream>
#include<memory>
#include<mutex>
#include<stdexcept>

using std::scoped_lock;

using std::atomic;

namespace mpcs51044 {
    namespace v1 {
        struct Stack {
            struct ListItem {
                ListItem(std::unique_ptr<ListItem> &&n, int d)
                        : next(std::move(n)), data(d) {}

                std::unique_ptr<ListItem> next;
                int data;
            };

            Stack() {
                std::cout << "locked stack get called!" << std::endl;
            } // Don't really need this, since compiler would generate on its own

            int pop() {
                scoped_lock guard(lock);
                if (!first) {
                    throw std::runtime_error("Can't pop from empty stack");
                }
                int result = first->data;
                first = std::move(first->next);
                return result;
            }

            void push(int i) {
                scoped_lock guard(lock);
                first = std::make_unique<ListItem>(std::move(first), i);
            }

        private:
            std::unique_ptr<ListItem> first;
            std::mutex lock;
        };


    }

    namespace v2 {

        struct StackItem {
            StackItem(int val) : next(0), value(val) {}

            StackItem *next; // Next item, 0 if this is last
            int value;
        };

        struct StackHead {
            StackItem *link;      // First item, 0 if list empty
            unsigned count;      // How many times the list has changed (see lecture notes)
        };

        struct Stack {
            Stack();

            int pop();

            void push(int);

        private:
            atomic<StackHead> head;
        };

        Stack::Stack() {
            std::cout << "lock-free stack get called!" << std::endl;

            StackHead init;
            init.link = nullptr;
            init.count = 0;
            head.store(init);
        }

// Pop value off list
        int
        Stack::pop() {
            // What the head will be if nothing messed with it
            StackHead expected = head.load();
            StackHead newHead;
            bool succeeded = false;
            while (!succeeded) {
                if (expected.link == 0) {
                    return 0; // List is empty
                }
                // What the head will be after the pop:
                newHead.link = expected.link->next;
                newHead.count = expected.count + 1;
                // Even if the compare_exchange fails, it updates expected.
                succeeded = head.compare_exchange_weak(expected, newHead);
            }
            int value = expected.link->value;
            delete expected.link;
            return value;
        }

// Push an item onto the list with the given head
        void
        Stack::push(int val) {
            StackHead original_head = head.load();
            StackHead newHead;
            StackItem *newFirst = new StackItem(val);
            do {
                newFirst->next = original_head.link;
                newHead.link = newFirst;
                newHead.count = original_head.count + 1;
            } while (!head.compare_exchange_weak(original_head, newHead));
        }

    }
    /* default use lock free*/
    using namespace v2;

}
#endif //CPP_HW8_8_1_H
