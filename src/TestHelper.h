#ifndef TESTHELPER_H
#define TESTHELPER_H

#include <fstream>
#include <sstream>
#include <vector>

#include "VectorStore.h"

class TestHelper {
 public:
  static SinglyLinkedList<float>* embedding(const string& text) {
    SinglyLinkedList<float>* vec = new SinglyLinkedList<float>();
    stringstream ss(text);
    string word;

    while (ss >> word) {
      int sum = 0;
      for (char c : word) {
        if (c >= 'a' && c <= 'z')
          sum += c - 'a' + 1;
        else if (c >= 'A' && c <= 'Z')
          sum -= c - 'A' + 1;
      }
      vec->add(sum);
    }

    return vec;
  }

  static SinglyLinkedList<float>* countCharsPerWord(const string& text) {
    SinglyLinkedList<float>* vec = new SinglyLinkedList<float>();
    stringstream ss(text);
    string word;

    while (ss >> word) {  // Split by spaces
      int count = 0;
      for (char c : word) {
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) count++;
      }
      vec->add(count);
    }

    return vec;
  }

  static SinglyLinkedList<float>* encode(const std::string& raw) {
    SinglyLinkedList<float>* vec = new SinglyLinkedList<float>();
    std::stringstream ss(raw);
    float value;
    while (ss >> value) {
      vec->add(value);
    }
    return vec;
  }

  static SinglyLinkedList<float>* readMNISTLine(const std::string& line) {
    SinglyLinkedList<float>* vec = new SinglyLinkedList<float>();
    std::stringstream ss(line);
    std::string value;

    while (std::getline(ss, value, ',')) {
      try {
        float num = std::stof(value);
        vec->add(num);
      } catch (...) {
        // bỏ qua nếu không phải số
      }
    }

    return vec;
  }

  static std::vector<std::string> readMNISTFile(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<std::string> lines;
    std::string line;

    if (!file.is_open()) {
      throw std::runtime_error("Cannot open file: " + filename);
    }

    bool firstLine = true;
    while (std::getline(file, line)) {
      if (firstLine) {
        firstLine = false;  // bỏ qua dòng đầu tiên
        continue;
      }

      if (!line.empty()) lines.push_back(line);
    }

    return lines;
  }
  static void resetNextId() { VectorStore::nextId = 0; }
};

#endif  // TESTHELPER_H
