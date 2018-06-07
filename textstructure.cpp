#include "textstructure.h"
#include "Visitor/abstractvisitor.h"


TextStructure::TextStructure()
{

}

/*------返回position位置上的之间的QChar------*/
QChar TextStructure::data(std::pair<int,int> position) const
{

    int text_size=text.size();
    int row_size;

    QChar a();

    if(position.first>=text_size)
        qDebug()<<position.first;

    auto i = text.begin();
    advance(i,position.first);

    if( position.second>=i->size() )
        qDebug()<<position.second;

    return  i->operator [](position.second) ;
}


/*------返回begin和end之间的QString------*/
const QString TextStructure::data(std::pair<int, int> begin, std::pair<int, int> end)const
{
    QString result="";
    std::pair<int,int>position(begin.first,begin.second);
    QChar append_char;

    int text_size=text.size();
    int row_size;

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
             append_char = data(position);
             result.append(append_char);
         }
    }

    return result;
}



bool TextStructure::insert(std::pair<int, int> position, QChar character)
{
    int text_size=text.size();
    int row_size;

    if(position.first>=text_size)
        qDebug()<<position.first;

    auto i = text.begin();
    advance(i,position.first);

    if( position.second>=i->size() )
        qDebug()<<position.second;

    if(character=='\n')
    {
        TextRow insert_row=TextRow();
        text.insert(i,insert_row);
    }
    else
       (*i).insert(position.second, character);

    return true;
}

bool TextStructure::insert(std::pair<int, int> position, QString newString)
{
    int length=newString.size();
    int text_size=text.size();
    if(position.first>=text_size)
        qDebug()<<position.first;
    auto i = text.begin();
    advance(i,position.first);

    if( position.second>=i->size() )
        qDebug()<<position.second;

    int j;
    for(j=0;j<length;j++)
        TextStructure::insert(position,newString[j]);

    return true;
}

bool TextStructure::erase(std::pair<int, int> position)
{
    int text_size=text.size();
    if(position.first>=text_size)
        qDebug()<<position.first;
    auto i = text.begin();
    auto t = text.begin();
    auto j = text.end();
    advance(i,position.first);
    advance(t,position.first);

    if( position.second>=i->size() )
        qDebug()<<position.second;

    QChar a=TextStructure::data(position);

    i->erase(position.second);
    if(a=='\n')   //每行以换行符结尾
    {
       t++;
       if(t!=j)
       {
          QString merge_string="";
          for(int k=0;k<t->size();i++)
              merge_string.append(t->operator [](k));
          TextStructure::insert(position, merge_string);
          text.erase(t);
       }
    }

    return true;
}

bool TextStructure::erase(std::pair<int, int> begin, std::pair<int, int> end)
{

    std::pair<int, int>current_position;
    auto m =text.begin();
    advance(m,begin.first);

    for(current_position.first=begin.first;current_position.first<=end.first;current_position.first++)
    {
        if(current_position.first==begin.first)
        {
            for(current_position.second=begin.second;current_position.second<m->size();current_position.second++)
                erase(current_position);
        }
        else
        {
            for(current_position.second=0;current_position.second<m->size();current_position.second++)
                erase(current_position);
        }
    }
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
