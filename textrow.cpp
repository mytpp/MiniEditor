#include "textrow.h"
#include "visitor\abstractvisitor.h"

TextRow::TextRow()
{
    row.push_back(QChar('\n'));
}


TextRow::TextRow(Qstring text)
{
    row.push_back(Qstring(text));
}

TextRow::QChar& operator[](int position)
{
    return row[position];
}

bool TextRow::insert(int position, QChar cha)
{
    int current_capacity=row.capacity();
    int current_size = row.size();
    
    int insert_position = position;
    int i;
    
    if( position > current_capacity  ||  positon > current_size )
          return false;                       //considering the position is undesirable
   
    if(position==current_capacity)
         row.reserve(current_capacity+50);    //considering the space is not enough
    
    while(row[position]!='\n')
         position++;
    
    for(i=position;i>=insert_position;i--)
         row[position+1]=row[position];
    
    row[insert_position]=cha;
    
    return true;
}

bool TextRow::insert(int position, QString str)
{
    int length = str.size();
    if(length==0)
        return true;
    
    if( position > current_capacity  ||  positon > current_size )
        return false;                           //considering the position is undesirable                        //considering oversteping the boundry
    
    if( current_size+length >= current_capacity )
    {    
        int n = length\50 + (length % 50 == 0)?0:1 ;
        row.reserve(current_capacity+n*50);     //considering the space is not enough
    }
    
    Qchar* data=str.data();
    int i=0;
    while(row[position]!='\n')
    {     
        row[position+length]=row[position];
        row[position]=data[i];
        i++;
        position++;
    }
    row[position+length]='\n';
    return true;
}


bool TextRow::erase(int position)
{
    int current_capacity=row.capacity();
    int current_size = row.size();
    
    if( position > current_capacity  ||  positon > current_size )
        return false;                           //considering the position is undesirable                        //considering oversteping the boundry
    
    
}

bool TextRow::erase(int begin, int end)
{
    
}

bool TextRow::traverse(AbstractVisitor &visitor)
{
    
}
