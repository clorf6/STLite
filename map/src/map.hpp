/**
 * implement a container like std::map
 */
#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP

// only for std::less<T>
#include <functional>
#include <cstddef>
#include "utility.hpp"
#include "exceptions.hpp"

namespace sjtu {

    template<typename U>
    struct my_type_traits {
        using iterator_assignable = typename U::iterator_assignable;
    };

    struct my_true_type {
        static constexpr bool value = true;
    };

    struct my_false_type {
        static constexpr bool value = false;
    };

    template<typename T, typename U>
    struct my_is_same : my_false_type {};

    template<typename T>
    struct my_is_same<T, T> : my_true_type {};

    template<
            class Key,
            class T,
            class Compare = std::less<Key>
    >
    class map {
    public:
        /**
         * the internal type of data.
         * it should have a default constructor, a copy constructor.
         * You can use sjtu::map as value_type by typedef.
         */
        typedef pair<const Key, T> value_type;
        /**
         * see BidirectionalIterator at CppReference for help.
         *
         * if there is anything wrong throw invalid_iterator.
         *     like it = map.begin(); --it;
         *       or it = map.end(); ++end();
         */

        enum color : bool {
            BLACK = false, RED = true
        };

        struct Node {
            value_type *val;
            color col;
            Node *fa;
            Node *ch[2];

            explicit Node() :
                    col(RED),
                    fa(nullptr) {
                val = nullptr;
                ch[0] = ch[1] = nullptr;
            };

            ~Node() {
                delete val;
                val = nullptr;
            }
        };

        Node *root;
        Node *nullNode;
        Node *minn;
        Node *maxx;
        int siz;

        class iterator;
        class const_iterator;

        class iterator {
        private:
            /**
             * TODO add data members
             *   just add whatever you want.
             */
            friend class map;

            Node *now;
            const map<Key, T, Compare> *bel;

        public:
            using iterator_assignable = my_true_type;

            iterator() {
                // TODO
                now = nullptr;
                bel = nullptr;
            }

            iterator(Node *_now, const map<Key, T, Compare> *_bel) {
                bel = _bel;
                now = _now;
            }

            iterator(const iterator &other) {
                // TODO
                bel = other.bel;
                now = other.now;
            }

            /**
             * TODO iter++
             */
            iterator operator++(int) {
                iterator ret = (*this);
                now = bel->suc(now);
                return ret;
            }

            /**
             * TODO ++iter
             */
            iterator &operator++() {
                now = bel->suc(now);
                return (*this);
            }

            /**
             * TODO iter--
             */
            iterator operator--(int) {
                iterator ret = (*this);
                now = bel->pre(now);
                return ret;
            }

            /**
             * TODO --iter
             */
            iterator &operator--() {
                now = bel->pre(now);
                return (*this);
            }

            /**
             * a operator to check whether two iterators are same (pointing to the same memory).
             */
            value_type &operator*() const {
                return *(now->val);
            }

            bool operator==(const iterator &rhs) const {
                return now == rhs.now;
            }

            bool operator==(const const_iterator &rhs) const {
                return now == rhs.now;
            }

            /**
             * some other operator for iterator.
             */
            bool operator!=(const iterator &rhs) const {
                return bel != rhs.bel || now != rhs.now;
            }

            bool operator!=(const const_iterator &rhs) const {
                return bel != rhs.bel || now != rhs.now;
            }

            /**
             * for the support of it->first.
             * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
             */
            value_type *operator->() const noexcept {
                return now->val;
            }
        };

        class const_iterator {
            // it should has similar member method as iterator.
            //  and it should be able to construct from an iterator.
        private:
            // data members.
            friend class map;

            Node *now;
            const map<Key, T, Compare> *bel;
        public:
            using iterator_assignable = my_false_type;

            const_iterator() {
                // TODO
                now = nullptr;
                bel = nullptr;
            }

            const_iterator(Node *_now, const map<Key, T, Compare> *_bel) {
                bel = _bel;
                now = _now;
            }

            const_iterator(const const_iterator &other) {
                // TODO
                bel = other.bel;
                now = other.now;
            }

