#include "Derived.h"
#include <memory>   // std::unique_ptr

int main () 
{
  StdDerived * stdder = new StdDerived();
  stdder->DoSomething();
  delete stdder;
  stdder = nullptr;

  std::unique_ptr<PureBase> p (new PureDerived());

}