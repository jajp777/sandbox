#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <cmath>

// #pragma once

#include <math.h>
#include <stdint.h>
#include <limits.h>

#include <vector>
#include <algorithm>

class Num {
public:
    typedef uint64_t word;

    std::vector<word> words;
    bool neg;

    static word word_mask(){
        return (word)-1;
    }

    static size_t word_bits(){
        return sizeof(word)*CHAR_BIT;
    }

    static word word_half_mask(){
        return word_mask() >> word_bits()/2;
    }

    static word char_to_word(char c){
        switch (c){
            case '0': return 0;
            case '1': return 1;
            case '2': return 2;
            case '3': return 3;
            case '4': return 4;
            case '5': return 5;
            case '6': return 6;
            case '7': return 7;
            case '8': return 8;
            case '9': return 9;
            case 'a': case 'A': return 10;
            case 'b': case 'B': return 11;
            case 'c': case 'C': return 12;
            case 'd': case 'D': return 13;
            case 'e': case 'E': return 14;
            case 'f': case 'F': return 15;
            case 'g': case 'G': return 16;
            case 'h': case 'H': return 17;
            case 'i': case 'I': return 18;
            case 'j': case 'J': return 19;
            case 'k': case 'K': return 20;
            case 'l': case 'L': return 21;
            case 'm': case 'M': return 22;
            case 'n': case 'N': return 23;
            case 'o': case 'O': return 24;
            case 'p': case 'P': return 25;
            case 'q': case 'Q': return 26;
            case 'r': case 'R': return 27;
            case 's': case 'S': return 28;
            case 't': case 'T': return 29;
            case 'u': case 'U': return 30;
            case 'v': case 'V': return 31;
            case 'w': case 'W': return 32;
            case 'x': case 'X': return 33;
            case 'y': case 'Y': return 34;
            case 'z': case 'Z': return 35;
            default: return word_mask();
        }
    }

    static word word_gcd(word a, word b){
        while (1){
            if (a == 0) return b;
            b %= a;
            if (b == 0) return a;
            a %= b;
        }
    }

    Num(): neg(false){}

    Num(size_t n, word w, bool neg = false): words(n, w), neg(neg){}

    Num(const word *a, const word *b, bool neg = false) : words(a, b), neg(neg){}

    Num(const Num &a){
        words = a.words;
        neg = a.neg;
    }

    Num& operator = (const Num &a){
        words = a.words;
        neg = a.neg;
        return *this;
    }

    Num(int i): neg(i < 0){
        for (unsigned u = std::abs(i); u;){
            push_back(u);
            for (size_t j = 0; j < word_bits(); j++) u >>= 1;
        }
    }

    Num(const char *c, word base = 10, char **endptr = NULL): neg(false){
        if (*c == '-'){
            c++;
            neg = true;
        }
        for (;*c; c++){
            mul_word(base);
            word b = char_to_word(*c);
            if (b >= base) break;
            add_word(b);
        }
        if (endptr) *endptr = (char*)c;
    }

    void resize(size_t n){
        words.resize(n);
    }

    void pop_back(){
        words.pop_back();
    }

    void push_back(word b){
        words.push_back(b);
    }

    word& back(){
        return words.back();
    }

    const word& back() const {
        return words.back();
    }

    size_t size() const {
        return words.size();
    }

    word& operator [] (size_t i){
        return words[i];
    }

    const word& operator [] (size_t i) const {
        return words[i];
    }

    Num& set_neg(bool neg){
        this->neg = neg;
        return *this;
    }

    Num& truncate(){
        while (size() > 0 && words.back() == 0) pop_back();
        return *this;
    }

    size_t bitlength() const {
        if (size() == 0) return 0;
        size_t last = size() - 1;
        size_t result = word_bitlength((*this)[last]) + last*word_bits();
        return result;
    }

    size_t count_trailing_zeros() const {
        for (size_t i = 0; i < size(); i++){
            word w = (*this)[i];
            if (w) return word_count_trailing_zeros(w) + i*word_bits();
        }
        return 0;
    }

    static int cmp_abs(const Num &a, const Num &b){
        size_t na = a.size(), nb = b.size();
        if (na != nb){
            return na < nb ? -1 : +1;
        }
        for (size_t i = na; i --> 0;){
            word wa = a[i], wb = b[i];
            if (wa != wb){
                return wa < wb ? -1 : +1;
            }
        }
        return 0;
    }