            const_iterator(const iterator &other) {
                // TODO
                bel = other.bel;
                now = other.now;
            }
            // And other methods in iterator.
            // And other methods in iterator.
            // And other methods in iterator.
            /**
             * TODO iter++
             */
            const_iterator operator++(int) {
                const_iterator ret = (*this);
                now = bel->suc(now);
                return ret;
            }

            /**
             * TODO ++iter
             */
            const_iterator &operator++() {
                now = bel->suc(now);
                return (*this);
            }

            /**
             * TODO iter--
             */
            const_iterator operator--(int) {
                const_iterator ret = (*this);
                now = bel->pre(now);
                return ret;
            }

            /**
             * TODO --iter
             */
            const_iterator &operator--() {
                now = bel->pre(now);
                return (*this);
            }

            /**
             * a operator to check whether two iterators are same (pointing to the same memory).
             */
            value_type operator*() const {
                return *(now->val);
            }

            bool operator==(const iterator &rhs) const {
                return now == rhs.now;
            }

            bool operator==(const const_iterator &rhs) const {
                return now == rhs.now;
            }

            /**
             * some other operator for iterator.
             */
            bool operator!=(const iterator &rhs) const {
                return bel != rhs.bel || now != rhs.now;
            }

            bool operator!=(const const_iterator &rhs) const {
                return bel != rhs.bel || now != rhs.now;
            }

            /**
             * for the support of it->first.
             * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
             */
            value_type *operator->() const noexcept {
                return now->val;
            }
        };

        /**
         * TODO two constructors
         */
        map() {
            nullNode = new Node();
            siz = 0;
            nullNode->col = BLACK;
            nullNode->fa = nullNode->ch[0] = nullNode->ch[1] = nullNode;
            root = minn = maxx = nullNode;
        }

        Node *clone(Node *other, Node *fa, Node *other_nullNode) {
            Node *ret = new Node();
            ret->fa = fa;
            ret->col = other->col;
            ret->val = new value_type(*(other->val));
            ret->ch[0] = ret->ch[1] = nullNode;
            if (other->ch[0] != other_nullNode) {
                ret->ch[0] = clone(other->ch[0], ret, other_nullNode);
            }
            if (other->ch[1] != other_nullNode) {
                ret->ch[1] = clone(other->ch[1], ret, other_nullNode);
            }
            return ret;
        }

        map(const map &other) {
            nullNode = new Node();
            nullNode->col = BLACK;
            nullNode->fa = nullNode->ch[0] = nullNode->ch[1] = nullNode;
            if (other.root != other.nullNode) root = clone(other.root, nullNode, other.nullNode);
            else root = nullNode;
            siz = other.siz;
            minn = root;
            while (minn->ch[0] != nullNode) minn = minn->ch[0];
            maxx = root;
            while (maxx->ch[1] != nullNode) maxx = maxx->ch[1];
        }

        void clean(Node *rt) {
            if (rt == nullNode) return;
            if (rt->ch[0] != nullNode) clean(rt->ch[0]);
            if (rt->ch[1] != nullNode) clean(rt->ch[1]);
            delete rt;
            rt = nullNode;
        }

        /**
         * TODO assignment operator
         */
        map &operator=(const map &other) {
            if (this == &other) return (*this);
            clean(root);
            if (other.root != other.nullNode) root = clone(other.root, nullNode, other.nullNode);
            else root = nullNode;
            siz = other.siz;
            minn = root;
            while (minn->ch[0] != nullNode) minn = minn->ch[0];
            maxx = root;
            while (maxx->ch[1] != nullNode) maxx = maxx->ch[1];
            return (*this);
        }

        /**
         * TODO Destructors
         */
        ~map() {
            clean(root);
            siz = 0;
            root = minn = maxx = nullNode;
            delete nullNode;
            nullNode = nullptr;
        }

        bool Dir(Node *rt) {
            return rt->fa->ch[1] == rt;
        }

