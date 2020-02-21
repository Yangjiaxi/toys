/******************************************************************************
 *                    JavaScript style Array class                            *
 *                           Just for fun!                                    *
 *                             Enjoy it!                                      *
 *                                                                            *
 *  @file     Array.h                                                         *
 *  @brief    JS like Array class                                             *
 *                                                                            *
 *  @author   Yang Jiaxi                                                      *
 *  @email    1468764755@qq.con                                               *
 *  @version  1.0.0                                                           *
 *  @date     2018.02.26                                                      *
 *  @license  MIT                                                             *
 *                                                                            *
 *----------------------------------------------------------------------------*
 */

#include <sstream>
#include <string>
#include <vector>

/**
 * @beief JavaScript风格的Array类\n
 * js中内置的Array类上的操作与c++中基于迭代器的处理方式完全不同，
 * c++中使用迭代器进行范围处理，而js的Array操作普遍都作用于所有元素，
 * 例如map, forEach方法
 */
template <class T> class Array {
    using iterator = typename std::vector<T>::iterator;

  public:
    std::vector<T> data;

    /*
        构造：
            Array() -> 默认构造
            Array(length, init_value) -> 返回length个init_value初始化的Array
            Array({element0, ...elementN}) -> 返回由列表初始化的Array
            Array(Array<T>) -> 浅拷贝构造
            Array(iterator l, iterator r) -> 范围迭代器初始化

     */
  public:
    Array() : data() {}
    Array(int length, T init_value) : data(length, init_value) {}
    Array(std::initializer_list<T> il) : data(il) {}
    Array(std::vector<T> v) : data(v) {}
    Array(iterator l, iterator r) : data(l, r) {}
    Array(const Array &B) : data(B.data) {}

    /*
        操作符重载
    */
    T &operator[](size_t idx) { return data[idx]; }

    /*
        方法：
            A<T>
    */

    // .pop() -> T
    // 删除最后一个元素并返回这个元素
    T pop();

    // .push(std::initializer_list<T>) -> this
    // 添加多个元素到尾部
    Array<T> &push(std::initializer_list<T>);

    // .push(T) -> this
    // 添加一个元素到尾部
    Array<T> &push(T);

    // .shift() -> T
    // 删除第一个元素并返回这个元素
    T shift();

    // .unshift(std::initializer_list<T>) -> this
    // 添加多个元素到头部
    Array<T> &unshift(std::initializer_list<T>);

    // .unshift(T) -> this
    // 添加一个元素到头部
    Array<T> &unshift(T);

    // .every(std::function<bool(T)>) -> bool
    // 测试所有元素是否都通过了指定函数的测试
    bool every(std::function<bool(T)>);

    // .some(std::function<bool(T)>) -> bool
    // 测试是否至少有一个元素通过测试
    bool some(std::function<bool(T)>);

    // .find(std::function<bool(T)>) -> T
    // 返回指向第一个通过测试的元素的拷贝，否则nullptr
    T *find(std::function<bool(T)>);

    // .findIndex(std::function<bool(T)) -> int
    // 返回第一个通过测试的元素的索引，否则-1
    int findIndex(std::function<bool(T)>);

    // .includes(T) -> bool
    // 返回是否包含某个特定值
    bool includes(T &);
    bool includes(T &&);

    // .indexOf(T[, fromIndex=0]) -> int
    // 返回可以找到的第一个给定元素的索引，否则-1
    int indexOf(T &);
    int indexOf(T &&);
    int indexOf(T &, size_t);
    int indexOf(T &&, size_t);

    // .lastIndexOf(T[, fromIndex=.length-1]) -> int
    // 从末尾开始，返回可以找到的第一个给定元素的索引，否则-1
    int lastIndexOf(T &);
    int lastIndexOf(T &&);
    int lastIndexOf(T &, size_t);
    int lastIndexOf(T &&, size_t);

    // .copyWithin(target[, start[, end]]) -> this
    // 将[start, end)覆盖到target开始的区域，inplace
    Array<T> &copyWithin(size_t);
    Array<T> &copyWithin(size_t, size_t);
    Array<T> &copyWithin(size_t, size_t, size_t);

    // .fill(value[, start[, end]]) -> this
    // 用value填充[start...end-1]，inplace
    Array<T> &fill(T);
    Array<T> &fill(T, size_t);
    Array<T> &fill(T, size_t, size_t);

    // .reverse() -> this
    // 倒序Array
    Array<T> &reverse();

    // .sort(std::function<bool(T, T)>) -> this
    // 原地升序排序
    Array<T> &sort();
    Array<T> &sort(std::function<bool(T, T)>);

    // .splice(start[, deleteCount[, {item1...itemN}]]) -> this
    // 从start开始删除deleteCount个元素，然后插入一堆元素，为空则仅删除，原地执行
    Array<T> &splice(size_t);
    Array<T> &splice(size_t, size_t);
    Array<T> &splice(size_t, size_t, std::initializer_list<T>);

    // .from() -> Array<T>
    // 返回浅拷贝副本
    Array<T> from();

    // .concat(Array<T> B) -> Array<T>
    // 返回A#B，是一个新的Array
    Array<T> concat(Array<T> &B);

    // .filter(std::function<bool(T)>) -> Array<T>
    // 返回通过测试的元素构成的Array
    Array<T> filter(std::function<bool(T)>);

    // .map<std::function<U(T)>> -> Array<U>
    // 返回对所有元素应用某个函数后的新Array
    template <class U> Array<U> map(std::function<U(T)>);

    // .slice([begin[, end]]) -> Array<T>
    // 从begin提取到end，不包含end，默认为[0, n)
    Array<T> slice();
    Array<T> slice(size_t);
    Array<T> slice(size_t, size_t);

    // .reduce(std::function<U(U, T)>, init_value) -> U
    // 从init_value开始对数组中元素升序执行reduce
    template <class U> U reduce(std::function<U(U, T)>, U);

    // .reduceRight(std::function<T(T, T)>, init_value) -> T
    // 从init_value开始对数组中元素升序执行reduce
    template <class U> U reduceRight(std::function<U(U, T)>, U);

    // .forEach(std::function<void(T)>)
    // 对数组中所有元素应用
    void forEach(std::function<void(T)>);

    // .join(spt=',') -> string
    std::string join();
    std::string join(std::string spt);
    std::ostream &output(std::ostream &os);

    // .size() -> size_t
    // 返回元素个数
    size_t size();
};

