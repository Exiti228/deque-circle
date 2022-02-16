#include <iostream>
#include <algorithm>
#include <exception>

template <typename T>
class cdeque_iterator {
private:
   T *_data;
   int _reserved;
   int _offset;
   int _pos;
public:
   cdeque_iterator(T* __data,int __reserved,int __offset,int __pos) :
      _data(__data), _reserved(__reserved), _offset(__offset), _pos(__pos) {}
   typedef std::random_access_iterator_tag iterator_category;
   typedef T value_type;
   typedef T difference_type;
   typedef T& reference;
   typedef T* pointer;
   cdeque_iterator<T>& operator++() {
      _pos = (_pos + 1) % _reserved;
      return *this;
   }
   cdeque_iterator<T>& operator--() {
      _pos = (_pos - 1 + _reserved) % _reserved;
      return *this;
   }
   T& operator*() {return *(_data + _pos);}

   bool operator==(const cdeque_iterator<T> &a) {
      return _data + _pos == a._data + a._pos;
   }
   bool operator!=(const cdeque_iterator<T> &a) {
      return _data + _pos!= a._data + a._pos;
   }
   bool operator<(const cdeque_iterator<T> &a) {
      int dist1 = (_pos - _offset + _reserved) % _reserved;
      int dist2 = (a._pos - a._offset + a._reserved) % a._reserved;
      return dist1 < dist2;
   }
   cdeque_iterator& operator+=(int k) {
       _pos = (_pos + k + _reserved) % _reserved;
       return *this;
   }
   cdeque_iterator operator+( int k) {
      cdeque_iterator<T> other(*this);
      return other+=k;
   }
   cdeque_iterator operator-(int k) {
      cdeque_iterator<T> other(*this);
      return other+=-k;
   }
   int operator-(const cdeque_iterator<T> &a) {
        return (_pos - a._pos + _reserved) % _reserved;
   }
};

template <typename T>
class cdeque {
public:
   int _reserved;
   int _size;
   int _offset;
   T* _data;
   void reallocate(int __reserve);

   cdeque(int _reserve) {
      if (_reserve<1)
         throw std::length_error("reserve must be >0");
      _reserved=_reserve;
      _size=0;
      _offset=0;
      _data=new T [_reserved];
   }
   ~cdeque() {
      delete[] _data;
   }
   int size() {
      return _size;
   }
   T& operator[](int i) {
      return _data[(_offset + i) % _reserved];
   }
   T& front() {
      return _data[_offset];
   }
   T& back() {
      return _data[(_offset + _size - 1) % _reserved];
   }
   auto begin() {
        return cdeque_iterator<T>(_data, _reserved, _offset, _offset);
   }
   auto end() {
       // std::cout << "---------------- " << (_offset + _size) % _reserved << std::endl;
        return cdeque_iterator<T>(_data, _reserved, _offset, (_offset + _size) % _reserved);
   }
   void push_back(const T &x) {
      if (_size+1==_reserved)
         reallocate(2*_reserved);
      _data[(_offset + _size++) % _reserved] = x;

   }
   void push_front(const T &x) {
      if (_size+1==_reserved)
         reallocate(2*_reserved);
      _data[(_offset-- - 1 + _reserved) % _reserved] = x;
      _offset = (_offset  + _reserved) % _reserved;
      _size++;
   }
   void pop_back() {
      _size--;
   }
   void pop_front() {
      _offset++;
      _offset = _offset % _reserved;
     // std::cout << _offset << std::endl;
      _size--;
   }
};
template <typename T>
void cdeque<T>::reallocate(int __reserve) {
    //std::cout << _offset << std::endl;
    T* up = new T[__reserve];
    for (int i = __reserve - _size; i < __reserve; ++i)
        up[i] = _data[(_offset + i - (__reserve - _size) ) % _reserved];
    delete[] _data;
    _data = up;
    _offset = __reserve - _size;

    _reserved = __reserve;
}

int main() {
  cdeque<int> d(1);
  for (int j=1;j<10;j++) {
     d.push_back(j);
     for (int i=0;i<d.size();i++)
        std::cout<<d[i]<<' ';
     std::cout<<std::endl;
  }
  for (int j=0;j<10;j++) {
     for (int i=0;i<d.size();i++)
        std::cout<<d[i]<<' ';
     std::cout<<std::endl;
     int x=d.front();
     d.pop_front();
     d.push_back(x);
  }
  for (int j=0;j<11;j++) {
     for (int i=0;i<d.size();i++)
        std::cout<<d[i]<<' ';
     std::cout<<std::endl;
     int x=d.back();
     d.pop_back();
     d.push_front(x);
  }
  cdeque<int> e(1);
  for (int j=1;j<=10;j++) {
     if (j&1)
        e.push_back(j);
     else
        e.push_front(j);
     for (int i=0;i<e.size();i++)
        std::cout<<e[i]<<' ';
     std::cout<<std::endl;
  }
  //std::cout << e._offset << " " << e._reserved << std::endl;
//  auto it = e.begin();
//  auto it2 = e.end();
//    for (auto c : e)
//        std::cout << c << std::endl;
 // std:: cout << *it;
  std::sort(e.begin(),e.end());

  for (int i=0;i<e.size();i++)
     std::cout<<e[i]<<' ';
  std::cout<<std::endl;
  return 0;
}
