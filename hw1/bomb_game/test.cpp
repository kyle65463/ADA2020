#include <iostream>
using namespace std;

int main()
{
    cout << "100001 100000\n";
    for(int i = 1; i <= 50000; i++){
        cout << "P " << i << " " << i << endl;
        cout << "A " << i << " " << i + 2 << " " << i + 3 << " " << i + 3 << endl;
    }
}