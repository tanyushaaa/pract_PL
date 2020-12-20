#include <iostream>
#include <cstring>
#include <exception>
#include <cmath>

using namespace std;

class array {
	int size;
	int *ptr_arr;

public:
	array(int n){
		size = n;
		ptr_arr = new int[size];
		if (ptr_arr == nullptr) {
			throw bad_alloc();
		}
	}
	~array(){
		delete [] ptr_arr;
	}

	array(const array& my_array){
		size = my_array.size;
		ptr_arr = new int[size];

		memcpy(ptr_arr, my_array.ptr_arr, size*sizeof(int));
	}

	void set(int i, int value){
		if (i >= 0 && i < size && value <= 100 && value >= -100)
			ptr_arr[i] = value;
		else {
			if (i >= size || i < 0) {
				throw out_of_range("out of range");
			}
			if (value < -100 || value > 100) {
				throw invalid_argument("invalid argument");
			}
		}
	}

	int get(int i){
		if (i < size && i >= 0)
			return ptr_arr[i];
		else throw invalid_argument("invalid argument");
	}

	int min (const array& some_arr){
		return (size - some_arr.size);
	}

	void sum (const array& some_arr){
		if (min(some_arr) == 0 || min(some_arr) < 0){
			for (int i = 0; i < size; i++){
				ptr_arr[i] += some_arr.ptr_arr[i];
			}
		}
		else {
			for (int j = 0; j < some_arr.size; j++){
				ptr_arr[j] += some_arr.ptr_arr[j];
			}
		}
	}

	void sub (const array& some_arr){
		if (min(some_arr) == 0 || min(some_arr) < 0){
			for (int i = 0; i < size; i++){
				ptr_arr[i] -= some_arr.ptr_arr[i];
			}
		}
		else {
			for (int j = 0; j < some_arr.size; j++){
				ptr_arr[j] -= some_arr.ptr_arr[j];
			}
		}
	}

	void operator = (const array& some_array){
		size = some_array.size;
		ptr_arr = new int[size];

		memcpy(ptr_arr, some_array.ptr_arr, size*sizeof(int));
	}

	double distance (const array& some_array){
		if (min(some_array) == 0){
			double distance = 0;
			for (int i = 0; i < size; i++){
				distance += (some_array.ptr_arr[i] - ptr_arr[i])*(some_array.ptr_arr[i] - ptr_arr[i]);
			}

			return sqrt(distance);
		}
		return -1;
	}

	friend ostream& operator << (ostream& out, const array& my_array);
};

ostream& operator << (ostream& out, const array& my_array){
	for (int i = 0;i < my_array.size; i++){
		out << my_array.ptr_arr[i] << ' ';
	}
	return out;
}

int main (){
	try {
		array my_array(10);
		my_array.set(8, 3);
		my_array.set(10, 4);
		my_array.get(-1);
		my_array.get(8);
	} catch (bad_alloc&){
		cerr << "bad allocation during creation" << endl;
	} catch (invalid_argument& ia){
		cerr << ia.what();
	} catch (out_of_range& oor){
		cerr << oor.what();
	}

	cout << my_array << endl;

	return 0;
}
