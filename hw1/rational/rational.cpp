#include "rational.h"
#include <stdexcept>
#include <cmath>
#include <numeric>
// Libraries may be included as necessary

using namespace std;


Rational::Rational() : n(0), d(1) {}

Rational::Rational(int n, int d) {
    // Since the denominator cannot be zero an exception is thrown
    if (d == 0) {
        throw std::invalid_argument("division by zero");
    }

    // TODO: continue your implementation below.
    // You should likely set the member variables ad then call this->reduce()
    // and this->normalize().
    this->n=n;
    this->d=d;

        normalize();
        reduce();
}

void Rational::normalize() {
    // In C++ you don't always have to use this-> to denote a member variable.
    // If you omit it, the compiler will infer that n and d are indeed members
    // of Rational. There are two reasons you will see or use this->:
    //
    // 1. If you have a local variable that has the same name as a member
    //    variable, you need to use this-> to differentiate the two. If you
    //    created `int n` on this next line, the compiler would decide that
    //    any consequent uses of that variable refer to the local n, not our
    //    member variable. To specifically access the member n in this
    //    scenario, we'd use this->n.
    //
    // 2. Style. Some people (like me, Noah, the guy writing this) prefer to
    //    to use this-> to make it clear what variables are members when
    //    reading through the code. Others use a prefix (m_, i.e. m_member) or
    //    a suffix (_, i.e. member_) to denote it. In assignments where you
    //    write your own classes, this will be up to you. For now, do as you
    //    wish, but don't forget readability :)
    //
    // And feel free to delete this huge comment.
    if (n == 0 && d != 1) {
        d = 1;
    }
}

void Rational::reduce(){
    if(d < 0){
        d *= -1;
        n *= -1;
    }
    int a = gcd(n, d);
    n/=a;
    d/=a;
}

Rational Rational::operator+(const Rational& rhs) const {
    // TODO
    Rational newRat;
    newRat.d = lcm(d, rhs.d);
    newRat.n = (n*(newRat.d/d))+(rhs.n*(newRat.d/rhs.d));
    if(newRat.n == 0){
        newRat.normalize();
    }
    newRat.reduce();
    return newRat;
}

Rational Rational::operator+(int rhs) const{
    Rational newRat;
    newRat.d = d;
    newRat.n = n+(rhs*d);
    if(newRat.n == 0){
        newRat.normalize();
    }
    newRat.reduce();
    return newRat;
}

Rational Rational::operator *(int rhs) const{
    Rational newRat;
    newRat.d = d;
    newRat.n = n * rhs;
    if(newRat.n == 0){
        newRat.normalize();
    }
    newRat.reduce();
    return newRat;
}

Rational& Rational::operator*=(int rhs) {
    n*=rhs;
    if(n==0){
        normalize();
    }
    reduce();
    return *this;
}

Rational& Rational::operator+=(int rhs) {
    // TODO
    n += (rhs*d);
    normalize();
    reduce();
    return *this;
}

Rational& Rational::operator*=(const Rational& rhs){
    n*=rhs.n;
    d*=rhs.d;
    normalize();
    reduce();
    return *this;
}

Rational Rational::operator*(const Rational& rhs) const {
    // TODO
    Rational newRat;
    newRat.n = n*rhs.n;
    newRat.d = d*rhs.d;
    newRat.normalize();
    newRat.reduce();
    return newRat;
}

Rational& Rational::operator+=(const Rational& rhs){
    int newD = lcm(d, rhs.d);
    n = (n*(newD/d))+(rhs.n*(newD/rhs.d));
    d = newD;
    normalize();
    reduce();
    return *this;
}

// This operator isn't prefixed by Rational:: because it's not actually a
// member of Rational. It just tells the compiler what to do when it sees
// multiplication between an int on the left and a Rational on the right. Our
// friend declaration simply enabled us to access private members on the const
// Rational& rhs in the function body.
//
// Note that we could implement any of the other operators above this same way
// using the corresponding global operator and passing a const Rational& lhs as
// the first argument, it's just a matter of convenience.
 Rational operator*(int lhs, const Rational& rhs) {
    // TODO
    Rational newRat(lhs, 1);
    newRat.n *= rhs.n;
    newRat.d *= rhs.d;
    if(newRat.n == 0){
        newRat.normalize();
    }
    newRat.reduce();
    return newRat;
}

Rational operator+(int lhs, const Rational& rhs){
    Rational newRat(lhs*rhs.d, rhs.d);
    newRat.n *= rhs.d;
    newRat.d *= rhs.d;
    newRat.n += rhs.n;
    newRat.normalize();
    newRat.reduce();
    return newRat;
}

bool Rational::operator==(const Rational& other) const {
    // TODO
    return (n == other.n && d== other.d);
}

bool Rational::operator!=(const Rational& other) const {
    return !(n == other.n && d== other.d);
}

bool Rational::operator<(const Rational &other) const {
    int newD = lcm(d, other.d);
    int n1 = n * (newD/d);
    int n2 = other.n * (newD/other.d);
    return (n1 < n2);
}

Rational Rational::operator^(const int power) const{
    Rational newRat;
    if(power == 0){
        newRat.n = 1;
        newRat.d = 1;
    }
    if(power < 0){
        newRat.n = pow(d, -power);
        newRat.d = pow(n, -power);
    }
    else{
        newRat.n = pow(n, power);
        newRat.d = pow(d, power);
    }
    return newRat;
}

std::istream& operator>>(std::istream& in, Rational& r){
    char c = '/';
    in >>r.n>> c >>r.d;
    r.normalize();
    r.reduce();
    return in;
}

std::ostream& operator<<(std::ostream& out, const Rational& r){
    out<< r.n << '/' << r.d;
    return out;
}



// TODO: implement the rest of your prototyped methods