template <class T> T Array<T>::pop() {
    auto val = data.back();
    data.pop_back();
    return val;
}

template <class T> Array<T> &Array<T>::push(std::initializer_list<T> il) {
    data.insert(data.end(), il);
    return *this;
}

template <class T> Array<T> &Array<T>::push(T e) {
    data.insert(data.end(), e);
    return *this;
}

template <class T> T Array<T>::shift() {
    auto val = data.front();
    data.erase(data.begin());
    return val;
}

template <class T> Array<T> &Array<T>::unshift(std::initializer_list<T> il) {
    data.insert(data.begin(), il);
    return *this;
}

template <class T> Array<T> &Array<T>::unshift(T e) {
    data.insert(data.begin(), e);
    return *this;
}

template <class T> std::string Array<T>::join() { return join(", "); }

template <class T> std::string Array<T>::join(std::string spt) {
    std::ostringstream res;
    for (size_t i = 0; i < size() - 1; ++i) res << data[i] << spt;
    res << data[size() - 1];
    return res.str();
}

template <class T> std::ostream &Array<T>::output(std::ostream &os) {
    std::ostringstream res;
    res << "[" << join() << "]";
    os << res.str() << std::endl;
    return os;
}

template <class T> size_t Array<T>::size() { return data.size(); }

template <class T> bool Array<T>::every(std::function<bool(T)> fn) {
    for (T &el : data)
        if (!fn(el)) return false;
    return true;
}

template <class T> bool Array<T>::some(std::function<bool(T)> fn) {
    for (T &el : data)
        if (fn(el)) return true;
    return false;
}

template <class T> T *Array<T>::find(std::function<bool(T)> fn) {
    for (T &el : data)
        if (fn(el)) return &el;
    return nullptr;
}

template <class T> int Array<T>::findIndex(std::function<bool(T)> fn) {
    for (int i = 0; i < size(); ++i)
        if (fn(data[i])) return i;
    return -1;
}

template <class T> bool Array<T>::includes(T &target) {
    for (T &el : data)
        if (el == target) return true;
    return false;
}

template <class T> bool Array<T>::includes(T &&target) { return includes(target); }

template <class T> int Array<T>::indexOf(T &t) { return indexOf(t, 0); }

template <class T> int Array<T>::indexOf(T &&t) { return indexOf(t); }

template <class T> int Array<T>::indexOf(T &t, size_t b) {
    for (int i = b; i < size(); ++i)
        if (data[i] == t) return i;
    return -1;
}