    static int cmp(const Num &a, const Num &b){
        if (a.size() == 0 && b.size() == 0) return 0;
        if (!a.neg && !b.neg) return +cmp_abs(a, b);
        if ( a.neg &&  b.neg) return -cmp_abs(a, b);
        return a.neg && !b.neg ? -1 : +1;
    }

    static size_t word_bitlength(word a){
        for (int i = word_bits() - 1; i >= 0; i--) if ((a >> i) & 1) return i+1;
        return 0;
    }

    static size_t word_count_trailing_zeros(word a){
        for (int i = 0; i < (int)word_bits(); i++) if ((a >> i) & 1) return i;
        return word_bits();
    }

    static word add_carry(word *a, word b){
        return (*a += b) < b;
    }

    static word sub_carry(word *a, word b){
        word tmp = *a;
        return (*a = tmp - b) > tmp;
    }

    static word word_mul_hi(word a, word b){
        size_t n = word_bits()/2;
        word a_hi = a >> n;
        word a_lo = a & word_half_mask();
        word b_hi = b >> n;
        word b_lo = b & word_half_mask();
        word tmp = ((a_lo*b_lo) >> n) + a_hi*b_lo;
        tmp = (tmp >> n) + ((a_lo*b_hi + (tmp & word_half_mask())) >> n);
        return tmp + a_hi*b_hi;
    }

    static Num& add_unsigned_overwrite(Num &a, const Num &b){
        size_t i, na = a.size(), nb = b.size(), n = std::max(na, nb);
        a.resize(n);
        word carry = 0;
        for (i = 0; i < nb; i++){
            carry  = add_carry(&a[i], carry);
            carry += add_carry(&a[i], b[i]);
        }
        for (; i < n && carry; i++) carry = add_carry(&a[i], carry);
        if (carry) a.push_back(carry);
        return a.truncate();
    }

    static Num& sub_unsigned_overwrite(Num &a, const Num &b){
        //assert(cmp_abs(a, b) >= 0);
        size_t i, na = a.size(), nb = b.size();
        word carry = 0;
        for (i = 0; i < nb; i++){
            carry  = sub_carry(&a[i], carry);
            carry += sub_carry(&a[i], b[i]);
        }
        for (; i < na && carry; i++) carry = sub_carry(&a[i], carry);
        //assert(!carry);
        return a.truncate();
    }

    static Num mul_long(const Num &a, const Num &b){
        size_t na = a.size(), nb = b.size(), nc = na + nb + 1;
        Num c(nc, 0, a.neg ^ b.neg), carries(nc, 0);
        for (size_t ia = 0; ia < na; ia++){
            for (size_t ib = 0; ib < nb; ib++){
                size_t i = ia + ib, j = i + 1;
                // WARNING: Might overflow if word size is chosen too small
                carries[i + 1] += add_carry(&c[i], a[ia] * b[ib]);
                carries[j + 1] += add_carry(&c[j], word_mul_hi(a[ia], b[ib]));
            }
        }
        return add_unsigned_overwrite(c, carries).truncate();
    }

    static Num mul_karatsuba(const Num &a, const Num &b){
        size_t na = a.size(), nb = b.size(), n = std::max(na, nb), m2 = n/2 + (n & 1);
        Num a_parts[2], b_parts[2];
        split(a, a_parts, 2, m2);
        split(b, b_parts, 2, m2);
        m2 *= word_bits();
        Num z0 = a_parts[0] * b_parts[0];
        Num z1 = (a_parts[0] + a_parts[1]) * (b_parts[0] + b_parts[1]);
        Num z2 = a_parts[1] * b_parts[1];
        Num result = z2;
        result <<= m2;
        result += z1 - z2 - z0;
        result <<= m2;
        result += z0;
        return result;
    }

    static Num mul(const Num &a, const Num &b){
        size_t karatsuba_threshold = 20;
        if (a.size() > karatsuba_threshold && b.size() > karatsuba_threshold){
            return mul_karatsuba(a, b);
        }
        return mul_long(a, b);
    }

    static Num add_signed(const Num &a, bool a_neg, const Num &b, bool b_neg){
        if (a_neg == b_neg) return add_unsigned(a, b).set_neg(a_neg);
        if (cmp_abs(a, b) >= 0) return sub_unsigned(a, b).set_neg(a_neg);
        return sub_unsigned(b, a).set_neg(b_neg);
    }

