#include<bits/stdc++.h>
using namespace std;

int main()
{
    cout<<"kill meh"<<endl;
    

    int n;
    cin>>n;
    vector<vector<int>> square;
    for(int i=0;i<n;i++)
    {
        vector<int> t;
        for(int j=0;j<n;j++)
        {
            int p;
            cin>>p;
            t.push_back(p);
        }
        square.push_back(t);
    }

    // cout<<"I am printing now"<<endl;
    // for(int i=0;i<n;i++)
    // {
    //     for(int j=0;j<n;j++)
    //     {
    //         cout<<square[i][j]<<" ";
    //     }
    //     cout<<endl;
    // }

    
    function<bool()> rec = [&] () -> bool 
    {
        int r,c;
        bool found = false;
        for(int i=0;i<n;i++)
        {
            for(int j=0;j<n;j++)
            {
                if(square[i][j]==0)
                {
                    r = i;
                    c = j;
                    found = true;
                    break;

                }
            }
            if(found)
            {
                break;
            }
        }
        if(found==false)
        {
            return true;
        }
        vector<bool> contenders;
        for(int i=0;i<n;i++)
        {
            contenders.push_back(true);
        }
        
        for(int i=0;i<n;i++)
        {
            int t1 = square[r][i]; // row search
            int t2 = square[i][c]; // col search
            if(t1!=0)
            {
                contenders[t1-1] = false;
            }
            if(t2!=0)
            {
                contenders[t2-1] = false;
            }
        }
        for(int i=0;i<n;i++)
        {
            if(contenders[i]==true)
            {
                int value = i+1;
                square[r][c] = value;
                bool r = rec();
                if(r==true)
                {
                    return true;
                }

            }
        }
        square[r][c] = 0 ;
        return false;

        


    };
    
    rec();
    
    cout<<"I am printing now"<<endl;
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++)
        {
            cout<<square[i][j]<<" ";
        }
        cout<<endl;
    }
    cout<<"testing"<<endl;
    bool result = true;
    for(int i=0;i<n;i++)
    {
        vector<bool> t1;
        vector<bool> t2;
        for(int j=0;j<n;j++)
        {
            t1.push_back(false);
            t2.push_back(false);
        }
        for(int j=0;j<n;j++)
        {
            int n1,n2;
            n1 = square[i][j];
            n2 = square[j][i];
            if(n1==0||n2==0)
            {
                result = false;
                break;
            }
            t1[n1-1] = true;
            t2[n2-1] = true;
        }
        for(int i=0;i<n;i++)
        {
            if(t1[i]==false || t2[i]==false)
            {
                result = false;
                break;
            }
        }
        if(result==false)
        {
            break;
        }
    }
    if(result==true)
    {
        cout<<"valid square"<<endl;
    }
    else
    {
        cout<<"invalid square"<<endl;
    }
    




}