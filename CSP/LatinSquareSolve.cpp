#include <bits/stdc++.h>
using namespace std;

int main()
{
    cout << "kill meh" << endl;

    int n;
    cin >> n;
    vector<vector<int>> square;
    for (int i = 0; i < n; i++)
    {
        vector<int> t;
        for (int j = 0; j < n; j++)
        {
            int p;
            cin >> p;
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

    function<void(vector<vector<int>> &)> latinSquareTest = [&](vector<vector<int>> &s) -> void {
        cout << "I am printing now" << endl;
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                cout << s[i][j] << " ";
            }
            cout << endl;
        }
        cout << "testing" << endl;
        bool result = true;
        for (int i = 0; i < n; i++)
        {
            vector<bool> t1;
            vector<bool> t2;
            for (int j = 0; j < n; j++)
            {
                t1.push_back(false);
                t2.push_back(false);
            }
            for (int j = 0; j < n; j++)
            {
                int n1, n2;
                n1 = s[i][j];
                n2 = s[j][i];
                if (n1 == 0 || n2 == 0)
                {
                    cout << "here1" << endl;
                    result = false;
                    break;
                }
                t1[n1 - 1] = true;
                t2[n2 - 1] = true;
            }
            for (int j = 0; j < n; j++)
            {
                if (t1[j] == false || t2[j] == false)
                {
                    cout << "here2" << endl;
                    cout << i << endl;

                    result = false;
                    break;
                }
            }
            if (result == false)
            {
                break;
            }
        }

        if (result == true)
        {
            cout << "valid square" << endl;
        }
        else
        {
            cout << "invalid square" << endl;
        }
    };

    function<void(void)> brelaz = [&](void) -> void {
        int numberOfNode = 0;    // for report
        int numberOfFailure = 0; // for report
        vector<vector<int>> table;
        vector<vector<unordered_set<int>>> contenders;
        for (int i = 0; i < n; i++)
        {
            vector<int> t;
            vector<unordered_set<int>> t1;
            for (int j = 0; j < n; j++)
            {
                t.push_back(square[i][j]);
                unordered_set<int> t3;
                if (square[i][j] == 0)
                {
                    for (int k = 0; k < n; k++)
                    {
                        t3.insert(k + 1);
                    }
                }
                t1.push_back(t3);
            }
            contenders.push_back(t1);
            table.push_back(t);
        }
        // cout<<"before removing"<<endl;
        // for(int i=0;i<n;i++)
        // {
        //     for(int j=0;j<n;j++)
        //     {
        //         cout<<i<<" "<<j<<" "<<table[i][j]<<endl;
        //         unordered_set<int> &t1 = contenders[i][j];
        //         for(auto it=t1.begin();it!=t1.end();it++)
        //         {
        //             cout<<*it<<" ";
        //         }
        //         cout<<endl;
        //     }

        // }

        for (int i = 0; i < n; i++)
        {
            unordered_set<int> t; // for rows
            // getting non zero element in each rows
            for (int j = 0; j < n; j++)
            {
                if (square[i][j] != 0)
                {
                    t.insert(square[i][j]);
                }
            }
            // removing non zero element found in this rows from contenderlist of cell having value 0
            for (auto it = t.begin(); it != t.end(); it++)
            {
                for (int j = 0; j < n; j++)
                {
                    unordered_set<int> &t1 = contenders[i][j];
                    t1.erase(*it);
                }
            }
        }

        for (int i = 0; i < n; i++)
        {
            unordered_set<int> t; // for colums
            // getting non zero element in each colums
            for (int j = 0; j < n; j++)
            {
                if (square[j][i] != 0)
                {
                    t.insert(square[j][i]);
                }
            }
            // removing non zero element found in these colums from contenderlist of cell having value 0
            for (auto it = t.begin(); it != t.end(); it++)
            {
                for (int j = 0; j < n; j++)
                {
                    unordered_set<int> &t1 = contenders[j][i];
                    t1.erase(*it);
                }
            }
        }

        // cout << "after removing" << endl;
        // for (int i = 0; i < n; i++)
        // {
        //     for (int j = 0; j < n; j++)
        //     {
        //         cout << i << " " << j << " " << table[i][j] << endl;
        //         unordered_set<int> &t1 = contenders[i][j];
        //         for (auto it = t1.begin(); it != t1.end(); it++)
        //         {
        //             cout << *it << " ";
        //         }
        //         cout << endl;
        //     }
        // }
        typedef tuple<int, int, int, int> CellNode; // contains information for each cell
        // 4 int represents row,column,domain,and forward degree

        function<bool(CellNode, CellNode)> comparatorForBrelaz = [&](CellNode a, CellNode b) -> bool {
            if (get<2>(a) != get<2>(b))
            {
                return get<2>(a) < get<2>(b); // comparing domain size.. choosing smallest domain
            }
            return get<3>(a) < get<3>(b); // breaking ties by taking larger forward degree
        };
        function<bool(void)> brelazRec = [&](void) -> bool {
            vector<CellNode> selectionCriterion; // 4 int represents row,column,domain,and forward degree
            // and would be used in sorting to find which variable to pick for instantsiation

            for (int i = 0; i < n; i++)
            {
                for (int j = 0; j < n; j++)
                {
                    if (table[i][j] == 0)
                    {
                        int r = i;
                        int c = j;
                        int domainSize = contenders[i][j].size();
                        if (domainSize == 0)
                        {
                            numberOfFailure++;
                            return false;
                        }
                        int forwardDegree = 0;
                        for (int k = 0; k < n; k++)
                        {
                            if (table[i][k] == 0)
                            {
                                forwardDegree++;
                            }
                            if (table[k][j] == 0)
                            {
                                forwardDegree++;
                            }
                        }
                        // cout << r << " " << c << " " << domainSize << " " << forwardDegree << endl;
                        selectionCriterion.push_back(make_tuple(r, c, domainSize, forwardDegree));
                    }
                }
            }
            if (selectionCriterion.size() == 0)
            {
                return true;
            }
            //sort(selectionCriterion.begin(),selectionCriterion.end(),comparatorForBrelaz);
            // cout<<"printing after sort"<<endl;
            // for(int i=0;i<selectionCriterion.size();i++)
            // {
            //     auto &t = selectionCriterion[i];
            //     cout<<get<0>(t)<<" "<<get<1>(t)<<" "<<get<2>(t)<<" "<<get<3>(t)<<endl;
            // }
            CellNode minNode = selectionCriterion[0];
            for (int i = 0; i < selectionCriterion.size(); i++)
            {
                // if(get<2>(a)!=get<2>(b))
                // {
                //     return get<2>(a)<get<2>(b); // comparing domain size.. choosing smallest domain
                // }
                // return get<3>(a)<get<3>(b);
                CellNode &t = selectionCriterion[i];
                if (get<2>(minNode) > get<2>(t))
                {
                    minNode = t;
                }
                else if (get<2>(minNode) == get<2>(t))
                {
                    if (get<3>(minNode) < get<3>(t))
                    {
                        minNode = t;
                    }
                }
            }

            //cout<<"hello-1"<<endl;
            auto &t = minNode;
            //cout<<"hello-1.0001"<<endl;
            int row = get<0>(t);
            int col = get<1>(t);
            // cout << "picking " << row << " " << col << " cell" << endl;

            //cout<<"hello-1.0011"<<endl;
            //cout<<row<<" "<<col<<endl;
            auto &choices = contenders[row][col];
            //cout<<"hello-1.1"<<endl;
            if (choices.size() == 0)
            {
                numberOfFailure++;
                return false;
            }
            //cout<<"hello-2"<<endl;
            vector<int> choiceCopy; // copying so that iterator doesn't shift while recursion....
            // it game be bug when i used set iterator inside next for loop that calls the recursion...
            // cout << "printing contenders: " << endl;
            for (auto it = choices.begin(); it != choices.end(); it++)
            {
                choiceCopy.push_back(*it);
                // cout << *it << " ";
            }
            // sort(choiceCopy.begin(), choiceCopy.end());
            // cout << endl;
            for (int it = 0; it < choiceCopy.size(); it++)
            {
                //cout<<"hello"<<endl;
                int value = choiceCopy[it];
                // cout << "choosing value " << value << endl;

                if (value < 0 || value > 10)
                {
                    //cout<<"hi\n";
                }
                //cout<<"hello1"<<endl;
                vector<pair<int, int>> updatedNeighbors;
                for (int j = 0; j < n; j++)
                {
                    if (table[row][j] == value)
                    {
                        cout << "shouldn't happen" << endl;
                    }
                    if (table[j][col] == value)
                    {
                        cout << "shouldn't happen" << endl;
                    }
                    auto &t1 = contenders[row][j];
                    auto &t2 = contenders[j][col];

                    if (t1.find(value) != t1.end() && table[row][j] == 0)
                    {
                        t1.erase(value);
                        updatedNeighbors.push_back(make_pair(row, j));
                    }
                    if (t2.find(value) != t2.end() && table[j][col] == 0)
                    {
                        t2.erase(value);
                        updatedNeighbors.push_back(make_pair(j, col));
                    }
                }
                table[row][col] = value;
                //cout<<"hello2"<<endl;

                numberOfNode++;
                // cout << numberOfNode << " " << numberOfFailure << " " << selectionCriterion.size() << endl;
                bool result = brelazRec();
                if (result == true)
                {
                    return true;
                }
                table[row][col] = 0;

                for (int j = 0; j < updatedNeighbors.size(); j++)
                {
                    int r1 = updatedNeighbors[j].first;
                    int c1 = updatedNeighbors[j].second;

                    contenders[r1][c1].insert(value);
                }
                //cout<<"hello3"<<endl;
            }

            //cout<<"hello4"<<endl;

            return false;
        };

        brelazRec();
        /*function<bool(void)> brelazRec = [&] (void)->bool
        {
            vector<CellNode> selectionCriterion; // 4 int represents row,column,domain,and forward degree 
            // and would be used in sorting to find which variable to pick for instantsiation

            for(int i=0;i<n;i++)
            {
                for(int j=0;j<n;j++)
                {
                    if(table[i][j]==0)
                    {
                        int r = i;
                        int c = j;
                        
                        unordered_set<int> t;//for calculating domain size
                        for(int k=0;k<n;k++)
                        {
                            t.insert(k+1);
                        }
                        int forwardDegree = 0;
                        for(int k=0;k<n;k++)
                        {
                            if(table[i][k]!=0)
                            {
                                t.erase(table[i][k]);
                            }
                            if(table[k][j]!=0)
                            {
                                t.erase(table[k][j]);
                            }

                            if(table[i][k]==0)
                            {
                                forwardDegree++;
                            }
                            if(table[k][j]==0)
                            {
                                forwardDegree++;
                            }
                        }
                        if(t.size() == 0)
                        {
                            numberOfFailure++;
                            //cout<<"leaf node"<<endl;
                            return false;
                        }

                        selectionCriterion.push_back(make_tuple(r,c,t.size(),forwardDegree));
                    }
                     
                }
            }
            if(selectionCriterion.size()==0)
            {
                return true;
            }
            sort(selectionCriterion.begin(),selectionCriterion.end(),comparatorForBrelaz);
            // cout<<"printing after sort"<<endl;
            // for(int i=0;i<selectionCriterion.size();i++)
            // {
            //     auto &t = selectionCriterion[i];
            //     cout<<get<0>(t)<<" "<<get<1>(t)<<" "<<get<2>(t)<<" "<<get<3>(t)<<endl;
            // }

            for(int i=0;i<selectionCriterion.size();i++)
            {
                //cout<<"hello-1"<<endl;
                auto &t = selectionCriterion[i];
                //cout<<"hello-1.0001"<<endl;
                int row = get<0>(t);
                int col = get<1>(t);
                //cout<<"picking "<<row<<" "<<col<<" cell"<<endl;
                
                //cout<<"hello-1.0011"<<endl;
                //cout<<row<<" "<<col<<endl;
                //auto &choices = contenders[row][col];
                //cout<<"hello-1.1"<<endl;
                // if(choices.size()==0)
                // {
                //     numberOfFailure++;
                //     return false;
                // }
                //cout<<"hello-2"<<endl;
                //vector<int> choiceCopy; // copying so that iterator doesn't shift while recursion.... 
                // it game be bug when i used set iterator inside next for loop that calls the recursion...
                //cout<<"printing contenders: "<<endl;.
                unordered_set<int> choiceSet;
                for(int j=0;j<n;j++)
                {
                    choiceSet.insert(j+1);
                }
                for(int j=0;j<n;j++)
                {
                    choiceSet.erase(table[row][j]);
                    choiceSet.erase(table[j][col]);
                }
                if(choiceSet.size()==0)
                {
                    numberOfFailure++;
                    return false;
                }
                vector<int> choiceArr;
                for(auto it = choiceSet.begin();it!=choiceSet.end();it++)
                {
                    choiceArr.push_back(*it);
                }
                sort(choiceArr.begin(),choiceArr.end());
                // int len = choiceArr.size();
                // for(int j=0;j<len;j++)
                // {  
                //     int r1 = rand()%len;
                //     int r2 = rand()%len;
                //     int t = choiceArr[r1];
                //     choiceArr[r1] = choiceArr[r2];
                //     choiceArr[r2] = t;
                // }

                // cout<<endl;
                for(auto it = choiceArr.begin();it!=choiceArr.end();it++)
                {
                    //cout<<"hello"<<endl;
                    int value = *it;
                    //cout<<"choosing value "<<value<<endl;
                    
                    if(value<0||value>n)
                    {
                        cout<<"hi\n";
                    }
                    //cout<<"hello1"<<endl;
                    
                    for(int j=0;j<n;j++)
                    {
                        if(table[row][j]==value)
                        {
                            cout<<"shouldn't happen"<<endl;
                        }
                        if(table[j][col]==value)
                        {
                            cout<<"shouldn't happen"<<endl;
                        }

                        //contenders[row][j].erase(value);
                       
                        //contenders[j][col].erase(value);
                        
                    }
                    table[row][col] = value;
                    //cout<<"hello2"<<endl;
                    

                    numberOfNode++;
                    cout<<numberOfNode<<" "<<numberOfFailure<<" "<<selectionCriterion.size()<<endl;
                    cout<<row<<" "<<col<<endl;
                    // for(int i=0;i<n;i++)
                    // {
                    //     for(int j=0;j<n;j++)
                    //     {
                    //         cout<<table[i][j]<<" ";
                    //     }
                    //     cout<<endl;
                    // }
                    bool result = brelazRec();
                    if(result==true)
                    {
                        return true;
                    }
                    table[row][col] = 0;
                    // for(int j=0;j<n;j++)
                    // {
                    //     if(table[row][j]==0)
                    //     {
                    //         contenders[row][j].insert(value);
                    //     }
                    //     if(table[j][col]==0)
                    //     {
                    //         contenders[j][col].insert(value);
                    //     }
                        
                        
                    // }
                    //cout<<"hello3"<<endl;



                }
                
            }
            //cout<<"hello4"<<endl;
            
            return false;


            
        
        };
        */
        brelazRec();
        
        //solution printing and testing codes
        latinSquareTest(table);
        cout << "number of failure: " << numberOfFailure << endl;
        cout << "number of node visited: " << numberOfNode << endl;
    };

    brelaz();

    function<void(void)> bruteForceBackTracking = [&](void) -> void {
        int numberOfNode = 0;    // for report
        int numberOfFailure = 0; // for report

        function<bool()> rec = [&]() -> bool {
            int r, c;
            bool found = false;
            for (int i = 0; i < n; i++)
            {
                for (int j = 0; j < n; j++)
                {
                    if (square[i][j] == 0)
                    {
                        r = i;
                        c = j;
                        found = true;
                        break;
                    }
                }
                if (found)
                {
                    break;
                }
            }
            if (found == false)
            {
                return true;
            }
            vector<bool> contenders;
            for (int i = 0; i < n; i++)
            {
                contenders.push_back(true);
            }

            for (int i = 0; i < n; i++)
            {
                int t1 = square[r][i]; // row search
                int t2 = square[i][c]; // col search
                if (t1 != 0)
                {
                    contenders[t1 - 1] = false;
                }
                if (t2 != 0)
                {
                    contenders[t2 - 1] = false;
                }
            }
            bool atLeastOneContenderFound = false; // needed to find leaf nodes
            for (int i = 0; i < n; i++)
            {
                if (contenders[i] == true)
                {
                    atLeastOneContenderFound = true;
                    int value = i + 1;
                    square[r][c] = value;
                    numberOfNode++;
                    bool r = rec();
                    if (r == true)
                    {
                        return true;
                    }
                }
            }
            if (atLeastOneContenderFound == false)
            {
                numberOfFailure++;
            }
            square[r][c] = 0;
            return false;
        };
        rec();
        
        //solution printing and testing codes
        latinSquareTest(square);
        cout << "number of failure: " << numberOfFailure << endl;
        cout << "number of node visited: " << numberOfNode << endl;
    };

    bruteForceBackTracking();
}