        Node *NewNode(const value_type &val) {
            Node *rt = new Node();
            rt->val = new value_type(val);
            rt->fa = rt->ch[0] = rt->ch[1] = nullNode;
            return rt;
        }

        void Rotate(Node *rt, bool dir) {
            Node *new_rt = rt->ch[!dir];
            rt->ch[!dir] = new_rt->ch[dir];
            if (new_rt->ch[dir] != nullNode) {
                new_rt->ch[dir]->fa = rt;
            }
            new_rt->fa = rt->fa;
            if (rt->fa == nullNode) {
                root = new_rt;
            } else {
                rt->fa->ch[Dir(rt)] = new_rt;
            }
            new_rt->ch[dir] = rt;
            rt->fa = new_rt;
        }

        Node *pre(Node *now) const {
            if (!siz) throw invalid_iterator();
            if (now == nullNode) {
                return maxx;
            }
            if (now->ch[0] != nullNode) {
                now = now->ch[0];
                while (now->ch[1] != nullNode) now = now->ch[1];
                return now;
            } else {
                while (now != root) {
                    Node *fa = now->fa;
                    if (fa->ch[1] == now) {
                        return fa;
                    }
                    now = fa;
                }
                throw invalid_iterator();
            }
        }

        Node *suc(Node *now) const {
            if (!siz) throw invalid_iterator();
            if (now == nullNode) throw invalid_iterator();
            if (now->ch[1] != nullNode) {
                now = now->ch[1];
                while (now->ch[0] != nullNode) now = now->ch[0];
                return now;
            } else {
                while (now != root) {
                    Node *fa = now->fa;
                    if (fa->ch[0] == now) {
                        return fa;
                    }
                    now = fa;
                }
                return nullNode;
            }
        }

        /**
         * TODO
         * access specified element with bounds checking
         * Returns a reference to the mapped value of the element with key equivalent to key.
         * If no such element exists, an exception of type `index_out_of_bound'
         */

        T &at(const Key &key) {
            Node *rt = root;
            while (rt != nullNode) {
                value_type &now = *(rt->val);
                bool if_less = Compare()(key, now.first);
                bool if_more = Compare()(now.first, key);
                if (!if_less && !if_more) return (*(rt->val)).second;
                if (if_less) {
                    rt = rt->ch[0];
                } else rt = rt->ch[1];
            }
            throw index_out_of_bound();
        }

        const T &at(const Key &key) const {
            Node *rt = root;
            while (rt != nullNode) {
                value_type &now = *(rt->val);
                bool if_less = Compare()(key, now.first);
                bool if_more = Compare()(now.first, key);
                if (!if_less && !if_more) return (*(rt->val)).second;
                if (if_less) {
                    rt = rt->ch[0];
                } else rt = rt->ch[1];
            }
            throw index_out_of_bound();
        }

        /**
         * TODO
         * access specified element
         * Returns a reference to the value that is mapped to a key equivalent to key,
         *   performing an insertion if such key does not already exist.
         */
        T &operator[](const Key &key) {
            Node *rt = root;
            while (rt != nullNode) {
                value_type &now = *(rt->val);
                bool if_less = Compare()(key, now.first);
                bool if_more = Compare()(now.first, key);
                if (!if_less && !if_more) return (*(rt->val)).second;
                if (if_less) {
                    rt = rt->ch[0];
                } else rt = rt->ch[1];
            }
            value_type newNode(key, T());
            pair<iterator, bool> ret = insert(newNode);
            return (ret.first.now)->val->second;
        }

        /**
         * behave like at() throw index_out_of_bound if such key does not exist.
         */
        const T &operator[](const Key &key) const {
            Node *rt = root;
            while (rt != nullNode) {
                value_type &now = *(rt->val);
                bool if_less = Compare()(key, now.first);
                bool if_more = Compare()(now.first, key);
                if (!if_less && !if_more) return (*(rt->val)).second;
                if (if_less) {
                    rt = rt->ch[0];
                } else rt = rt->ch[1];
            }
            throw index_out_of_bound();
        }

        /**
         * return a iterator to the beginning
         */
        iterator begin() {
            return iterator(minn, this);
        }

