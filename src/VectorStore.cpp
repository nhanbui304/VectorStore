#include "VectorStore.h"

// ----------------- ArrayList Implementation -----------------

template <class T>
void ArrayList<T>::ensureCapacity(int cap) {
  if (cap <= capacity) return;
  if (cap > capacity) {
    int newCapacity = capacity * 1.5;
    if (newCapacity == capacity) newCapacity++;
    if (newCapacity < cap) newCapacity = cap;

    T* newArr = new T[newCapacity];
    for (int i = 0; i < count; i++) {
      newArr[i] = data[i];
    }
    delete[] data;
    this->data = newArr;
    this->capacity = newCapacity;
  }
}

template <class T>
ArrayList<T>::ArrayList(int initCapacity) {
  this->capacity = initCapacity;
  this->count = 0;
  if (initCapacity < 0) {
    this->data = nullptr;
  } else {
    this->data = new T[capacity];
  }
}

template <class T>
ArrayList<T>::ArrayList(const ArrayList<T>& other) {
  this->capacity = other.capacity;
  this->count = other.count;
  this->data = new T[this->capacity];
  for (int i = 0; i < this->count; i++) {
    this->data[i] = other.data[i];
  }
}

template <class T>
ArrayList<T>::~ArrayList() {
  if (data != nullptr) {
    delete[] data;
    data = nullptr;
  }
  capacity = 0;
  count = 0;
}

// TODO: implement other methods of ArrayList
template <class T>
ArrayList<T>& ArrayList<T>::operator=(const ArrayList<T>& other) {
  if (this == &other) {
    return *this;
  }
  if (data != nullptr) {
    delete[] this->data;
    data = nullptr;
  }

  this->capacity = other.capacity;
  this->count = other.count;
  this->data = new T[this->capacity];
  for (int i = 0; i < this->count; i++) {
    this->data[i] = other.data[i];
  }
  return *this;
}

template <class T>
void ArrayList<T>::add(T e) {
  ensureCapacity(count + 1);
  data[count++] = e;
}

template <class T>
void ArrayList<T>::add(int index, T e) {
  if ((index > count) || (index < 0))
    throw std::out_of_range("Index is invalid!");
  ensureCapacity(count + 1);

  for (int i = count; i > index; i--) {
    data[i] = data[i - 1];
  }
  data[index] = e;
  count++;
}

template <class T>
T ArrayList<T>::removeAt(int index) {
  if ((index >= count) || (index < 0))
    throw std::out_of_range("Index is invalid!");
  T removedElement = data[index];
  for (int i = index; i < count - 1; i++) {
    data[i] = data[i + 1];
  }
  count--;
  return removedElement;
}

template <class T>
bool ArrayList<T>::empty() const {
  return count == 0;
}

template <class T>
int ArrayList<T>::size() const {
  return count;
}

template <class T>
void ArrayList<T>::clear() {
  delete[] data;
  data = nullptr;
  count = 0;
  capacity = 10;
  data = new T[capacity];
}

template <class T>
T& ArrayList<T>::get(int index) const {
  if ((index >= count) || (index < 0))
    throw std::out_of_range("Index is invalid!");
  return data[index];
}

template <class T>
void ArrayList<T>::set(int index, T e) {
  if ((index >= count) || (index < 0))
    throw std::out_of_range("Index is invalid!");
  data[index] = e;
}

template <class T>
int ArrayList<T>::indexOf(T item) const {
  for (int i = 0; i < count; i++) {
    if (data[i] == item) return i;
  }
  return -1;
}

template <class T>
bool ArrayList<T>::contains(T item) const {
  for (int i = 0; i < count; i++) {
    if (data[i] == item) return true;
  }
  return false;
}

template <class T>
std::string ArrayList<T>::toString(string (*item2str)(T&)) const {
  stringstream ss;
  ss << "[";
  for (int i = 0; i < count; i++) {
    if (item2str) {
      ss << item2str(data[i]);
    } else {
      ss << data[i];
    }
    if (i < count - 1) {
      ss << ", ";
    }
  }
  ss << "]";
  return ss.str();
}