    Num& operator >>= (size_t n_bits){
        if (n_bits == 0) return *this;
        size_t n_words = n_bits / word_bits();
        if (n_words >= size()){
            resize(0);
            return *this;
        }
        n_bits %= word_bits();
        if (n_bits == 0){
            for (size_t i = 0; i < size() - n_words; i++){
                (*this)[i] = (*this)[i + n_words];
            }
        }else{
            word hi, lo = (*this)[n_words];
            for (size_t i = 0; i < size() - n_words - 1; i++){
                hi = (*this)[i + n_words + 1];
                (*this)[i] = (hi << (word_bits() - n_bits)) | (lo >> n_bits);
                lo = hi;
            }
            (*this)[size() - n_words - 1] = lo >> n_bits;
        }
        resize(size() - n_words);
        return truncate();
    }

    Num& operator <<= (size_t n_bits){
        if (n_bits == 0) return *this;
        size_t n_words = n_bits / word_bits();
        n_bits %= word_bits();
        size_t old_size = size();
        size_t n = old_size + n_words + (n_bits != 0);
        resize(n);
        if (n_bits == 0){
            for (size_t i = n; i --> n_words;){
                (*this)[i] = (*this)[i - n_words];
            }
        }else{
            word lo, hi = 0;
            for (size_t i = n - 1; i > n_words; i--){
                lo = (*this)[i - n_words - 1];
                (*this)[i] = (hi << n_bits) | (lo >> (word_bits() - n_bits));
                hi = lo;
            }
            (*this)[n_words] = hi << n_bits;
        }
        for (size_t i = 0; i < n_words; i++) (*this)[i] = 0;
        return truncate();
    }

    static void div_mod(const Num &numerator, Num denominator, Num &quotient, Num &remainder){
        quotient = 0;
        remainder = numerator;
        if (cmp_abs(remainder, denominator) >= 0){
            int n = numerator.bitlength() - denominator.bitlength();
            denominator <<= n;
            for (; n >= 0; n--){
                if (cmp_abs(remainder, denominator) >= 0){
                    sub_unsigned_overwrite(remainder, denominator);
                    quotient.set_bit(n);
                }
                denominator >>= 1;
            }
        }
        quotient.set_neg(numerator.neg ^ denominator.neg);
        remainder.set_neg(numerator.neg);
    }

    static void div_mod_half_word(const Num &numerator, word denominator, Num &quotient, word &remainder){
        remainder = 0;
        Num dst(numerator.size(), 0);

        for (size_t i = numerator.size(); i --> 0;){
            word dst_word = 0;
            word src_word = numerator[i];
            word parts[2];
            parts[0] = src_word >> word_bits()/2;
            parts[1] = src_word & word_half_mask();

            for (size_t j = 0; j < 2; j++){
                remainder <<= word_bits()/2;
                remainder |= parts[j];

                word div_word = remainder / denominator;
                word mod_word = remainder % denominator;
                remainder = mod_word;

                dst_word <<= word_bits()/2;
                dst_word |= div_word;
            }

            dst[i] = dst_word;
        }

        quotient = dst.truncate().set_neg(numerator.neg);
    }

    static void split(const Num &a, Num *parts, size_t n_parts, size_t n){
        size_t i = 0;
        for (size_t k = 0; k < n_parts; k++){
            Num &part = parts[k];
            part.resize(n);
            for (size_t j = 0; j < n && i < a.size(); j++) part[j] = a[i++];
            part = part.truncate();
        }
    }

    static Num div(const Num &numerator, const Num& denominator){
        Num quotient, remainder;
        div_mod(numerator, denominator, quotient, remainder);
        return quotient;
    }

    static Num mod(const Num &numerator, const Num& denominator){
        Num quotient, remainder;
        div_mod(numerator, denominator, quotient, remainder);
        return remainder;
    }

    static Num add_unsigned(const Num &a, const Num &b){
        Num result(a);
        return add_unsigned_overwrite(result, b);
    }

    static Num sub_unsigned(const Num &a, const Num &b){
        Num result(a);
        return sub_unsigned_overwrite(result, b);
    }

    static Num add(const Num &a, const Num &b){
        Num result = add_signed(a, a.neg, b, b.neg);
        return result;
    }

