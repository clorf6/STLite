#ifndef SJTU_PRIORITY_QUEUE_HPP
#define SJTU_PRIORITY_QUEUE_HPP

#include <cstddef>
#include <functional>
#include "exceptions.hpp"

namespace sjtu {

/**
 * a container like std::priority_queue which is a heap internal.
 */
    template<typename T, class Compare = std::less<T>>
    class priority_queue {
    public:
        struct Node {
            Node *ls, *rs;
            T data;
            int dis;

            Node(const T &val) : ls(nullptr), rs(nullptr), dis(0), data(val) {}

        };

        Node *root;
        int siz;
        /**
         * TODO constructors
         */
        priority_queue() {
            root = nullptr;
            siz = 0;
        }

        Node* clone(Node* other) {
            Node *ret = new Node(other->data);
            ret->dis = other->dis;
            if (other->ls) {
                ret->ls = clone(other->ls);
            }
            if (other->rs) {
                ret->rs = clone(other->rs);
            }
            return ret;
        }

        priority_queue(const priority_queue &other) {
            root = clone(other.root);
            siz = other.siz;
        }

        /**
         * TODO deconstructor
         */
        void clean(Node* &now) {
            if (now == nullptr) return ;
            if (now->ls) clean(now->ls);
            if (now->rs) clean(now->rs);
            delete now;
            now = nullptr;
        }

        ~priority_queue() {
            clean(root);
            siz = 0;
        }

        /**
         * TODO Assignment operator
         */
        priority_queue &operator=(const priority_queue &other) {
            if (this == &other) return (*this);
            clean(root);
            root = clone(other.root);
            siz = other.siz;
            return (*this);
        }

        /**
         * get the top of the queue.
         * @return a reference of the top element.
         * throw container_is_empty if empty() returns true;
         */
        const T &top() const {
            if (!siz) throw container_is_empty();
            return root->data;
        }

        /**
         * TODO
         * push new element to the priority queue.
         */
        void push(const T &e) {
            priority_queue<T, Compare> now;
            now.root = new Node(e);
            now.siz = 1;
            merge(now);
        }

        /**
         * TODO
         * delete the top element.
         * throw container_is_empty if empty() returns true;
         */
        void pop() {
            if (!siz) throw container_is_empty();
            siz--;
            Node* pos = root;
            root = merge(root->ls, root->rs);
            delete pos;
        }

        /**
         * return the number of the elements.
         */
        size_t size() const {
            return siz;
        }

        /**
         * check if the container has at least an element.
         * @return true if it is empty, false if it has at least an element.
         */
        bool empty() const {
            return (!siz);
        }

        /**
         * merge two priority_queues with at least O(logn) complexity.
         * clear the other priority_queue.
         */

        Node *merge(Node *x, Node *y) {
            if (x == nullptr)
                return y;
            if (y == nullptr)
                return x;
            if (Compare()(x->data, y->data))
                std::swap(x, y);
            x->rs = merge(x->rs, y);
            if (x->ls == nullptr) {
                x->ls = x->rs;
                x->rs = nullptr;
            } else if (x->rs->dis > x->ls->dis)
                std::swap(x->ls, x->rs);
            x->dis = (x->rs == nullptr) ? 0 : x->rs->dis + 1;
            return x;
        }

        void merge(priority_queue &other) {
            siz += other.siz;
            root = merge(root, other.root);
            other.root = nullptr;
            other.siz = 0;
        }
    };

}

#endif