template <class T>
typename ArrayList<T>::Iterator ArrayList<T>::begin() {
  return Iterator(this, 0);
}

template <class T>
typename ArrayList<T>::Iterator ArrayList<T>::end() {
  return Iterator(this, count);
}

// ----------------- Iterator of ArrayList Implementation -----------------
template <class T>
ArrayList<T>::Iterator::Iterator(ArrayList<T>* pList, int index) {
  if (index < 0 || index > pList->count || pList == nullptr) {
    throw out_of_range("Index is invalid!");
  }

  this->pList = pList;
  this->cursor = index;
}

template <class T>
typename ArrayList<T>::Iterator& ArrayList<T>::Iterator::operator=(
    const Iterator& other) {
  if (this != &other) {
    this->pList = other.pList;
    this->cursor = other.cursor;
  }
  return *this;
}

template <class T>
T& ArrayList<T>::Iterator::operator*() {
  if (cursor == pList->count) {
    throw out_of_range("Iterator is out of range!");
  }
  return pList->get(cursor);
}

template <class T>
bool ArrayList<T>::Iterator::operator!=(const Iterator& other) const {
  return ((this->cursor != other.cursor) || (this->pList != other.pList));
}

template <class T>
typename ArrayList<T>::Iterator& ArrayList<T>::Iterator::operator++() {
  if (cursor == pList->count) {
    throw out_of_range("Iterator cannot advance past end!");
  }
  cursor++;
  return *this;
}

template <class T>
typename ArrayList<T>::Iterator ArrayList<T>::Iterator::operator++(int) {
  Iterator temp = *this;
  ++(*this);
  return temp;
}

template <class T>
typename ArrayList<T>::Iterator& ArrayList<T>::Iterator::operator--() {
  if (cursor == 0) {
    throw out_of_range("Iterator cannot move before begin!");
  }
  cursor--;
  return *this;
}

template <class T>
typename ArrayList<T>::Iterator ArrayList<T>::Iterator::operator--(int) {
  Iterator temp = *this;
  --(*this);
  return temp;
}

// ----------------- SinglyLinkedList Implementation -----------------
template <class T>
SinglyLinkedList<T>::SinglyLinkedList() {
  head = nullptr;
  tail = nullptr;
  count = 0;
}

template <class T>
SinglyLinkedList<T>::~SinglyLinkedList() {
  clear();
}

template <class T>
void SinglyLinkedList<T>::add(T e) {
  Node* newNode = new Node(e, nullptr);
  if (head == nullptr) {
    head = newNode;
    tail = newNode;
  } else {
    tail->next = newNode;
    tail = newNode;
  }
  count++;
}

template <class T>
void SinglyLinkedList<T>::add(int index, T e) {
  if (index > count || index < 0)
    throw out_of_range("Index is invalid!");
  else if (index == 0) {
    Node* newNode = new Node(e, head);
    head = newNode;
    if (tail == nullptr) tail = newNode;
    count++;
  } else if (index == count) {
    add(e);
  } else {
    Node* current = head;
    for (int i = 0; i < index - 1; ++i) {
      current = current->next;
    }
    Node* newNode = new Node(e, current->next);
    current->next = newNode;
    count++;
  }
}

template <class T>
T SinglyLinkedList<T>::removeAt(int index) {
  if (index < 0 || index >= count) {
    throw std::out_of_range("Index is invalid!");
  }

  T dataToReturn;
  Node* nodeToDelete;

  if (index == 0) {
    nodeToDelete = head;
    dataToReturn = nodeToDelete->data;
    head = head->next;
    if (head == nullptr) {
      tail = nullptr;
    }
  } else {
    Node* prev = head;
    for (int i = 0; i < index - 1; i++) {
      prev = prev->next;
    }

    nodeToDelete = prev->next;
    dataToReturn = nodeToDelete->data;
    prev->next = nodeToDelete->next;
    if (prev->next == nullptr) {
      tail = prev;
    }
  }

  delete nodeToDelete;
  count--;
  return dataToReturn;
}

template <class T>
bool SinglyLinkedList<T>::removeItem(T item) {
  Node* current = head;
  int index = 0;
  while (current != nullptr) {
    if (current->data == item) {
      removeAt(index);
      return true;
    }
    current = current->next;
    index++;
  }
  return false;
}