    static Num sub(const Num &a, const Num &b){
        Num result = add_signed(a, a.neg, b, !b.neg);
        return result;
    }

    static Num gcd(const Num &a0, const Num &b0){

        if (a0.size() == 1 && b0.size() == 1){
            return Num(1, word_gcd(a0[0], b0[0]));
        }

        Num a(a0), b(b0);
        a.neg = b.neg = false;

        if (a.size() == 0) return b0;
        if (b.size() == 0) return a0;

        size_t n = std::min(a.count_trailing_zeros(), b.count_trailing_zeros());

        a >>= n;
        b >>= n;

        do {
            b >>= b.count_trailing_zeros();
            if (cmp_abs(a, b) > 0) a.words.swap(b.words);
            sub_unsigned_overwrite(b, a);
        } while (b.size() > 0);

        a <<= n;

        return a;
    }

    typedef void (*random_func)(uint8_t *bytes, size_t n_bytes);

    static Num random_bits(size_t n_bits, random_func func){
        if (n_bits == 0) return 0;
        size_t partial_bits = n_bits % word_bits();
        size_t n_words = n_bits / word_bits() + (partial_bits > 0);
        size_t n_bytes = n_words*sizeof(word);
        Num result(n_words, 0);
        uint8_t *bytes = (uint8_t*)&result[0];
        func(bytes, n_bytes);
        if (partial_bits){
            size_t too_many_bits = word_bits() - partial_bits;
            result.back() >>= too_many_bits;
        }
        return result;
    }

    static Num random_inclusive(const Num &inclusive, random_func func){
        size_t n_bits = inclusive.bitlength();
        while (true){
            Num result = random_bits(n_bits, func);
            if (result <= inclusive) return result;
        }
    }

    static Num random_exclusive(const Num &exclusive, random_func func){
        size_t n_bits = exclusive.bitlength();
        while (true){
            Num result = random_bits(n_bits, func);
            if (result < exclusive) return result;
        }
    }

    static Num random_second_exclusive(const Num &inclusive_min_val, const Num &exclusive_max_val, random_func func){
        return inclusive_min_val + random_exclusive(exclusive_max_val - inclusive_min_val, func);
    }

    static Num random_both_inclusive(const Num &inclusive_min_val, const Num &inclusive_max_val, random_func func){
        return inclusive_min_val + random_inclusive(inclusive_max_val - inclusive_min_val, func);
    }

    Num& set_bit(size_t i){
        size_t i_word = i / word_bits();
        size_t i_bit =  i % word_bits();
        if (size() <= i_word) resize(i_word + 1);
        (*this)[i_word] |= ((word)1) << i_bit;
        return *this;
    }

    word get_bit(size_t i) const {
        size_t i_word = i / word_bits();
        size_t i_bit =  i % word_bits();
        if (i_word >= size()) return 0;
        return ((*this)[i_word] >> i_bit) & 1;
    }

    void clr_bit(size_t i){
        size_t i_word = i / word_bits();
        size_t i_bit =  i % word_bits();
        if (i_word >= size()) return;
        word mask = 1;
        mask <<= i_bit;
        (*this)[i_word] &= ~mask;
    }

    Num& mul_word(word b){
        word carry = 0;
        for (size_t i = 0; i < size(); i++){
            word a = (*this)[i];
            word tmp = a * b;
            carry = add_carry(&tmp, carry);
            carry += word_mul_hi(a, b);
            (*this)[i] = tmp;
        }
        if (carry) push_back(carry);
        return truncate();
    }

    Num& add_word(word carry, size_t i0 = 0){
        if (i0 >= size()) resize(i0 + 1);
        for (size_t i = i0; i < size() && carry; i++){
            carry = add_carry(&(*this)[i], carry);
        }
        if (carry) push_back(carry);
        return truncate();
    }

    void print(
        std::vector<char> &text,
        word base = 10,
        const char *alphabet = "0123456789abcdefghijklmnopqrstuvwxyz"
    ) const {
        if (size() == 0){
            text.push_back('0');
        }else{
            Num tmp(*this);
            while (tmp.size() > 0){
                word remainder;
                div_mod_half_word(tmp, base, tmp, remainder);
                text.push_back(alphabet[remainder]);
            }
            if (neg) text.push_back('-');
            std::reverse(text.begin(), text.end());
        }
        text.push_back('\0');
    }

