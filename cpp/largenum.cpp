#include <iostream>
#include <deque>
#include <string>
#include <sstream>
#include <assert.h>

using namespace std;

struct largenum {
	deque<int> op;
	bool neg;
	
	friend ostream& operator<< (ostream& out, largenum val) {
		if (val.neg) out<<'-';
		if (val.op.size()==0) {out<<'0';}
		else {
			for(deque<int>::const_reverse_iterator vi = val.op.rbegin(); vi < val.op.rend(); vi++) {
				out<<(*vi);
			}
		}
		return out;
	}
	
	bool readstring(const string& src, largenum& dst) {
		dst.clear();
		bool wasdigit = false;
		bool wassign = false;
		//verify that the string is valid
		for (string::const_iterator si = src.begin(); si<src.end();si++) {
			if (isspace(*si)) continue;
			if (isdigit(*si)) {
				wasdigit = true;
				continue;
			}
			if ( (((*si)=='-') || ((*si)=='+')) && !wasdigit && !wassign) {
				wassign = true;
				continue;
			}
			return false;
		}
		for (string::const_reverse_iterator si = src.rbegin(); si<src.rend();si++) {
			if (isspace(*si)) continue;
			else if (isdigit(*si)) {
				int t = (*si)-'0';
				dst.op.push_back(t);
			} else if ((*si)=='-') dst.neg=true;
		}
		while (!(*dst.op.rbegin())) dst.op.pop_back();
		return true;
	}
	
	void writestring(string& dst, const largenum& src) {
		ostringstream os;
		os<<src;
		dst = os.str();
	}
	void readlong(const long& arg, largenum& dst) {
		dst.clear();
		if (arg<0) dst.neg = true;
		long a = arg;
		while(a>0) {
			int rem = a%10;
			dst.op.push_back(rem);
			a = (a-rem)/10;
		}
	}
	largenum(const string& arg) {
		if (!readstring(arg, *this)) throw("bad input!");
	}
	largenum(long arg) {
		readlong(arg, *this);
	}
	largenum() {
		clear();
	}
	void clear() {
		op.clear();
		neg = false;
	}
	const bool operator>(const largenum& op1) const{
		if (this->op.size()>op1.op.size()) return true;
		if (this->op.size()<op1.op.size()) return false;
		deque<int>::const_reverse_iterator ti = this->op.rbegin(), vi = op1.op.rbegin();
		while(ti<op.rend() && vi<op1.op.rend() && ((*ti)==(*vi))) { ti++; vi++;}
		if (ti==op.rend()) {
			assert(vi==op1.op.rend());
			return false;
		}
		return ((*ti)>(*vi));
	}
	int popzeros(largenum& op) {
		int numzeros = 0;
		while(!(*op.op.begin())) {op.op.pop_front(); numzeros++;}
		return numzeros;
	}
	
	void pushzeros(largenum& op, const int numzeros) {
		for (int i=0;i<numzeros;i++) op.op.push_front(0);
	}
	
	friend largenum operator + (const largenum& op1, const largenum& op2) {
		int rest = 0;
		largenum dst;
		dst.clear();
		
		deque<int>::const_iterator it1 = op1.op.begin();
		deque<int>::const_iterator it2 = op2.op.begin();
		while (rest || it1<op1.op.end() || it2<op2.op.end()) {
			int sum = rest;
			if (it1<op1.op.end()) sum+=(*it1);
			if (it2<op2.op.end()) sum+=(*it2);
			int token = sum%10;
			rest = (sum-token)/10;
			dst.op.push_back(token);
			it1++;
			it2++;
		}
		return dst;
	}
	
	friend largenum operator - (const largenum& op1, const largenum& op2) {
		largenum dst;
		dst.clear();
		bool op1more = (op1>op2);
		bool op2more = (op2>op1);
		if (!op1more && !op2more) {
			return dst;
		}
		const largenum& opfrom = (op1more?op1:op2);
		const largenum& opwhat = (op1more?op2:op1);
		dst.neg = (!op1more);
		int rest = 0;
		deque<int>::const_iterator it1 = opfrom.op.begin();
		deque<int>::const_iterator it2 = opwhat.op.begin();
		while (it1<opfrom.op.end()) {
			int diff = (*it1) - rest;
			if (it2<opwhat.op.end()) diff-=(*it2);
			if (diff<0) {
				rest = (-diff/10) + (-diff%10?1:0);
				diff = rest*10+diff;
			} else rest = 0;
			dst.op.push_back(diff);
			it1++; it2++;
		}
		while((*dst.op.rbegin())==0) dst.op.pop_back();
		return dst;
	}
};

int main (int argc, char* argv[]) {
	largenum N1;
	string out, str;
	cout<<"N1="<<N1<<endl;
	largenum N2(123414454);
	cout<<"N2="<<N2<<endl;
	try{
	largenum N3("@#%$#@%@$%$#%$#$%@#%@#$%@#%@#$%");
	} catch(...) {
		cout<<"Exception!"<<endl;
	}
	largenum N4("23412432542354364356357676674563456435346345");
	//just to prove that it works as well
	N4.writestring(out, N4);
	cout<<"N4="<<out<<endl;
	cout<<"Enter first number"<<endl;
	cin>>str;
	largenum op1(str);
	cout<<"Enter second number"<<endl;
	cin>>str;
	largenum op2(str);
	cout<<"op1+op2="<<op1+op2<<endl;
	cout<<"op1-op2="<<op1-op2<<endl;
	return 0;
}
