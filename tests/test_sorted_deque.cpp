#include "SortedDeque.h"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>

struct SubScore{ std::string label; SubScore(std::string L=""):label(std::move(L)){}
    int passed=0;
    int total=0;
    void ok(bool cond){
        total++;
        if(cond){ passed++; std::cout << "PASS: " << label << " case " << total << "\n"; }
        else    { std::cout << "FAIL: " << label << " case " << total << "\n"; }
    }
    std::string str() const {
        std::ostringstream os; os<<passed<<"/"<<total; return os.str();
    }
};

static bool eqvec(const std::vector<int>& a, const std::vector<int>& b){
    if(a.size()!=b.size()) return false;
    for(size_t i=0;i<a.size();++i) if(a[i]!=b[i]) return false;
    return true;
}

int main(){
    SubScore s1("SUB1"), s2("SUB2"), s3("SUB3"), s4("SUB4"), s5("SUB5");

    // ---------- SUB1: insert ----------
    {
        SortedDeque d;
        d.insert(3); d.insert(1); d.insert(2);
        s1.ok(eqvec(d.to_vector(), {1,2,3}));
    }
    {
        SortedDeque d;
        for(int x: std::vector<int>{5,4,3,2,1}) d.insert(x);
        s1.ok(eqvec(d.to_vector(), {1,2,3,4,5}));
    }
    {
        SortedDeque d;
        for(int x: std::vector<int>{2,2,2,2}) d.insert(x);
        s1.ok(eqvec(d.to_vector(), {2,2,2,2}));
    }
    {
        SortedDeque d;
        for(int x: std::vector<int>{1,3,5,7,9,0,2,4,6,8}) d.insert(x);
        s1.ok(eqvec(d.to_vector(), {0,1,2,3,4,5,6,7,8,9}));
    }
    {
        SortedDeque d;
        d.insert(10);
        s1.ok(eqvec(d.to_vector(), {10}));
    }

    // ---------- SUB2: erase ----------
    {
        SortedDeque d;
        for(int x: std::vector<int>{1,2,2,3,4}) d.insert(x);
        bool removed = d.erase(2);
        s2.ok(removed);
        s2.ok(eqvec(d.to_vector(), {1,2,3,4}));
    }
    {
        SortedDeque d;
        for(int x: std::vector<int>{1,1,1}) d.insert(x);
        bool removed = d.erase(1);
        s2.ok(removed);
        s2.ok(eqvec(d.to_vector(), {1,1}));
    }
    {
        SortedDeque d;
        for(int x: std::vector<int>{2,4,6}) d.insert(x);
        bool removed = d.erase(5);
        s2.ok(!removed);
        s2.ok(eqvec(d.to_vector(), {2,4,6}));
    }
    {
        SortedDeque d;
        for(int x: std::vector<int>{1}) d.insert(x);
        bool removed = d.erase(1);
        s2.ok(removed);
        s2.ok(eqvec(d.to_vector(), {}));
    }
    {
        SortedDeque d;
        for(int x: std::vector<int>{1,2,3,4,5}) d.insert(x);
        bool removed = d.erase(5);
        s2.ok(removed);
        s2.ok(eqvec(d.to_vector(), {1,2,3,4}));
    }

    
    // ---------- SUB3: merge ----------
    {
        SortedDeque a,b;
        for(int x: std::vector<int>{1,3,5}) a.insert(x);
        for(int x: std::vector<int>{2,4,6}) b.insert(x);
        a.merge(b);
        s3.ok(eqvec(a.to_vector(), {1,2,3,4,5,6}));
        s3.ok(eqvec(b.to_vector(), {2,4,6})); // unchanged
    }
    {
        SortedDeque a,b;
        for(int x: std::vector<int>{}) a.insert(x); // none
        for(int x: std::vector<int>{1,2,3}) b.insert(x);
        a.merge(b);
        s3.ok(eqvec(a.to_vector(), {1,2,3}));
        s3.ok(eqvec(b.to_vector(), {1,2,3}));
    }
    {
        SortedDeque a,b;
        for(int x: std::vector<int>{1,1,1}) a.insert(x);
        for(int x: std::vector<int>{1,1}) b.insert(x);
        a.merge(b);
        s3.ok(eqvec(a.to_vector(), {1,1,1,1,1}));
        s3.ok(eqvec(b.to_vector(), {1,1}));
    }
    {
        SortedDeque a,b;
        for(int x: std::vector<int>{0,10,20}) a.insert(x);
        for(int x: std::vector<int>{5,15,25}) b.insert(x);
        a.merge(b);
        s3.ok(eqvec(a.to_vector(), {0,5,10,15,20,25}));
    }
    {
        SortedDeque a,b;
        for(int x: std::vector<int>{1,2,100}) a.insert(x);
        for(int x: std::vector<int>{3,4,5,6,7,8,9}) b.insert(x);
        a.merge(b);
        s3.ok(eqvec(a.to_vector(), {1,2,3,4,5,6,7,8,9,100}));
    }

    // ---------- SUB4: kth_smallest ----------
    {
        SortedDeque d;
        for(int x: std::vector<int>{1,2,3,4,5}) d.insert(x);
        s4.ok(d.kth_smallest(1)==1);
        s4.ok(d.kth_smallest(5)==5);
    }
    {
        SortedDeque d;
        for(int x: std::vector<int>{2,2,2,3,3}) d.insert(x);
        s4.ok(d.kth_smallest(3)==2);
        s4.ok(d.kth_smallest(5)==3);
    }
    {
        SortedDeque d;
        for(int x: std::vector<int>{9}) d.insert(x);
        bool caught=false;
        try{ d.kth_smallest(0); } catch(const std::out_of_range&){ caught=true; }
        s4.ok(caught);
    }
    {
        SortedDeque d;
        for(int x: std::vector<int>{1,2}) d.insert(x);
        bool caught=false;
        try{ d.kth_smallest(3); } catch(const std::out_of_range&){ caught=true; }
        s4.ok(caught);
    }
    {
        SortedDeque d;
        for(int x: std::vector<int>{-5,-1,0,7}) d.insert(x);
        s4.ok(d.kth_smallest(2)==-1);
    }
// ---------- SUB5: to_vector ----------
    {
        SortedDeque d;
        s5.ok(d.to_vector().empty());
    }
    {
        SortedDeque d; for(int x: std::vector<int>{3,1,2}) d.insert(x);
        s5.ok(eqvec(d.to_vector(), {1,2,3}));
    }
    {
        SortedDeque d; for(int x: std::vector<int>{5,4,3,2,1,0}) d.insert(x);
        s5.ok(eqvec(d.to_vector(), {0,1,2,3,4,5}));
    }
    {
        SortedDeque d; for(int x: std::vector<int>{2,2,2}) d.insert(x);
        s5.ok(eqvec(d.to_vector(), {2,2,2}));
    }
    {
        SortedDeque d; for(int x: std::vector<int>{-1, -1, 0, 10}) d.insert(x);
        auto v = d.to_vector();
        bool nondec=true;
        for(size_t i=1;i<v.size();++i) if(v[i]<v[i-1]) nondec=false;
        s5.ok(nondec);
    }

    // ---- Print human summary ----
    int totalPassed = s1.passed+s2.passed+s3.passed+s4.passed+s5.passed;
    int totalTests  = s1.total +s2.total +s3.total +s4.total +s5.total;

    std::cout << "SUB1 insert: " << s1.str() << "\n";
    std::cout << "SUB2 erase: " << s2.str() << "\n";
    std::cout << "SUB3 merge: " << s3.str() << "\n";
    std::cout << "SUB4 kth_smallest: " << s4.str() << "\n";
    std::cout << "SUB5 to_vector: " << s5.str() << "\n";
    std::cout << "TOTAL: " << totalPassed << "/" << totalTests << "\n";

    // ---- Machine-readable single-line JSON ----
    std::cout << "{\"sub1\":\"" << s1.str() << "\",\"sub2\":\"" << s2.str()
              << "\",\"sub3\":\"" << s3.str() << "\",\"sub4\":\"" << s4.str()
              << "\",\"sub5\":\"" << s5.str() << "\",\"total\":\""
              << totalPassed << "/" << totalTests << "\"}\n";

    return (totalPassed==totalTests)? 0 : 1;
}