        const_iterator cbegin() const {
            return const_iterator(minn, this);
        }

        /**
         * return a iterator to the end
         * in fact, it returns past-the-end.
         */
        iterator end() {
            return iterator(nullNode, this);
        }

        const_iterator cend() const {
            return const_iterator(nullNode, this);
        }

        /**
         * checks whether the container is empty
         * return true if empty, otherwise false.
         */
        bool empty() const {
            return !siz;
        }

        /**
         * returns the number of elements.
         */
        size_t size() const {
            return siz;
        }

        /**
         * clears the contents
         */
        void clear() {
            clean(root);
            siz = 0;
            root = minn = maxx = nullNode;
        }

        void Insert_fix(Node *rt) {
            while (rt->fa->col) {
                Node *f = rt->fa, *gf = f->fa;
                bool dir = Dir(f);
                Node *unc = gf->ch[!dir];
                if (unc->col) {
                    f->col = unc->col = BLACK;
                    gf->col = RED;
                    rt = gf;
                } else {
                    if (rt == f->ch[!dir]) {
                        rt = f;
                        Rotate(f, dir);
                        f = rt->fa;
                    }
                    f->col = BLACK;
                    gf->col = RED;
                    Rotate(gf, !dir);
                    break;
                }
            }
            root->col = BLACK;
        }

        /**
         * insert an element.
         * return a pair, the first of the pair is
         *   the iterator to the new element (or the element that prevented the insertion),
         *   the second one is true if insert successfully, or false.
         */
        pair<iterator, bool> insert(const value_type &value) {
            Node *rt = root, *f = nullNode;
            while (rt != nullNode) {
                f = rt;
                bool if_less = Compare()(value.first, rt->val->first);
                bool if_more = Compare()(rt->val->first, value.first);
                if (!if_less && !if_more) {
                    return pair(iterator(rt, this), false);
                }
                rt = rt->ch[if_more];
            }
            siz++;
            rt = NewNode(value);
            if (f != nullNode) {
                bool if_more = Compare()(f->val->first, value.first);
                f->ch[if_more] = rt;
            } else root = rt;
            rt->fa = f;
            Insert_fix(rt);
            if (minn == nullNode || Compare()(value.first, minn->val->first)) {
                minn = rt;
            }
            if (maxx == nullNode || Compare()(maxx->val->first, value.first)) {
                maxx = rt;
            }
            return pair(iterator(rt, this), true);
        }

        void Remove_fix(Node *rt) {
            while (rt != root && !(rt->col)) {
                bool dir = Dir(rt);
                Node *f = rt->fa, *bro = f->ch[!dir];
                if (bro->col) {
                    bro->col = BLACK;
                    f->col = RED;
                    Rotate(f, dir);
                    bro = f->ch[!dir];
                }
                if (!(bro->ch[0]->col) && !(bro->ch[1]->col)) {
                    bro->col = RED;
                    rt = f;
                } else {
                    if (!(bro->ch[!dir]->col)) {
                        bro->ch[dir]->col = BLACK;
                        bro->col = RED;
                        Rotate(bro, !dir);
                        bro = f->ch[!dir];
                    }
                    bro->col = f->col;
                    bro->ch[!dir]->col = f->col = BLACK;
                    Rotate(f, dir);
                    break;
                }
            }
            rt->col = BLACK;
        }

        void swapNode(Node *&x, Node *&y) {
            bool dir_y = Dir(y);
            bool dir_x = Dir(x);
            if (y->fa == x) {
                if (x->fa != nullNode) x->fa->ch[dir_x] = y;
                y->fa = x->fa;
                x->fa = y;
                x->ch[dir_y] = y->ch[dir_y];
                y->ch[dir_y] = x;
                std::swap(y->ch[!dir_y], x->ch[!dir_y]);
                if (x->ch[0] != nullNode) x->ch[0]->fa = x;
                if (x->ch[1] != nullNode) x->ch[1]->fa = x;
                if (y->ch[0] != nullNode) y->ch[0]->fa = y;
                if (y->ch[1] != nullNode) y->ch[1]->fa = y;
            } else {
                if (x->fa != nullNode) x->fa->ch[dir_x] = y;
                if (y->fa != nullNode) y->fa->ch[dir_y] = x;
                std::swap(y->fa, x->fa);
                std::swap(y->ch[0], x->ch[0]);
                std::swap(y->ch[1], x->ch[1]);
                if (x->ch[0] != nullNode) x->ch[0]->fa = x;
                if (x->ch[1] != nullNode) x->ch[1]->fa = x;
                if (y->ch[0] != nullNode) y->ch[0]->fa = y;
                if (y->ch[1] != nullNode) y->ch[1]->fa = y;
            }
        }

