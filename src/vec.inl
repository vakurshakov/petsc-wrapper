#include "vec.h"

namespace Petsc {

template<bool isConst>
Vec::BasicBorrowedArray<isConst>::BasicBorrowedArray(VecRef vec, GetArrayType type)
    : vec(vec), type(type) {
  if constexpr (isConst) {
    PetscCallThrow(VecGetArrayRead(vec, &array));
  }
  else if (type == Write) {
    PetscCallThrow(VecGetArrayWrite(vec, &array));
  }
  else {
    PetscCallThrow(VecGetArray(vec, &array));
  }
}

template<bool isConst>
void Vec::BasicBorrowedArray<isConst>::Restore() {
  if constexpr (isConst) {
    PetscCallThrow(VecRestoreArrayRead(vec, &array));
  }
  else if (type == Write) {
    PetscCallThrow(VecRestoreArrayWrite(vec, &array));
  }
  else {
    PetscCallThrow(VecRestoreArray(vec, &array));
  }
}

template<bool isConst>
Vec::BasicBorrowedArray<isConst>::Iterator
Vec::BasicBorrowedArray<isConst>::begin() {
  return Iterator(*this, 0);
}

template<bool isConst>
Vec::BasicBorrowedArray<isConst>::Iterator
Vec::BasicBorrowedArray<isConst>::end() {
  Int localSize = vec.LocalSize();
  return Iterator(*this, localSize);
}

template<bool isConst>
Vec::BasicBorrowedArray<isConst>::ConstIterator
Vec::BasicBorrowedArray<isConst>::begin() const {
  return ConstIterator(*this, 0);
}

template<bool isConst>
Vec::BasicBorrowedArray<isConst>::ConstIterator
Vec::BasicBorrowedArray<isConst>::end() const {
  Int localSize = vec.LocalSize();
  return ConstIterator(*this, localSize);
}

template<bool isConst>
Vec::BasicBorrowedArray<isConst>::~BasicBorrowedArray() noexcept(false) {
  Restore();
}

template<bool arrConst>
template<bool iterConst>
Vec::BasicBorrowedArray<arrConst>::BasicIterator<iterConst>::
BasicIterator(Vec::BasicBorrowedArray<arrConst>& array, Int current)
  : array(array), current(current) {}

template<bool arrConst>
template<bool iterConst>
Int Vec::BasicBorrowedArray<arrConst>::BasicIterator<iterConst>::index() const {
  return current;
}

template<bool arrConst>
template<bool iterConst>
Vec::BasicBorrowedArray<arrConst>::BasicIterator<iterConst>::Reference
Vec::BasicBorrowedArray<arrConst>::BasicIterator<iterConst>::value() {
  return array[current];
}

template<bool arrConst>
template<bool iterConst>
Vec::BasicBorrowedArray<arrConst>::BasicIterator<iterConst>::Reference
Vec::BasicBorrowedArray<arrConst>::BasicIterator<iterConst>::operator*() {
  return array[current];
}

template<bool arrConst>
template<bool iterConst>
Vec::BasicBorrowedArray<arrConst>::BasicIterator<iterConst>::Pointer
Vec::BasicBorrowedArray<arrConst>::BasicIterator<iterConst>::operator->() {
  return &array[current];
}

template<bool arrConst>
template<bool iterConst>
Vec::BasicBorrowedArray<arrConst>::BasicIterator<iterConst>&
Vec::BasicBorrowedArray<arrConst>::BasicIterator<iterConst>::operator++() {
  current++;
  return *this;
}

template<bool arrConst>
template<bool iterConst>
bool Vec::BasicBorrowedArray<arrConst>::BasicIterator<iterConst>::operator!=(const BasicIterator& other) const {
  return current != other.current;
}

template<bool arrConst>
template<bool iterConst>
Vec::BasicBorrowedArray<arrConst>::BasicIterator<iterConst>&
Vec::BasicBorrowedArray<arrConst>::BasicIterator<iterConst>::operator+=(Int difference) {
  current += difference;
  return *this;
}

template<bool arrConst>
template<bool iterConst>
Int Vec::BasicBorrowedArray<arrConst>::BasicIterator<iterConst>::operator-(const BasicIterator& other) const {
  return current - other.current;
}

}
