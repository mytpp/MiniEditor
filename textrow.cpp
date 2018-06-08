#include "textrow.h"
#include "visitor\abstractvisitor.h"
#include <iterator>



TextRow::TextRow()
{
    row.push_back(QChar('\n'));
}


TextRow::TextRow(QString text)     //添加新内容到一行中
{
    int i;
    for(i=0;i<text.size();i++)
          row.push_back(text[i]);
}

 int TextRow::size() const
 {
     return row.size();
 }

 int TextRow::capacity() const
 {
     return row.capacity();
 }

QChar TextRow::operator[](int position)const
{
    qDebug()<<position;
    return row[position];
}

bool TextRow::insert(int position, QChar cha)
{
    int current_capacity=row.capacity();
    int current_size = row.size();
    
    if(position > current_size )
          return false;                                       //considering the position is undesirable
   
    if(current_size==current_capacity)
         row.reserve(current_capacity+50);    //considering the space is not enough


    auto i = row.begin();
    advance(i,position);

    row.insert(i,cha);
    return true;
}

bool TextRow::insert(int position, QString str)
{
    int length = str.size();
    int current_capacity=row.capacity();
    int current_size = row.size();
    if(length==0)
        return true;
    
    if(  position > current_size )
        return false;                           //considering the position is undesirable

    if( current_size+length >= current_capacity )
    {    
        int n=length/50;
        if( length%50!=0 )
              n = n+1;
        row.reserve(current_capacity+n*50);      //considering the space is not enough
    }

//    QChar* data=str.data();             //inserting the whole string one by one

//    auto j = row.begin();
//    advance(j,position);

//    for(int i=0;i<length;i++,j++)
//        row.insert(j,data[i]);

    //shift QChar in the 'row' whose position is after 'pos' backwoard by 'length' characters
    for(int i=current_size+length-1; i>position+length-1; --i)
        row[i] = row[i-length];
    //insert the string
    for(int i=position; i<position+length; ++i)
        row[i] = str[i-position];

    return true;
}


/*-------删除一行中position位置上的字符--------*/
bool TextRow::erase(int position)     //position从第零位开始计
{
    int current_size = row.size();    //是否考虑换行符也算一个字符？？

    //considering the position is undesirable
    if(  position > current_size - 1  )
        return false;

    auto i = row.begin();
    advance(i,position);
    row.erase(i);
    return true;
}


/*--------删除区间为[begin,end)，左闭右开--------*/
/*begin和end直接表示下标 ，删除到end的前一位，*/
bool TextRow::erase(int begin, int end)
{
    int current_size = row.size();

    //considering the position is undesirable
    if(  end > current_size   || begin < 0 )    //删除区间的开头不可以小于零，结尾不可以超过当前串的结尾
        return false;

    auto i = row.begin();
    advance(i,begin);
    auto j = row.begin();
    advance(j,end);

    row.erase(i,j);
    return true;

}

bool TextRow::traverse(AbstractVisitor &visitor)
{
    for(auto i=row.begin(); i!=row.end(); i++)
        if(!visitor.visit(*i))
                return false;
    return true;
}
