#include "textstructure.h"
#include "Visitor/abstractvisitor.h"


TextStructure::TextStructure()
{
    text.push_back(TextRow());
}

/*------返回position位置上的QChar------*/
QChar TextStructure::data(std::pair<int,int> position) const
{

    int text_size=text.size();

    if(position.first>=text_size)
        qDebug()<<position.first;

    auto i = text.begin();
    advance(i,position.first);

    if( position.second>=i->size() )
        qDebug()<<position.second;

    return  (*i)[position.second];
}


/*------返回begin和end之间的QString------*/
const QString TextStructure::data(std::pair<int, int> begin, std::pair<int, int> end)const
{
    QString result="";
    std::pair<int,int>position(begin.first,begin.second);
    QChar append_char;

    int text_size=text.size();

    if(begin.first>=text_size)
        qDebug()<<begin.first;
    if(end.first>=text_size)
        qDebug()<<end.first;

    auto i = text.begin();
    advance(i,begin.first);
    int j;

    for(;position.first<=end.first;position.first++,i++)
    {
         if(position.first==begin.first)
         {
             j=begin.second;
         }
         else
             j=0;
         for(;j<i->size();j++)
         {
             append_char = (*i)[j];
             result.append(append_char);
         }
    }

    return result;
}



bool TextStructure::insert(std::pair<int, int> position, QChar character)
{
    int text_size=text.size();

    if(position.first>=text_size)
        qDebug()<<position.first;

    auto i = text.begin();
    advance(i,position.first);

    if( position.second>=i->size() )
        qDebug()<<position.second;

    if(character=='\n')
    {
        QString rest = data(position, {position.first, i->size()});
        i->erase(position.second, i->size());
        text.emplace(++i, rest);
    }
    else
       i->insert(position.second, character);

    return true;
}

bool TextStructure::insert(std::pair<int, int> position, QString newString)
{
    qDebug()<<"txt ins";
    int length=newString.size();
    int text_size=text.size();
    if(position.first>=text_size)
        qDebug()<<position.first;
    auto i = text.begin();
    advance(i,position.first);

    if( position.second>=i->size() )
        qDebug()<<position.second;

    qDebug()<<position.first<<"  "<<position.second;
    int j;
    QString slice;
    for(j=0;j<length;j++){
        if(newString[j]=='\n'){
            QString rest = data(position, {position.first, i->size()});
            i->erase(position.second, i->size());
            i->insert(position.second, slice);
            text.emplace(++i, rest);
            position.second = 0;
            slice.clear();
        } else {
            slice.append(newString[j]);
        }
    }
        //TextStructure::insert(position,newString[j]);
    if(!slice.isEmpty())
        i->insert(position.second, slice);

    return true;
}

bool TextStructure::erase(std::pair<int, int> position)
{
    int text_size=text.size();
    if(position.first>=text_size)
        qDebug()<<position.first;
    auto i = text.begin();
    advance(i,position.first);
    auto t = i;
    auto j = text.end();

    if( position.second>=i->size() )
        qDebug()<<position.second;

    QChar a=(*i)[position.second];

    i->erase(position.second);
    if(a=='\n')   //每行以换行符结尾
    {
       t++;
       if(t!=j)
       {
          QString merge_string="";
          for(int k=0;k<t->size();k++)
              merge_string.append((*t)[k]);
          i->insert(position.second, merge_string);
          text.erase(t);
       }
    }

    return true;
}

bool TextStructure::erase(std::pair<int, int> begin, std::pair<int, int> end)
{

    //std::pair<int, int>current_position;
    auto m =text.begin();
    auto n =text.begin();
    advance(m,begin.first);
    advance(n, end.first);

    if(m == n) {
        (*m).erase(begin.second, end.second);
        return true;
    }

//    for(current_position.first=begin.first; current_position.first<=end.first; current_position.first++)
//    {
//        if(current_position.first==begin.first)
//        {
//            for(current_position.second=begin.second;current_position.second<m->size();current_position.second++)
//                erase(current_position);
//        }
//        else
//        {
//            for(current_position.second=0;current_position.second<m->size();current_position.second++)
//                erase(current_position);
//        }
//    }
    m = text.erase(++m, n);
    n = m--;
    m->erase(begin.second, m->size());
    n->erase(0, end.second);

    QString merge_string="";
    for(int k=0;k<n->size();k++)
        merge_string.append((*n)[k]);
    m->insert(begin.second, merge_string);
    text.erase(n);

    return true;
}


bool TextStructure::traverse(AbstractVisitor &visitor)
{
    for(auto i=text.begin(); i!=text.end(); i++)
        for(int j=0;j<i->size();j++)
           if(!i->traverse(visitor))
                return false;

    return true;

}
