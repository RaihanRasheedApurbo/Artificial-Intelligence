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
    vector<vector<int>> studentCourseList;
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
        studentCourseList.push_back(currentStudent);
        
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

    for(auto &t : studentCourseList)
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
    int totalColorSoFar = 0;
    

    function<void()> largestEnrollment = [&] () -> void
    {
        for(int i=0;i<totalNode;i++)
        {
            colorAssigned.push_back(-5);
        }
        totalColorSoFar = 0;
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
            unordered_set <int> s;
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
        // cout<<totalColorSoFar<<endl;
        // for(i=0;i<colorFrequency.size();i++)
        // {
        //     cout<<i<<" "<<colorFrequency[i]<<endl;
        // }

    };

    function<void()> DSatur = [&] () -> void
    {
        for(int i=0;i<totalNode;i++)
        {
            colorAssigned.push_back(-5);
        }
        totalColorSoFar = 0;
        unordered_map<int,int> degree;
        for(int i=0;i<adjMatrix.size();i++)
        {
            int count = 0;
            for(int j=0;j<adjMatrix[i].size();j++)
            {
                if(adjMatrix[i][j]>0)
                {
                    count++;
                }
            }
            degree[i] = count;
        }
        unordered_map<int,int> sat;
        for(int i=0;i<degree.size();i++)
        {
            sat[i] = 0;
        }

        while(!sat.empty())
        {
            int cs = -5;  // current saturation
            int cd = -5;  // current degree
            int cv = -5;  // current vertex
            for(auto &t : sat)
            {
                int key = t.first;
                int val = t.second;
                if(cs<val)
                {
                    cv = key;
                    cd = degree[key];
                    cs = val;
                }
                else if(cs==val)
                {
                    if(cd<degree[key])
                    {
                        cv = key;
                        cd = degree[key];
                        cs = val;
                    }
                }
                
            }
            unordered_set<int> s;
            for(int i=0;i<totalColorSoFar;i++)
            {
                s.insert(i);
            }
            for(int i=0;i<adjMatrix[cv].size();i++)
            {
                if(adjMatrix[cv][i]>0)
                {
                    if(colorAssigned[i]>=0)
                    {
                        s.erase(colorAssigned[i]);
                    }
                }
            }
            int vColor;
            if(s.empty())
            {
                vColor = totalColorSoFar;
                totalColorSoFar++;
            }
            else
            {
                vColor = *(s.begin());
            }
            colorAssigned[cv] = vColor;
            for(int i=0;i<adjMatrix[cv].size();i++)
            {
                if(adjMatrix[cv][i]>0)
                {
                    if(sat.find(i)!=sat.end())
                    {
                        sat[i]++;
                    }
                }
            }
            sat.erase(cv);
            


        }
        cout<<totalColorSoFar<<endl;

    };
    DSatur();
    //largestEnrollment();

    function<float(void)> countPenalty = [&] () -> float 
    {
        float penalty = 0;
        unordered_map<int,int> penaltyAmount;
        penaltyAmount[1] = 16;
        penaltyAmount[2] = 8;
        penaltyAmount[3] = 4;
        penaltyAmount[4] = 2;
        penaltyAmount[5] = 1;
        for(auto &t : studentCourseList)
        {
            vector<int> slots;
            for(auto &t1 : t)
            {
                slots.push_back(colorAssigned[t1]);
            }
            sort(slots.begin(),slots.end());
            int penaltyForAStudent = 0;
            for(int i=1;i<slots.size();i++)
            {
                int dif = slots[i] - slots[i-1];
                if(dif<=5)
                {
                    penaltyForAStudent += penaltyAmount[dif];
                }
            }
            //cout<<penaltyForAStudent<<endl;
            penalty += penaltyForAStudent;

        }
        float t = studentCourseList.size();
        penalty = penalty/t;// avg penalty
        //cout<<"penalty: "<<penalty<<endl;
        return penalty;
        
    };

    float avgPenalty = countPenalty();
    float currentPenalty = avgPenalty;
    cout<<"total color needed: "<<totalColorSoFar<<endl;
    cout<<"penalty: "<<avgPenalty<<endl;
    function<void()> kempeChainInterchange = [&] () -> void 
    {
        for(int i=0;i<courseFrequency.size();i++)
        {

            int vertex1 = courseFrequency[i].courseNo;
            int c1 = colorAssigned[vertex1];
            for(int j=i+1;j<courseFrequency.size();j++)
            {
                
                int vertex2 = courseFrequency[j].courseNo;
                int c2 = colorAssigned[vertex2];
                if(adjMatrix[vertex1][vertex2]<=0)
                {
                    continue;
                }
                
                bool *visited = new bool[totalNode];
                for(int z=0;z<totalNode;z++)
                {
                    visited[z] = false;
                }
                

                vector<int> sameColorVertices;

                stack<int> s;
                s.push(vertex2);
                s.push(vertex1);
                sameColorVertices.push_back(vertex1);
                sameColorVertices.push_back(vertex2);
                visited[vertex1] = true;
                visited[vertex2] = true;
                while(!s.empty())
                {
                    int t = s.top();
                    s.pop();
                    for(int i1=0;i1<adjMatrix[t].size();i1++)
                    {
                        if(adjMatrix[t][i1]>0 && visited[i1]==false)
                        {
                            if(colorAssigned[i1]==c1 || colorAssigned[i1]==c2)
                            {
                                s.push(i1);
                                sameColorVertices.push_back(i1);
                                visited[i1] = true;

                            }
                        }
                    }
                }

                for(auto &t : sameColorVertices)
                {
                    if(colorAssigned[t]==c1)
                    {
                        colorAssigned[t] = c2;
                    }
                    else
                    {
                        colorAssigned[t] = c1;
                    }
                }

                float newPenalty = countPenalty();
                //cout<<"newPenalty: "<<newPenalty<<endl;
                if(newPenalty>currentPenalty)
                {
                    for(auto &t : sameColorVertices)
                    {
                        if(colorAssigned[t]==c1)
                        {
                            colorAssigned[t] = c2;
                        }
                        else
                        {
                            colorAssigned[t] = c1;
                        }
                    }
                }
                else
                {
                    currentPenalty = newPenalty;
                }
                



                
            }
        }
    };
    kempeChainInterchange();
    cout<<"after kempe chain interchange new penalty: "<<currentPenalty<<endl;
    cout<<"bye1"<<endl;
    
     return 0;
}