#include<bits/stdc++.h>
using namespace std;
struct Node
{
    int courseNo;
    int studentsTaken;

};
int main()
{
    //cout<<"kill meh"<<endl;
    string studentFile = "yor-f-83.stu";
    string courseFile = "yor-f-83.crs";
    // string studentFile = "test2.txt";
    // string courseFile = "test.txt";
    vector<vector<int>> courses;
    ifstream file(studentFile);
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
    file.close();
    file.open(courseFile);

    vector<Node> courseFrequency;
    while (getline(file, str)) 
    {
        
        stringstream ss(str);
        int t1,t2;
        while(!ss.eof())
        {
            ss>>t1;
            ss>>t2;
            Node t3;
            t3.courseNo = t1;
            t3.studentsTaken = t2;
            courseFrequency.push_back(t3);
        }
        
        
        
    }
    int totalNode = courseFrequency.size()+1;  // 1 indexing so 0 is dummy node
    vector<vector<int>> adjMatrix;
    for(int i=0;i<totalNode;i++)
    {
        vector<int> t;
        for(int j=0;j<totalNode;j++)
        {
            t.push_back(0);
        }
        adjMatrix.push_back(t);
    }

    for(auto &t : courses)
    {
        for(int i=0;i<t.size();i++)
        {
            int leftVertex = t[i];
            for(int j=i+1;j<t.size();j++)
            {
                int rightVertex = t[j];
                adjMatrix[leftVertex][rightVertex]++;
                adjMatrix[rightVertex][leftVertex]++;
            }
        }
    }

    vector<int> colorAssigned;
    for(int i=0;i<totalNode;i++)
    {
        colorAssigned.push_back(-5);
    }
    // int totalColorSoFar = 0;

    // function<bool<Node,Node>> compareNode = 
    // {


    // } ;
    // for(auto &t1 : adjMatrix)
    // {
    //     for(auto &t2 : t1)
    //     {
    //         cout<<t2<<" ";
    //     }
    //     cout<<endl;
    // }




    // for(auto &t: courseFrequency)
    // {
    //     cout<<t.courseNo<<" "<<t.studentsTaken<<endl;
    // }

    
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