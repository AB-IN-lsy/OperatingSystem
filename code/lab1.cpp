#include <bits/stdc++.h>

using namespace std;

map <int, int> m;

signed main()
{
	int n;
	cin >> n;
	for(int i = 1; i <= n; ++ i){
		int a, b;
		cin >> a >> b;
		m[a] = b;
	}
	for(auto [a, b] : m){
		cout << a << " " << b << '\n';
	}
	return 0;
}