template <class T> int Array<T>::indexOf(T &&t, size_t b) { return indexOf(t, b); }

template <class T> int Array<T>::lastIndexOf(T &t) { return lastIndexOf(t, size() - 1); }

template <class T> int Array<T>::lastIndexOf(T &&t) { return lastIndexOf(t); }

template <class T> int Array<T>::lastIndexOf(T &t, size_t b) {
    for (int i = b; i >= 0; --i)
        if (data[i] == t) return i;
    return -1;
}

template <class T> int Array<T>::lastIndexOf(T &&t, size_t b) { return lastIndexOf(t, b); }

template <class T> Array<T> &Array<T>::copyWithin(size_t target) {
    return copyWithin(target, 0, size());
}

template <class T> Array<T> &Array<T>::copyWithin(size_t target, size_t start) {
    return copyWithin(target, start, size());
}

template <class T> Array<T> &Array<T>::copyWithin(size_t target, size_t start, size_t end) {
    std::vector<T> tmp(data.begin() + start, data.begin() + end);
    for (size_t i = 0, j = target; i < tmp.size() && j < end; ++i, ++j) data[j] = tmp[i];
    return *this;
}

template <class T> Array<T> &Array<T>::fill(T t) { return fill(t, 0, size()); }

template <class T> Array<T> &Array<T>::fill(T t, size_t start) { return fill(t, start, size()); }

template <class T> Array<T> &Array<T>::fill(T t, size_t start, size_t end) {
    if (size() < end) data.resize(end);
    for (int i = start; i < end; ++i) data[i] = t;
    return *this;
}

template <class T> Array<T> &Array<T>::reverse() {
    auto l = data.begin();
    auto r = data.end();
    while ((l != r) && l != --r) {
        std::iter_swap(l, r);
        ++l;
    }
    return *this;
}

template <class T> Array<T> &Array<T>::sort() {
    return sort([](const T l, const T r) { return (l < r); });
}

template <class T> Array<T> &Array<T>::sort(std::function<bool(T, T)> cmp) {
    std::sort(data.begin(), data.end(), cmp);
    return *this;
}

template <class T> Array<T> &Array<T>::splice(size_t start) {
    return splice(start, size() - start, {});
}

template <class T> Array<T> &Array<T>::splice(size_t start, size_t deleteCount) {
    return splice(start, deleteCount, {});
}

template <class T>
Array<T> &Array<T>::splice(size_t start, size_t deleteCount, std::initializer_list<T> il) {
    size_t l = il.size();

    if (l > 0) data.insert(data.begin() + start, il);

    data.erase(data.begin() + start + l, data.begin() + start + l + deleteCount);
    return *this;
}

template <class T> Array<T> Array<T>::from() {
    Array<T> tmp(*this);
    return std::move(tmp);
}

template <class T> Array<T> Array<T>::concat(Array<T> &B) {
    Array tmp(*this);
    tmp.data.insert(tmp.data.end(), B.data.begin(), B.data.end());
    return std::move(tmp);
}

template <class T> Array<T> Array<T>::filter(std::function<bool(T)> fn) {
    Array<T> tmp;
    for (auto el : data)
        if (fn(el)) tmp.push(el);
    return std::move(tmp);
}

template <class T> template <class U> Array<U> Array<T>::map(std::function<U(T)> fn) {
    Array<U> tmp;
    for (auto el : data) tmp.push(fn(el));
    return std::move(tmp);
}

template <class T> Array<T> Array<T>::slice() { return slice(0, size()); }

template <class T> Array<T> Array<T>::slice(size_t start) { return slice(start, size()); }

template <class T> Array<T> Array<T>::slice(size_t start, size_t end) {
    iterator l = data.begin() + start;
    iterator r = data.begin() + end;
    Array<T> tmp(l, r);
    return std::move(tmp);
}

template <class T> template <class U> U Array<T>::reduce(std::function<U(U, T)> fn, U init_value) {
    U c = init_value;
    for (auto el : data) c = fn(c, el);
    return std::move(c);
}

template <class T>
template <class U>
U Array<T>::reduceRight(std::function<U(U, T)> fn, U init_value) {
    U c = init_value;
    for (int i = size() - 1; i >= 0; --i) c = fn(c, data[i]);
    return std::move(c);
}

template <class T> void Array<T>::forEach(std::function<void(T)> fn) {
    for (auto el : data) fn(el);
}