#include "textstructure.h"
#include "Visitor/abstractvisitor.h"

/*构造函数*/
TextStructure::TextStructure()
{
    text.push_back(TextRow());
}

/*返回position位置上的QChar*/
QChar TextStructure::data(std::pair<int,int> position) const
{
    auto i = text.begin();
    advance(i,position.first);

    return  (*i)[position.second];
}


/*返回begin和end之间的QString*/
const QString TextStructure::data(std::pair<int, int> begin, std::pair<int, int> end)const
{
    QString result="";
    std::pair<int,int>position(begin.first,begin.second);
    QChar append_char;

    auto i = text.begin();
    advance(i,begin.first);
    int j;

    for(;position.first<=end.first;position.first++,i++)
    {
         if(position.first==begin.first)
             j=begin.second;
         else
             j=0;

         int sz = i->size();
         if(position.first == end.first)
             sz = end.second;
         for(; j<sz; j++)
         {
             append_char = (*i)[j];
             result.append(append_char);
         }
    }
    return result;
}

/*插入字符*/
bool TextStructure::insert(std::pair<int, int> position, QChar character)
{
    auto i = text.begin();
    advance(i,position.first);

    if(character=='\n')
    {
        QString rest = data(position, {position.first, i->size()-1});
        i->erase(position.second, i->size()-1);
        text.emplace(++i, rest);

    }
    else
       i->insert(position.second, character);

    return true;
}

/*插入字符串*/
bool TextStructure::insert(std::pair<int, int> position, QString newString)
{
    int length=newString.size();
    auto i = text.begin();
    advance(i,position.first);

    int j;
    QString slice;
    for(j=0;j<length;j++)
    {
        if(newString[j]=='\n'){
            QString rest = data(position, {position.first, i->size()-1});
            i->erase(position.second, i->size()-1);
            i->insert(position.second, slice);
            text.emplace(++i, rest);
            position.second = 0;
            slice.clear();
        } else {
            slice.append(newString[j]);
        }
    }
    if(!slice.isEmpty())
        i->insert(position.second, slice);

    return true;
}

/*删除字符*/
bool TextStructure::erase(std::pair<int, int> position)
{
    auto i = text.begin();
    advance(i,position.first);
    auto t = i;

    QChar a=(*i)[position.second];

    i->erase(position.second);
    if(a == '\n')   //每行以换行符结尾
    {
       t++;
       if(t != text.end())
       {
          QString merge_string;
          for(int k=0; k<t->size(); k++)   //包括末尾换行符
              merge_string.append((*t)[k]);
          i->insert(i->size(), merge_string);
          text.erase(t);
       }
    }

    return true;
}

/*删除字符串*/
bool TextStructure::erase(std::pair<int, int> begin, std::pair<int, int> end)
{
    auto m =text.begin();
    auto n =text.begin();
    advance(m,begin.first);
    advance(n, end.first);

    if(m == n) {
        (*m).erase(begin.second, end.second);
        return true;
    }

    m = text.erase(++m, n);
    n = m--;
    m->erase(begin.second, m->size());

    QString merge_string;
    for(int k=end.second; k<n->size(); k++)  //including '\n' at the end
        merge_string.append((*n)[k]);
    m->insert(m->size(), merge_string);
    text.erase(n);

    return true;
}


bool TextStructure::traverse(AbstractVisitor &visitor)
{
    for(auto i=text.begin(); i!=text.end(); i++)
        if(!i->traverse(visitor))
            return false;

    return true;
}
