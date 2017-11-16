#include <iostream>
#include <vector>

// 迭代器失效
int main ()
{
  std::vector<int> myvector;

  // set some values (from 1 to 10)
  for (int i=1; i<=10; i++) myvector.push_back(i);

  std::cout << "myvector contains:";
  for (unsigned i=0; i<myvector.size(); ++i)
    std::cout << ' ' << myvector[i];
  std::cout << '\n';

  for (auto it = myvector.begin(); it != myvector.end(); /* it++ 在函数体中实现 */)
  {
    if (*it == 5) 
    {
      // erase input  : iterator
      //       return : next iterator
      it = myvector.erase(it);
      std::cout << "erase : " << *it << std::endl;
    } 
    else 
    {
      it++;
    }
  }

  std::cout << "myvector contains:";
  for (unsigned i=0; i<myvector.size(); ++i)
    std::cout << ' ' << myvector[i];
  std::cout << '\n';

  for (auto it = myvector.begin(); it != myvector.end(); /* it++ 在函数体中实现 */)
  {
    if (*it == 6) 
    {
      // insert input  :      iterator
      //       return  : slef iterator
      it = myvector.insert(it, 11);
      // ietor(6)   next
      // insert   ietor+1(6)  next
      it =it + 2;  // for next +2
      std::cout << "insert : " << *it << std::endl;
    }
    else
    {
      it++;
    }
  }

  std::cout << "myvector contains:";
  for (unsigned i=0; i<myvector.size(); ++i)
    std::cout << ' ' << myvector[i];
  std::cout << '\n';

  return 0;
}