    double to_double() const {
        if (size() == 0) return 0.0;
        double d = 0.0, base = ::pow(2.0, word_bits());
        for (size_t i = size(); i --> 0;) d = d * base + (*this)[i];
        return neg ? -d : d;
    }

    Num pow(size_t exponent) const {
        Num result(1), p(*this);
        for (; exponent; exponent >>= 1){
            if (exponent & 1){
                result = result * p;
                exponent--;
            }
            p = p*p;
        }
        return result;
    }

    Num mod_pow(Num exponent, const Num &modulus) const {
        Num result(1), base = (*this) % modulus;
        for (; exponent.size() > 0; exponent >>= 1){
            if (exponent.get_bit(0)){
                result = (result * base) % modulus;
            }
            base = (base * base) % modulus;
        }
        return result;
    }

    Num sqrt() const {
        Num n = *this;
        int bit = bitlength();
        if (bit & 1) bit ^= 1;
        Num result = 0;
        for (; bit >= 0; bit -= 2){
            Num tmp = result;
            tmp.set_bit(bit);
            if (n >= tmp){
                n -= tmp;
                result.set_bit(bit + 1);
            }
            result >>= 1;
        }
        return result;
    }

    Num& operator ++(){
        add_word(1);
        return *this;
    }

    Num& operator += (const Num &b){ return *this = add(*this, b); }
    Num& operator -= (const Num &b){ return *this = sub(*this, b); }
    Num& operator *= (const Num &b){ return *this = mul(*this, b); }
    Num& operator /= (const Num &b){ return *this = div(*this, b); }
    Num& operator %= (const Num &b){ return *this = mod(*this, b); }

    bool operator == (const Num &b) const { return cmp(*this, b) == 0; }
    bool operator != (const Num &b) const { return cmp(*this, b) != 0; }
    bool operator <= (const Num &b) const { return cmp(*this, b) <= 0; }
    bool operator >= (const Num &b) const { return cmp(*this, b) >= 0; }
    bool operator <  (const Num &b) const { return cmp(*this, b) <  0; }
    bool operator >  (const Num &b) const { return cmp(*this, b) >  0; }

    Num operator + (const Num &b) const { return add(*this, b); }
    Num operator - (const Num &b) const { return sub(*this, b); }
    Num operator * (const Num &b) const { return mul(*this, b); }
    Num operator / (const Num &b) const { return div(*this, b); }
    Num operator % (const Num &b) const { return mod(*this, b); }
    Num operator - (            ) const { return Num(*this).set_neg(!neg); }

    Num operator >> (size_t n_bits) const { return Num(*this) >>= n_bits; }
    Num operator << (size_t n_bits) const { return Num(*this) <<= n_bits; }
};

using INTTYPE = unsigned long long;
//using INTTYPE = Num;
const bool NEEDMOD = false;

template <size_t p, size_t a>
INTTYPE polinomial_hash_family(const std::string& x, int beginv = 0, int endv = 0)
{
    if(endv == 0)
    {
        endv = x.size();
    }
    // static const size_t multiplier = 263;
    // static const size_t prime = 1000000007;
    INTTYPE hash = 0;
    for (int i = (endv - 1); i >= beginv; --i)
    {
        hash = (hash * a + x[i]);// % p;
        if(NEEDMOD) hash = hash % p;
    }
    return hash;
}

INTTYPE phash(const std::string& str, int begin = 0, int end = 0)
{
    return polinomial_hash_family<1000000007, 263>(str, begin, end);
}

//https://stackoverflow.com/questions/20412405/rolling-hash-in-rabin-karp
INTTYPE shiftleft_phash(const std::string& str, INTTYPE currenthash, int newbegin, int newend, INTTYPE xsizemodp = 0)
{
    INTTYPE p = 1000000007;
    INTTYPE m = 263;
    
    INTTYPE r = (currenthash * m);
    if(NEEDMOD) r = r % p;

    INTTYPE remove = ((INTTYPE)str[newend] * xsizemodp);
    if(NEEDMOD) remove = remove % p;
    r = r - remove;
    if(NEEDMOD) r = r % p;

    INTTYPE add  = (INTTYPE)str[newbegin];
    r = r + add;
    if(NEEDMOD) r = r % p;

    return r;
}

class RabinKarp
{
public:
    RabinKarp(bool uh) : useHash(uh)
    {
    }