        /**
         * erase the element at pos.
         *
         * throw if pos pointed to a bad element (pos == this->end() || pos points an element out of this)
         */
        void erase(iterator pos) {
            if (pos.bel != this) throw runtime_error();
            Node *rt = pos.now;
            if (rt == nullNode) throw runtime_error();
            const Key value = rt->val->first;
            Node *suc = rt;
            if (suc->ch[0] != nullNode && suc->ch[1] != nullNode) {
                suc = rt->ch[1];
                while (suc->ch[0] != nullNode) suc = suc->ch[0];
            }
            if (suc != rt) {
                if (root == rt)
                    root = suc;
                if (minn == rt)
                    minn = suc;
                if (maxx == rt)
                    maxx = suc;
                delete rt->val;
                rt->val = new value_type(*(suc->val));
                std::swap(rt->col, suc->col);
                swapNode(rt, suc);
                std::swap(rt, suc);
            }
            Node *suc_son = suc->ch[suc->ch[0] == nullNode];
            suc_son->fa = suc->fa;
            if (suc->fa == nullNode) root = suc_son;
            else suc->fa->ch[Dir(suc)] = suc_son;
            if (!suc->col) Remove_fix(suc_son);
            siz--;
            if (minn == nullNode || (!Compare()(minn->val->first, value))) {
                minn = root;
                while (minn->ch[0] != nullNode) minn = minn->ch[0];
            }
            if (maxx == nullNode || (!Compare()(value, maxx->val->first))) {
                maxx = root;
                while (maxx->ch[1] != nullNode) maxx = maxx->ch[1];
            }
            delete suc;
            suc = nullNode;
        }

        /**
         * Returns the number of elements with key
         *   that compares equivalent to the specified argument,
         *   which is either 1 or 0
         *     since this container does not allow duplicates.
         * The default method of check the equivalence is !(a < b || b > a)
         */
        size_t count(const Key &key) const {
            Node *rt = root;
            while (rt != nullNode) {
                value_type &now = *(rt->val);
                bool if_less = Compare()(key, now.first);
                bool if_more = Compare()(now.first, key);
                if (!if_less && !if_more) return 1;
                if (if_less) {
                    rt = rt->ch[0];
                } else rt = rt->ch[1];
            }
            return 0;
        }

        /**
         * Finds an element with key equivalent to key.
         * key value of the element to search for.
         * Iterator to an element with key equivalent to key.
         *   If no such element is found, past-the-end (see end()) iterator is returned.
         */
        iterator find(const Key &key) {
            //printf("find\n");
            Node *rt = root;
            while (rt != nullNode) {
                value_type &now = *(rt->val);
                bool if_less = Compare()(key, now.first);
                bool if_more = Compare()(now.first, key);
                if (!if_less && !if_more) return iterator(rt, this);
                if (if_less) {
                    rt = rt->ch[0];
                } else rt = rt->ch[1];
            }
            return iterator(rt, this);
        }

        const_iterator find(const Key &key) const {
            Node *rt = root;
            while (rt != nullNode) {
                value_type &now = *(rt->val);
                bool if_less = Compare()(key, now.first);
                bool if_more = Compare()(now.first, key);
                if (!if_less && !if_more) return const_iterator(rt, this);
                if (if_less) {
                    rt = rt->ch[0];
                } else rt = rt->ch[1];
            }
            return const_iterator(rt, this);
        }
    };

}

#endif