template <class T>
bool SinglyLinkedList<T>::empty() const {
  return count == 0;
}

template <class T>
int SinglyLinkedList<T>::size() const {
  return count;
}

template <class T>
void SinglyLinkedList<T>::clear() {
  Node* current;
  for (int i = 0; i < count; ++i) {
    current = head;
    head = head->next;
    delete current;
  }
  count = 0;
  head = nullptr;
  tail = nullptr;
}

template <class T>
T& SinglyLinkedList<T>::get(int index) const {
  if (index < 0 || index >= count) throw out_of_range("Index is invalid!");

  Node* current = head;

  for (int i = 0; i < index; ++i) {
    current = current->next;
  }

  return current->data;
}

template <class T>
int SinglyLinkedList<T>::indexOf(T item) const {
  Node* current = head;

  for (int i = 0; i < count; ++i) {
    if (current->data == item) return i;
    current = current->next;
  }

  return -1;
}

template <class T>
bool SinglyLinkedList<T>::contains(T item) const {
  Node* current = head;

  for (int i = 0; i < count; ++i) {
    if (current->data == item) return true;
    current = current->next;
  }

  return false;
}

template <class T>
string SinglyLinkedList<T>::toString(string (*item2str)(T&)) const {
  if (head == nullptr) return "";
  stringstream ss;
  Node* current = head;
  for (int i = 0; i < count; ++i) {
    ss << "[";
    if (item2str) {
      ss << item2str(current->data);
    } else {
      ss << current->data;
    }
    ss << "]";
    if (i < count - 1) {
      ss << "->";
    }
    current = current->next;
  }
  return ss.str();
}

// ----------------- Iterator of SinglyLinkedList Implementation
// -----------------
template <class T>
SinglyLinkedList<T>::Iterator::Iterator(Node* node) {
  current = node;
}

template <class T>
typename SinglyLinkedList<T>::Iterator SinglyLinkedList<T>::begin() const {
  return Iterator(head);
}

template <class T>
typename SinglyLinkedList<T>::Iterator SinglyLinkedList<T>::end() const {
  return Iterator(nullptr);
}

template <class T>
typename SinglyLinkedList<T>::Iterator&
SinglyLinkedList<T>::Iterator::operator=(const Iterator& other) {
  if (this != &other) {
    this->current = other.current;
  }
  return *this;
}

template <class T>
T& SinglyLinkedList<T>::Iterator::operator*() {
  if (current == nullptr) {
    throw out_of_range("Iterator is out of range!");
  }
  return current->data;
}

template <class T>
bool SinglyLinkedList<T>::Iterator::operator!=(const Iterator& other) const {
  return current != other.current;
}

template <class T>
typename SinglyLinkedList<T>::Iterator&
SinglyLinkedList<T>::Iterator::operator++() {
  if (current == nullptr) {
    throw out_of_range("Iterator cannot advance past end!");
  }
  current = current->next;
  return *this;
}

template <class T>
typename SinglyLinkedList<T>::Iterator
SinglyLinkedList<T>::Iterator::operator++(int) {
  Iterator temp = *this;
  ++(*this);
  return temp;
}

// ----------------- VectorStore Implementation -----------------

VectorStore::VectorStore(
    int dimension,
    SinglyLinkedList<float>* (*embeddingFunction)(const string&)) {
  this->dimension = dimension;
  this->count = 0;
  this->embeddingFunction = embeddingFunction;
}

int VectorStore::nextId = 0;

VectorStore::~VectorStore() { clear(); }

VectorStore::VectorRecord::VectorRecord(int id, const string& rawText,
                                        SinglyLinkedList<float>* vector) {
  this->id = id;
  this->rawText = rawText;
  this->vector = vector;
}

int VectorStore::size() const { return this->count; }

bool VectorStore::empty() const { return this->count == 0; }

void VectorStore::clear() {
  for (int i = 0; i < records.size(); ++i) {
    VectorStore::VectorRecord* record = records.get(i);
    delete record->vector;
    delete record;
  }
  records.clear();
  this->count = 0;
}