    std::vector<int> Run(const std::string& text, const std::string& pattern) const
    {
        // std::cout << "Text " << text << " Pattern " << pattern << std::endl;

        auto result = std::vector<int>();
        auto patternHash = phash(pattern);

        auto hashes = computeHashTable(text, pattern);

        auto last = (text.size() - pattern.size()) + 1;
        for(int i = 0; i < last;++i)
        {
            auto ihash = hashes[i];

            // std::cout << "index " << i << " " << patternHash << " " << ihash;
            
            if((useHash)&&(ihash != patternHash))
            {
                // std::cout << std::endl;
                 continue;
            }

            // std::cout << " hashequal";

            if(AreEqual(text, pattern, i))
            {
                // std::cout << " and equal" << std::endl;
                result.push_back(i);
            }
            else
            {
                // std::cout << " but different " << std::endl;
            }
        }

        return result;
    }
private:
    bool useHash;

    std::vector<INTTYPE> computeHashTable(const std::string& text, const std::string& pattern) const
    {
        auto psize = pattern.size();
        bool firsthash = true;
        INTTYPE hash = 0;

        auto resultsize = text.size() - pattern.size() + 1;
        auto result = std::vector<INTTYPE>(resultsize);

        INTTYPE ypower = 1;
        for(int i = 1; i <= psize; ++i)
        {
            ypower = (ypower * 263);
            if(NEEDMOD) ypower = ypower % 1000000007;
        }

        for(int i = resultsize - 1; i >= 0; --i)
        {
            if(firsthash)
            {
                hash = phash(text, i, i + psize);
                firsthash = false;
            }
            else
            {
                hash = shiftleft_phash(text, hash, i, i + psize, ypower);
            }

            result[i] = hash;
        }

        return result;
    }

    //Can be elegantly implemented as reduce(logicalAnd, zip(text,pattern, testEquality))
    bool AreEqual(const std::string& text, const std::string& pattern, int i) const
    {
        bool result = true;

        for(int j = i, jj = 0; jj < pattern.size(); ++j,++jj)
        {
            result = result && (text[j] == pattern[jj]);
        }

        return result;
    }
};


using std::string;
typedef unsigned long long ull;

struct Data {
    string pattern, text;
};

Data read_input(std::istream& in) {
    Data data;
    in >> data.pattern >> data.text;
    return data;
}

void print_occurrences(std::ostream& out, const std::vector<int>& output) {
    for (size_t i = 0; i < output.size(); ++i)
        out << output[i] << " ";
    out << "\n";
}

std::vector<int> get_occurrences(const Data& input, bool useHash) {
    // const string& s = input.pattern, t = input.text;
    // std::vector<int> ans;
    // for (size_t i = 0; i + s.size() <= t.size(); ++i)
    //     if (t.substr(i, s.size()) == s)
    //         ans.push_back(i);
    // return ans;
    const string& s = input.pattern, t = input.text;
    auto sk = RabinKarp(useHash);
    return sk.Run(t, s);
}


void run(std::istream& in, std::ostream& out, bool useHash)
{
    print_occurrences(out, get_occurrences(read_input(in), useHash));
}

#ifdef UNITTESTS

#define CATCH_CONFIG_MAIN
#include "../../catch.hpp"

void test(const std::string& inStr, const std::string& expectedOutStr)
{
    auto instream = std::stringstream{inStr};
    auto expectedOutStream = std::stringstream{expectedOutStr};

    auto actualOutStream = std::stringstream{};

    run(instream, actualOutStream, true);
    actualOutStream.seekg(0);

    std::string actual, expected;
    while(!expectedOutStream.eof())
    {
        actualOutStream >> actual;
        expectedOutStream >> expected;

        REQUIRE(expected == actual);
    }
}

