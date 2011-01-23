#include <iostream>
#include <cstring>
#include <vector>
#include <assert.h>
using namespace std;
const string NO_MEMORY = "not enough memory";
const string BAD_PARAM = "bad parameter!";

typedef pair<int,int> IntPair;
typedef struct {int x, y;} Coords;
typedef struct {int pos, offset;} BytePos;
typedef unsigned char Byte;
const IntPair movements[] = {IntPair(-1,0), IntPair(0,-1), IntPair(1,0), IntPair(0,1)};
const int numMoves = sizeof(movements)/sizeof(IntPair);
class DuctPath {
	Byte* rooms;
	int w;
	int h;
	unsigned int size;
    unsigned int count;
	IntPair start;
	IntPair finish;
	const IntPair toByteOffset(IntPair where) const {
		if (where.first<0 || where.first>= w || where.second<0 || where.second>=h) return IntPair(-1,-1);
		int pos = where.second*w+where.first;
		int byte = pos/8;
		int offset = pos%8;
		return IntPair(byte, offset);
	}
	bool setPos(IntPair where, Byte*& rooms) {
		//where -> (x,y)
		IntPair bo = toByteOffset(where);
		if (bo.first<0 && bo.second<0) return false;
		(*(rooms+bo.first)) |= (1<<bo.second);
		return true;
	}
	const int getPos(IntPair where, Byte*& rooms) const {
		IntPair bo = toByteOffset(where);
		if (bo.first<0 && bo.second<0) return -1;
		return (*(rooms + bo.first)) & (1<<bo.second);
	}
public:
	DuctPath(int w, int h, vector<int> rooms) {
		this->w = w;
		this->h = h;
		size = w*h;
        assert(rooms.size()==size);
		this->size = (size%8)? (size/8)+1: size/8;
		start = finish = IntPair(0,0);
		this->rooms = new Byte(size);
        this->count = 0;
		if (!this->rooms) throw NO_MEMORY;
        for (int y=0;y<h;y++) {
            for (int x=0;x<w;x++) {
                switch(rooms[y*w+x]) {
                    case 1:
                        setPos(IntPair(x,y), this->rooms); this->count++; break;
                    case 2:
                        start = IntPair(x,y);break;
                    case 3:
                        finish = IntPair(x,y);break;
                    default:
                        throw BAD_PARAM;
                }
            }
        }
	}
	
	int calcPath(IntPair start = IntPair(-1,-1), IntPair finish = IntPair(-1,-1)) {
        if (start==IntPair(-1,-1)) start = this->start;
        if (finish==IntPair(-1,-1)) finish = this->finish;
        return calcStep(start, this->rooms, this->count);
	}
	~DuctPath() {
		if (rooms) delete[] rooms;
	}
	int calcStep(IntPair prevPos, Byte* rooms, unsigned int count) {
        if (getPos(prevPos, rooms)) return 0;
		Byte* myRooms = new Byte(size);
		if (!myRooms) throw NO_MEMORY;
		memcpy(myRooms, rooms, size);
		setPos(prevPos, myRooms);
		count+=1;
		if (count==size*8) {
            delete myRooms;
			return (prevPos == finish);
        }
		int result = 0;
		for (int i=0;i<numMoves;i++) {
			IntPair newPos = IntPair(prevPos.first+movements[i].first, prevPos.second+movements[i].second);
			result+=calcStep(newPos, myRooms, count);
		}
        delete myRooms;
		return result;
	}
};

int main(int argc, char* argv[]) {
    return 0;
}
