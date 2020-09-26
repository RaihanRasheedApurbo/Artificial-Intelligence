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
    int totalColorSoFar = 0;
    vector<int> colorFrequency;

    function<bool(Node,Node)> compareNode = [&] (Node n1, Node n2) -> bool
    {
        return n1.studentsTaken<n2.studentsTaken;

    };

    sort(courseFrequency.begin(),courseFrequency.end(),compareNode);
    reverse(courseFrequency.begin(),courseFrequency.end());

    for(auto &t : courseFrequency)
    {
        int vertex = t.courseNo;
        set<int> s;
        for(int i=0;i<totalColorSoFar;i++)
        {
            s.insert(i);
        }
        for( int i=0;i<adjMatrix[vertex].size();i++)
        {
            int t1 = adjMatrix[vertex][i];
            if(t1>0)
            {
                int color = colorAssigned[i];
                if(color>=0)
                {
                    s.erase(color);
                }
            }
        }
        int colorContender = -1;
        int contenderFrequency = INT_MAX;

        for( auto it = s.begin(); it!=s.end() ; it++)
        {
            int color = *it;
            int freq = colorFrequency[color];
            if(freq<=contenderFrequency)
            {
                colorContender = color;
                contenderFrequency = freq;
            }
        }

        if(colorContender == -1)
        {
            colorContender = totalColorSoFar;
            totalColorSoFar++;
            contenderFrequency = 1;
            colorFrequency.push_back(contenderFrequency);
        }
        else
        {
            colorFrequency[colorContender]++;
        }
        colorAssigned[vertex] = colorContender;
        



    }
    cout<<totalColorSoFar<<endl;



    


    
     return 0;
}