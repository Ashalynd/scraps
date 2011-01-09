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
};

int main (int argc, char* argv[]) {
	largenum N1;
	string out;
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
	cout<<"N2="<<out<<endl;
	return 0;
}