TEST_CASE("","")
{
    // auto powers = std::vector<unsigned long long>(53);
    // unsigned long long ypower = 1;
    // powers[0] = 1;
    // for(int i = 1; i <= 52; ++i)
    // {
    //     ypower = (ypower * 263) % 1000000007;
    //     powers[i] = ypower;
    // }

    // // auto h1 = ('e'*powers[0] + 's'*powers[1] + 't'*powers[2] + 't'*powers[3]) % 1000000007;
    // auto h1 = phash("oYhXmlwOscxnkTWjsyNJNhgvzMFbxFnbiWuBAGjZQlCRQHjTUXua");
    // auto h2 = shiftleft_phash("lNoYhXmlwOscxnkTWjsyNJNhgvzMFbxFnbiWuBAGjZQlCRQHjTUXua", h1, 1, 53);
    // auto h3 = shiftleft_phash("lNoYhXmlwOscxnkTWjsyNJNhgvzMFbxFnbiWuBAGjZQlCRQHjTUXua", h2, 0, 52);

    // auto expectedh2 = phash("NoYhXmlwOscxnkTWjsyNJNhgvzMFbxFnbiWuBAGjZQlCRQHjTUXu");
    // auto expectedh3 = phash("lNoYhXmlwOscxnkTWjsyNJNhgvzMFbxFnbiWuBAGjZQlCRQHjTUX");

    // std::cout << h1 << std::endl;
    // std::cout << expectedh2 << " " << h2 << std::endl;;
    // std::cout << expectedh3 << " " << h3 << std::endl;;

    // auto h3 = shiftleft_phash("lNoYhXmlwOscxnkTWjsyNJNhgvzMFbxFnbiWuBAGjZQlCRQHjTUXu", h1, 0, 52);
    
    // // std::cout << "mh1 " << mh1 << " h1 " << h1 << " " << (mh1 == h1) << std::endl;

    // auto s = (263*h1) % 1000000007;
    // auto diff = ('u'*powers[52]) % 1000000007;
    // auto mh2 = (s + 'l'*powers[0] - diff) % 1000000007;

    // std::cout << "h1 " << h1 << " and h2 " << h2 << " and mh2 " << mh2 << " and h3 " << h3 << std::endl;
    // auto dmh2 = ('t'*powers[0] + 'e'*powers[1] + 's'*powers[2] + 't'*powers[3]) % 1000000007;
    // auto dh2 = ;

    // auto oldchar = 't'*powers[3] % 1000000007;
    // auto shmh2 = ((h1 - oldchar) * 263 ) % 1000000007 + 't';

    //  std::cout << "dh2 " << dh2 << " shmh2 " << shmh2 << std::endl;

    
    // REQUIRE(h3 == h2);
    // test("a a", "0");
    // test("aba abacaba", "0 4");
    // test("Test testTesttesT", "4");
    // test("aaaaa baaaaaaa", "1 2 3");
    // test("lNoYhXmlwOscxnkTWjsyNJNhgvzMFbxFnbiWuBAGjZQlCRQHjTUX lNoYhXmlwOscxnkTWjsyNJNhgvzMFbxFnbiWuBAGjZQlCRQHjTUXua", "0");
     test("lNoYhXmlwOscxnkTWjsyNJNhgvzMFbxFnbiWuBAGjZQlCRQHjTUX ZtonpqnFzlpvUKZrBbRlNoYhXmlwOscxnkTWjsyNJNhgvzMFbxFnbiWuBAGjZQlCRQHjTUXxtHmTxoLuMbRYsvSpxhtrlvABBlFYmndFzHypOmJyFxjHEPlNoYhXmlwOscxnkTWjsyNJNhgvzMFbxFnbiWuBAGjZQlCRQHjTUXbDiEAvtPlNoYhXmlwOscxnkTWjsyNJNhgvzMFbxFnbiWuBAGjZQlCRQHjTUXRRNoBCUMQVOlNoYhXmlwOscxnkTWjsyNJNhgvzMFbxFnbiWuBAGjZQlCRQHjTUXRLKlNoYhXmlwOscxnkTWjsyNJNhgvzMFbxFnbiWuBAGjZQlCRQHjTUXAYPDKWtVpShhclNoYhXmlwOscxnkTWjsyNJNhgvzMFbxFnbiWuBAGjZQlCRQHjTUXOJlUlNoYhXmlwOscxnkTWjsyNJNhgvzMFbxFnbiWuBAGjZQlCRQHjTUXglmlNoYhXmlwOscxnkTWjsyNJNhgvzMFbxFnbiWuBAGjZQlCRQHjTUXuaOibGlVrwghvNTgLfltIbEdBlgjelFjQkBeFrdEV",
         "19 118 178 241 296 361 417 472 ");
}

TEST_CASE("random","")
{
    
}

#else

int main() {
    run(std::cin, std::cout, true);
    return 0;
}

#endif
