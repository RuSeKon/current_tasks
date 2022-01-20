/* only function for binary search */

int binsearch(int x, int a[], int n)
{
	int low = 0;
	int high = n-1;
	while(low < high) {
		mid = (low+high)/2;
		if(x <= a[mid])
			high = mid;
		else
			low = mid + 1;
	}
	if(x == a[low])
		return low;
	return -1;
}
