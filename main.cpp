#include <bits/stdc++.h>

using namespace std;
typedef long long ll;
typedef vector<int> vi;
typedef vector<vector<string> > vvs;

#define F first
#define S second

//-----------------------------------------------------
//attribute-> different values
map<string,vector<string> > attr_vals;

/// weather:cold,rain
/// label=weather   :child["cold"]=new node(); ,  child["rain"]=new node();
struct node
{
    string label;
    map<string,node*> child;
    map<string,bool> value;
    map<string,bool> isleaf;
};




double entropy(double pos,double neg)
{
    if(pos==0 || neg ==0 )return 0;
    return -pos/(pos+neg) * log2(pos/(pos+neg)) - neg/(pos+neg) * log2(neg/(pos+neg));
}


double gain(vector<pair<int,int> > v,int sum_pos,int sum_neg)
{
    //cout<<sum_pos<<" pn "<<sum_neg<<endl;
    double sum_entropy=0.0;
    for(int i=0; i<v.size(); i++)
    {
        int pos=v[i].first;
        int neg=v[i].second;
        sum_entropy+= entropy(pos,neg)*((pos+neg)/((double)(sum_pos+sum_neg)));
    }
    // cout<<"sume="<<sum_entropy<<endl;
    return entropy(sum_pos,sum_neg)-sum_entropy;
}



string getbestGain(vvs data)
{
    string best_attr;
    double bestgain=0.0;

    for(int j=0; j<data[0].size()-1; j++)
    {
        vector<pair<int,int> > acc;
        int sum_pos=0,sum_neg=0;
        for(int z=0; z<attr_vals[data[0][j]].size(); z++)
        {
            int pos=0,neg=0;
            for(int i=1; i<data.size(); i++)
            {
                if(data[i][j]==attr_vals[data[0][j]][z])
                {
                    if(data[i][data[0].size()-1]=="Yes")pos++;
                    else neg++;
                }
            }
            sum_pos+=pos;
            sum_neg+=neg;
            acc.push_back( {pos,neg});
        }
        double ans=gain(acc,sum_pos,sum_neg);
        //cout<<ans<<" "<<data[0][j]<<endl;
        if(ans>bestgain)
        {
            bestgain=ans;
            best_attr=data[0][j];
        }

    }

    return best_attr;
}

vvs filterData(vvs data,string attr_name,string val)
{
    int attr_col=0;
    vvs filtered;
    vector<string> header;
    for(int j=0; j<data[0].size(); j++)
    {
        if(data[0][j]==attr_name)
        {
            attr_col=j;
            continue;
        }
        header.push_back(data[0][j]);
    }
    filtered.push_back(header);

    for(int i=1; i<data.size(); i++)
    {
        if(data[i][attr_col]==val)
        {
            vector<string> row;
            for(int j=0; j<data[i].size(); j++)
            {
                if(j==attr_col)continue;
                row.push_back(data[i][j]);
            }
            filtered.push_back(row);
        }
    }

    return filtered;
}

void printTable(vvs f)
{
    for(int i=0; i<f.size(); i++)
    {
        for(int j=0; j<f[i].size(); j++)
        {
            cout<<f[i][j]<<" ";
        }
        cout<<endl;
    }
}


pair<int,int> pure(vvs table,string attr,string value)
{
    int pos=0,neg=0,attr_col;

    for(int j=0; j<table[0].size(); j++)
        if(table[0][j]==attr)
        {
            attr_col=j;
            break;
        }

    for(int i=1; i<table.size(); i++)
    {
        if(table[i][attr_col]==value)
        {
            if(table[i][table[0].size()-1]=="Yes")pos++;
            else neg++;
        }
    }
    return {pos,neg};
}


bool vote(vvs table)
{
    int pos=0,neg=0;
    for(int i=1; i<table.size(); i++)
    {
        if(table[i][table[0].size()-1]=="Yes")pos++;
        else neg++;
    }
    return pos>=neg;
}

void dfs(node* n,vvs table)
{

    string label=getbestGain(table);
    //cout<<label<<endl;
    n->label=label;
    for(int i=0; i<attr_vals[label].size(); i++)
    {
        string val=attr_vals[label][i];
        pair<int,int> pn=pure(table,label,val);

        vvs newTable=filterData(table,label,val);

        if(pn.first==0 && pn.second==0)
        {
            bool res=vote(newTable);
            n->value[val]=res;
            n->isleaf[val]=true;
        }
        else if(pn.first==0)
        {
            n->value[val]=false;
            n->isleaf[val]=true;

        }
        else if(pn.second==0)
        {
            n->value[val]=true;
            n->isleaf[val]=true;

        }
        else
        {
            node* ch=new node();
            n->child[val]=ch;
            dfs(ch,newTable);
        }
    }


}

void determineQ(node* n,map<string,string> mq)
{
    string val=mq[n->label];
    if(n->isleaf[val]){
        cout<<(n->value[val]?"Yes":"No")<<endl;
        return;
    }else if(n->child.find(val)==n->child.end()){
      cout<<"Undefined"<<endl;
      return;
    }
    determineQ(n->child[val],mq);

}



int n,m;
int main()
{

/// read table data
    cin>>n>>m;
    vvs DATA(n);

    for(int i=0; i<n; i++)
    {
        for(int j=0; j<m; j++)
        {
            string s;
            cin>>s;
            DATA[i].push_back(s);
        }
    }

/// init attr_vals;
    for(int j=0; j<m-1; j++)
    {
        map<string,bool> mp;
        for(int i=1; i<n; i++)
        {
            if(!mp[DATA[i][j]])
            {
                mp[DATA[i][j]]=1;
                attr_vals[DATA[0][j]].push_back(DATA[i][j]);
            }
        }
    }


    node* root=new node();
    dfs(root,DATA);
    int q;
    cin>>q;

    for(int i=0; i<q; i++)
    {
        map<string,string> mq;
        for(int j=0; j<m-1; j++)
        {
          string s;
          cin>>s;
          mq[DATA[0][j]]=s;
        }
        determineQ(root,mq);
    }

    return 0;
}
