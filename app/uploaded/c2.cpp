#include<stdio.h>
using namespace std
int main(){
	int t;
	scanf("%d",&t);
	int n;
	while(t--){
		scanf("%d",&n);
		int arr[n+1];
		for(int i=1;i<=n;i++)
			scanf("%d",&arr[i]);
		int flag=0;
		int j=2;
		for(int i=1;i<n;i++){
			if(arr[i]!=arr[j]){
				if(arr[arr[i]]==arr[arr[j]])
					flag=1;
					break;
			}
			j++;
		}
		if(flag)
			printf("Truly Happy\n");
		else
			printf("Poor Chef");
	}
	return 0;
}
