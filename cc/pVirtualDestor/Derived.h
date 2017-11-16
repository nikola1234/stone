#pragma once

#include "Base.h"

class StdDerived : public StdBase {
public:
    StdDerived() { std::cout << "StdDerived constructor!" << std::endl; };
    ~StdDerived() { std::cout << "StdDerived destructor!" << std::endl; };

    void DoSomething() { std::cout << "Do something in class StdDerived!" << std::endl; };
};

class PureDerived : public PureBase{  // pure virtual destor class
  public:
   ~PureDerived() {};
};