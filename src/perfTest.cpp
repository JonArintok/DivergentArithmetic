// g++ playingWithFnPtrs.cpp -o playingWithFnPtrs -std=c++11 -Wall -O3

#include <iostream>
using std::cout;
using std::endl;
#include <chrono>
#include "node_f.hpp"
#include "node_s.hpp"


typedef num (*op)(num, num);

num _add(num a, num b) {return a + b;}
num _sub(num a, num b) {return a - b;}
num _mul(num a, num b) {return a * b;}
num _div(num a, num b) {return a / b;}

enum switch_op {so_add, so_sub, so_mul, so_div};
num everyFn(switch_op op, num a, num b) {
  switch (op) {
    case so_add: return a+b;
    case so_sub: return a-b;
    case so_mul: return a*b;
    case so_div: return a/b;
  }
  return 0;
}


#define _checkData \
for (uint i = 0; i < dataSize; i++) {\
  if (data[i] != 100) {\
    cout << "incorrect data at " << i << ": " << data[i] << endl;\
    exit(1);\
  }\
  data[i] = 0;\
}


int main(int argc, char const **argv) {
  long dataSize = 1e6;
  num data[dataSize];
  cout << "dataSize: " << dataSize << endl;
  for (uint i = 0; i < dataSize; i++) {data[i] = 0;}
  
  //control
  {
  	auto start_time = std::chrono::high_resolution_clock::now();
    for (uint i = 0; i < dataSize; i++) {
      data[i] = _add(90, _sub(12, _div(8, _mul(2, 2))));
    }
  	auto end_time = std::chrono::high_resolution_clock::now();
    cout << "control: " <<
    std::chrono::duration_cast<std::chrono::microseconds>(
      end_time - start_time
    ).count() << " microseconds" << endl;
  }
  _checkData
  
  //every fn ptr per iteration
  {
    //the conditions are to prevent the compiler from inlining
    const op opAdd = argc > 1000 ? _sub : _add;
    const op opSub = argc > 1000 ? _mul : _sub;
    const op opMul = argc > 1000 ? _div : _mul;
    const op opDiv = argc > 1000 ? _add : _div;
  	auto start_time = std::chrono::high_resolution_clock::now();
    for (uint i = 0; i < dataSize; i++) {
      data[i] = opAdd(90, opSub(12, opDiv(8, opMul(2, 2))));
    }
  	auto end_time = std::chrono::high_resolution_clock::now();
    cout << "every fn ptr per iteration: " <<
    std::chrono::duration_cast<std::chrono::microseconds>(
      end_time - start_time
    ).count() << " microseconds" << endl;
  }
  _checkData
  
  //switch per iteration
  {
    const switch_op a = argc > 1000 ? so_sub : so_add;
    const switch_op s = argc > 1000 ? so_mul : so_sub;
    const switch_op m = argc > 1000 ? so_div : so_mul;
    const switch_op d = argc > 1000 ? so_add : so_div;
  	auto start_time = std::chrono::high_resolution_clock::now();
    for (uint i = 0; i < dataSize; i++) {
      data[i] = everyFn(a, 90, everyFn(s, 12, everyFn(d, 8, everyFn(m, 2, 2))));
    }
  	auto end_time = std::chrono::high_resolution_clock::now();
    cout << "switch per iteration: " <<
    std::chrono::duration_cast<std::chrono::microseconds>(
      end_time - start_time
    ).count() << " microseconds" << endl;
  }
  _checkData
  
  //one fn ptr per iteration
  {
    const op opAdd = argc > 1000 ? _sub : _add;
    const op opSub = argc > 1000 ? _mul : _sub;
    const op opMul = argc > 1000 ? _div : _mul;
    const op opDiv = argc > 1000 ? _add : _div;
  	auto start_time = std::chrono::high_resolution_clock::now();
    for (uint i = 0; i < dataSize; i++) {data[i] = opMul(2, 2);}
    for (uint i = 0; i < dataSize; i++) {data[i] = opDiv(8, data[i]);}
    for (uint i = 0; i < dataSize; i++) {data[i] = opSub(12, data[i]);}
    for (uint i = 0; i < dataSize; i++) {data[i] = opAdd(90, data[i]);}
  	auto end_time = std::chrono::high_resolution_clock::now();
    cout << "one fn ptr per iteration: " <<
    std::chrono::duration_cast<std::chrono::microseconds>(
      end_time - start_time
    ).count() << " microseconds" << endl;
  }
  _checkData
  
  //node_f
  {
    std::vector<node> mulArgs = {node( 2), node(2)};
    node muln = node(nf_mul, mulArgs);
    std::vector<node> divArgs = {node( 8), muln};
    node divn = node(nf_div, divArgs);
    std::vector<node> subArgs = {node(12), divn};
    node subn = node(nf_sub, subArgs);
    std::vector<node> addArgs = {node(90), subn};
    node addn = node(nf_add, addArgs);
    auto start_time = std::chrono::high_resolution_clock::now();
    addn.outputTo(data, dataSize);
  	auto end_time = std::chrono::high_resolution_clock::now();
    cout << "node_f: " <<
    std::chrono::duration_cast<std::chrono::microseconds>(
      end_time - start_time
    ).count() << " microseconds" << endl;
  }
  _checkData
  
  //node_s
  {
    std::vector<node_s> mulArgs = {node_s( 2), node_s(2)};
    node_s muln = node_s(ni_mul, mulArgs);
    std::vector<node_s> divArgs = {node_s( 8), muln};
    node_s divn = node_s(ni_div, divArgs);
    std::vector<node_s> subArgs = {node_s(12), divn};
    node_s subn = node_s(ni_sub, subArgs);
    std::vector<node_s> addArgs = {node_s(90), subn};
    node_s addn = node_s(ni_add, addArgs);
    auto start_time = std::chrono::high_resolution_clock::now();
    addn.outputTo(data, dataSize);
  	auto end_time = std::chrono::high_resolution_clock::now();
    cout << "node_s: " <<
    std::chrono::duration_cast<std::chrono::microseconds>(
      end_time - start_time
    ).count() << " microseconds" << endl;
  }
  _checkData
  
  return 0;
}