SinglyLinkedList<float>* VectorStore::preprocessing(string rawText) {
  if (this->embeddingFunction == nullptr) {
    return nullptr;
  }

  SinglyLinkedList<float>* initialVector = this->embeddingFunction(rawText);

  while (initialVector->size() > this->dimension) {
    initialVector->removeAt(initialVector->size() - 1);
  }

  while (initialVector->size() < this->dimension) {
    initialVector->add(0.0f);
  }

  return initialVector;
}

void VectorStore::addText(string rawText) {
  SinglyLinkedList<float>* initialVector = preprocessing(rawText);
  VectorRecord* newRecord = new VectorRecord(nextId++, rawText, initialVector);
  records.add(newRecord);
  count++;
}

SinglyLinkedList<float>& VectorStore::getVector(int index) {
  if (index < 0 || index > count - 1) {
    throw out_of_range("Index is invalid!");
  }
  VectorRecord* record = records.get(index);
  return *(record->vector);
}

string VectorStore::getRawText(int index) const {
  if (index < 0 || index > count - 1) {
    throw out_of_range("Index is invalid!");
  }
  VectorRecord* record = records.get(index);
  return record->rawText;
}

int VectorStore::getId(int index) const {
  if (index < 0 || index > count - 1) {
    throw out_of_range("Index is invalid!");
  }
  VectorRecord* record = records.get(index);
  return record->id;
}

bool VectorStore::removeAt(int index) {
  if (index < 0 || index > count - 1) {
    throw out_of_range("Index is invalid!");
  }

  VectorRecord* recordToDelete = records.get(index);
  delete recordToDelete->vector;
  delete recordToDelete;
  records.removeAt(index);
  count--;
  return true;
}

bool VectorStore::updateText(int index, string newRawText) {
  if (index < 0 || index > count - 1) {
    throw out_of_range("Index is invalid!");
  }

  VectorRecord* record = records.get(index);
  delete record->vector;
  SinglyLinkedList<float>* newVector = preprocessing(newRawText);
  record->vector = newVector;
  record->rawText = newRawText;
  record->rawLength = newRawText.length();
  return true;
}

void VectorStore::setEmbeddingFunction(EmbedFn newEmbeddingFunction) {
  this->embeddingFunction = newEmbeddingFunction;
}

void VectorStore::forEach(void (*action)(SinglyLinkedList<float>&, int,
                                         string&)) {
  for (int i = 0; i < records.size(); ++i) {
    VectorRecord* record = records.get(i);
    action(*(record->vector), record->id, record->rawText);
  }
}

double VectorStore::cosineSimilarity(const SinglyLinkedList<float>& v1,
                                     const SinglyLinkedList<float>& v2) const {
  double dotProduct = 0.0;
  double Alength = 0.0;
  double Blength = 0.0;

  auto it1 = v1.begin();
  auto it2 = v2.begin();

  while (it1 != v1.end() && it2 != v2.end()) {
    float val1 = *it1;
    float val2 = *it2;

    dotProduct += val1 * val2;
    Alength += (val1 * val1);
    Blength += (val2 * val2);

    ++it1;
    ++it2;
  }

  Alength = sqrt(Alength);
  Blength = sqrt(Blength);
  if (Alength == 0 || Blength == 0) return 0.0;
  double result = dotProduct / (Alength * Blength);
  if (result > 1.0) result = 1.0;
  if (result < -1.0) result = -1.0;
  return result;
}

double VectorStore::l1Distance(const SinglyLinkedList<float>& v1,
                               const SinglyLinkedList<float>& v2) const {
  double res = 0.0;

  auto it1 = v1.begin();
  auto it2 = v2.begin();

  while (it1 != v1.end() && it2 != v2.end()) {
    float val1 = *it1;
    float val2 = *it2;

    res += fabs(val1 - val2);

    ++it1;
    ++it2;
  }
  return res;
}

