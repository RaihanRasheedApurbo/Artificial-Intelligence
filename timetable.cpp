#include<bits/stdc++.h>
using namespace std;
int main()
{
    //cout<<"kill meh"<<endl;
    
    vector<vector<int>> courses;
    ifstream file("yor-f-83.stu");
    string str;
    int i=0;
    while (getline(file, str)) 
    {
        vector<int> currentStudent;
        stringstream ss;
        ss<<str;
        while(!ss.eof())
        {
            int t;
            ss>>t;
            currentStudent.push_back(t);
        }
        courses.push_back(currentStudent);
        
    }
    
    // for(auto t1:courses)
    // {
    //     for(auto t2:t1)
    //     {
    //         cout<<t2<<" ";
    //     }
    //     cout<<endl;
    // }

    // cin>>t;
    // cout<<t<<endl;
    // cin>>t;
    // cout<<t<<endl;
    // cin>>t;
    // cout<<t<<endl;
    // cin>>t;
    // cout<<t<<endl;



    // freopen("yor-f-83.stu","r",stdin);

    // freopen("yor-f-83.stu","r",stdin);
    // int t;
    // cin>>t;
    // cout<<t<<endl;
     return 0;
}