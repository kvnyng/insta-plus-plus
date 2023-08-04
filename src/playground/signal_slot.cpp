#include <boost/signals2.hpp>
using namespace boost;
//#include <boost/bind.hpp>
#include <iostream>
//
//using namespace boost;
//using namespace std;
//
//struct ClassA
//{
//    boost::signals2::signal<void ()>    SigA;
//    boost::signals2::signal<void (int)> SigB;
//};
//
//struct ClassB
//{
//    void PrintFoo()      { cout << "Foo" << endl; }
//    void PrintInt(int i) { cout << "Bar: " << i << endl; }
//};
//
//int main()
//{
//    ClassA a;
//    ClassB b, b2;
//
//    a.SigA.connect(bind(&ClassB::PrintFoo, &b));
//    a.SigB.connect(bind(&ClassB::PrintInt, &b,  _1));
//    a.SigB.connect(bind(&ClassB::PrintInt, &b2, _1));
//
//    a.SigA();
//    a.SigB(4);
//}

void print_args(float x, float y)
{
    std::cout << "The arguments are " << x << " and " << y << std::endl;
}

void print_sum(float x, float y)
{
    std::cout << "The sum is " << x + y << std::endl;
}

void print_product(float x, float y)
{
    std::cout << "The product is " << x * y << std::endl;
}

void print_difference(float x, float y)
{
    std::cout << "The difference is " << x - y << std::endl;
}

void print_quotient(float x, float y)
{
    std::cout << "The quotient is " << x / y << std::endl;
}

int main() {
    boost::signals2::signal<void (float, float)> sig;

    sig.connect(&print_args);
    sig.connect(&print_sum);
    sig.connect(&print_product);
    sig.connect(&print_difference);
    sig.connect(&print_quotient);

    sig(5., 3.);
}