double VectorStore::l2Distance(const SinglyLinkedList<float>& v1,
                               const SinglyLinkedList<float>& v2) const {
  double res = 0.0;

  auto it1 = v1.begin();
  auto it2 = v2.begin();

  while (it1 != v1.end() && it2 != v2.end()) {
    float val1 = *it1;
    float val2 = *it2;

    res += (val1 - val2) * (val1 - val2);

    ++it1;
    ++it2;
  }
  return sqrt(res);
}

// Hàm phụ so sánh
bool VectorStore::better(const RecordScore& a, const RecordScore& b,
                         const std::string& metric) {
  if (metric == "cosine") {
    if (fabs(a.score - b.score) < 1e-9) return a.index < b.index;
    return a.score > b.score;  // cosine: chọn lớn hơn
  } else {                     // manhattan, euclidean
    if (fabs(a.score - b.score) < 1e-9) return a.index < b.index;
    return a.score < b.score;  // distance: chọn nhỏ hơn
  }
}

int VectorStore::findNearest(const SinglyLinkedList<float>& query,
                             const string& metric) const {
  if (records.empty()) return -1;
  if (!(metric == "cosine" || metric == "manhattan" || metric == "euclidean")) {
    throw invalid_metric();
  }
  ArrayList<RecordScore> scores;

  // Tính tất cả score
  for (int i = 0; i < records.size(); ++i) {
    double score = 0.0;
    if (metric == "cosine") {
      score = cosineSimilarity(query, *(records.get(i)->vector));
    } else if (metric == "manhattan") {
      score = l1Distance(query, *(records.get(i)->vector));
    } else if (metric == "euclidean") {
      score = l2Distance(query, *(records.get(i)->vector));
    }
    scores.add({score, i});
  }

  // Tìm phần tử tốt nhất bằng so sánh
  int bestIndex = 0;
  for (int i = 1; i < scores.size(); ++i) {
    if (better(scores.get(i), scores.get(bestIndex), metric)) {
      bestIndex = i;
    }
  }

  return scores.get(bestIndex).index;
}

// Hàm phụ quickSort
void VectorStore::quickSort(ArrayList<RecordScore>& arr, int left, int right,
                            const std::string& metric) {
  int i = left, j = right;
  RecordScore pivot = arr.get((left + right) / 2);

  while (i <= j) {
    while (better(arr.get(i), pivot, metric)) i++;
    while (better(pivot, arr.get(j), metric)) j--;
    if (i <= j) {
      RecordScore tmp = arr.get(i);
      arr.set(i, arr.get(j));
      arr.set(j, tmp);
      i++;
      j--;
    }
  }
  if (left < j) quickSort(arr, left, j, metric);
  if (i < right) quickSort(arr, i, right, metric);
}

int* VectorStore::topKNearest(const SinglyLinkedList<float>& query, int k,
                              const string& metric) const {
  if (!(metric == "cosine" || metric == "manhattan" || metric == "euclidean")) {
    throw invalid_metric();
  }

  if (k <= 0 || k > records.size()) {
    throw invalid_k_value();
  }

  ArrayList<RecordScore> scores;

  // B1: Tính score cho từng record
  for (int i = 0; i < records.size(); ++i) {
    double score = 0.0;
    if (metric == "cosine") {
      score = cosineSimilarity(query, *(records.get(i)->vector));
    } else if (metric == "manhattan") {
      score = l1Distance(query, *(records.get(i)->vector));
    } else if (metric == "euclidean") {
      score = l2Distance(query, *(records.get(i)->vector));
    }
    scores.add({score, i});
  }

  // B2: QuickSort
  quickSort(scores, 0, scores.size() - 1, metric);

  // B3: Lấy k kết quả đầu tiên
  int* result = new int[k];
  for (int i = 0; i < k; ++i) {
    result[i] = scores.get(i).index;
  }
  return result;
}

// Explicit template instantiation for char, string, int, double, float, and
// Point

template class ArrayList<char>;
template class ArrayList<string>;
template class ArrayList<int>;
template class ArrayList<double>;
template class ArrayList<float>;
template class ArrayList<Point>;

template class SinglyLinkedList<char>;
template class SinglyLinkedList<string>;
template class SinglyLinkedList<int>;
template class SinglyLinkedList<double>;
template class SinglyLinkedList<float>;
template class SinglyLinkedList<Point>;