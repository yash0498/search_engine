#include<sstream>
#include<iostream>
#include<cstdlib>
#include<cstring>
#include<fstream>
#include<map>
#include<vector>
#include<algorithm>

using namespace std;

#define alphabet_size 26

struct trie_node
{
    struct trie_node *children[alphabet_size];
    int isEndOfWord;
};

struct trie_node *get_node()
{
    struct trie_node *tmp = (struct trie_node*)malloc(sizeof(struct trie_node));

    tmp->isEndOfWord = 0;

    int i;
    for(i = 0;i<alphabet_size;i++)
        tmp->children[i] = NULL;

    return tmp;
};

void insert(struct trie_node **root,string key,int co[26])
{
    struct trie_node *p = (*root);
    int i;

    int in = key[0];

    if(in >= 65 && in <= 90)
        in += 32;
    co[in - 'a']++;
     for(i=0;i<key.length();i++)
     {

        int ind = (int)key[i];

        if(ind >= 65 && ind <= 90)
            ind += 32;

        int index = ind - 'a';

        if(!p->children[index])
            p->children[index] = get_node();

        p = p->children[index];
    }

    p->isEndOfWord = 1;
}

int search(struct trie_node *root,string key)
{
    struct trie_node *p = root;
    int i,l = key.length();

    for(i=0;i<key.length();i++)
    {
        int index = (int)key[i] - 'a';

        if(!p->children[index])
            return 0;

        p = p->children[index];

    }

    return (p != NULL && p->isEndOfWord);
}

int have_children(struct trie_node *root)
{
    int i;
    for(i=0;i<alphabet_size;i++)
    {
        if(root->children[i]!=NULL)
            return 1;
    }
    return 0;
}

int del_node(struct trie_node **root,string key,int in)
{
    if(*root == NULL)
        return 0;

    if(key[in])
    {
        int index = (int)key[in] - 'a';
        if((*root) != NULL && (*root)->children[index] != NULL &&
           del_node(&(*root)->children[index],key,in+1)&& (*root)->isEndOfWord == 0)
        {
            if(!have_children(*root))
            {
                free(*root);
                (*root) = NULL;
                return 1;
            }
            else
                return 0;
        }
    }

    if(key[in] == '\0' && (*root)->isEndOfWord)
    {
        if(!have_children(*root))
        {
            free(*root);
            (*root) = NULL;
            return 1;
        }
        else
        {
            (*root)->isEndOfWord = 0;
            return 0;
        }
    }
}

int is_endnode(struct trie_node *root)
{
    return root->isEndOfWord != 0;
}

void display(struct trie_node *root,string str,int level)
{
    if (is_endnode(root))
    {
        str[level] = '\0';
        cout<<str<<"\n";
    }

    int i;
    for (i = 0; i < alphabet_size; i++)
    {
        // if NON NULL child is found
        // add parent key to str and
        // call the display function recursively
        // for child node
        if (root->children[i])
        {
            str[level] = i + 'a';
            display(root->children[i], str, level + 1);
        }
    }
}

void getfile_content(string file,struct trie_node *root,int co[26],map<string,vector<string> > &vi)
{
     string line;
     string buf;
     ifstream fin(file.c_str());
     getline(fin,line);

     while(!fin.eof())
     {
         stringstream check1(line);
         getline(fin,line);
         getline(check1,buf,' ');

            //cout<<buf<<"\n";
            insert(&root,buf,co);

            string ke = buf;
            transform(ke.begin(),ke.end(),ke.begin(), ::tolower);
            //cout<<"ll "<<ke<<" ll";
            while(getline(check1,buf,','))
            {
                //cout<<"kk\n";
                //printf("%s kk \n",buf);

                    //strcpy(tmp,buf);
                    vi[ke].push_back(buf);
                    //cout<<buf<<" ";
            }
            //cout<<"\n";
     }
}

int main()
{
    int co[26] = {0};
     struct trie_node *root = get_node();
     map<string,vector<string> > vi;

     //struct info inf[280],d,*tmp;

     //freopen("paragraph1.txt","rb",stdin);

     //tmp = &inf[0];

     getfile_content("synonyms.txt",root,co,vi);

     int c,b;
     char *a,k;
     string str;
     a = (char *)malloc(sizeof(char));


     int level = 0,no;
     char s[20];
     string temp;

   while(1)
   {
     cout<<"enter choice\n";
     cout<<"1 : insert a given key\n";
     cout<<"2 : delete a given key\n";
     cout<<"3 : find a given key\n";
     cout<<"4 : synonyms\n";
     cout<<"5 : list the content of table\n";
     cout<<"6 : exit\n";

     cin>>c;

     switch(c)
     {
         case 1: cout<<"\nenter word to insert\n";
                 cin>>str;
                 insert(&root,str,co);
                 cout<<"enter number synonyms you want to insert\n";
                 cin>>no;
                 for(int i=0;i<no;i++)
                 {
                     cin>>temp;
                     vi[str].push_back(temp);
                 }
                 break;
         case 2: cout<<"\nenter word to delete\n";
                 cin>>str;
                 del_node(&root,str,0);
                 break;
         case 3: cout<<"\nenter word to search\n";
                 cin>>str;
                 if(search(root,str))
                    cout<<"word found\n";
                 else
                    {
                        cout<<"word not found \n\n";
                        cout<<"I :ignore the word\n";
                        cout<<"A : add the word in dictionary\n";
                        cin>>k;

                        switch(k)
                        {
                            case 'I': break;

                            case 'A': insert(&root,str,co);
                                      break;
                            default : cout<<"invalid choice\n";
                                      break;
                        }
                    }
                 break;
         case 4: cout<<"\nenter word for which you want to find synonyms\n";
                 cin>>str;
                 if(search(root,str))
                 {
                     cout<<"\n synonyms are : ";
                     transform(str.begin(), str.end(), str.begin(), ::tolower);
                     auto it = vi[str].begin();
                     while(it != vi[str].end())
                     {
                         cout<<*it<<" ";

                         it++;
                     }
                     cout<<"\n";
                 }
                 else
                    cout<<"not found\n";
                 break;
         case 5: display(root,s,level);
                 break;
         case 6: exit(1);

         default: cout<<"wrong choice";
                  break;
     }
   }
    return 0;
}

